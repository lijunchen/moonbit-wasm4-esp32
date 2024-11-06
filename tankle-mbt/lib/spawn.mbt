fn spawn_enemy_system(entities : Entities) -> Unit {
  let level = entities.levels.get(entities.level_idx)
  if level.is_empty() {
    return
  }
  let level = level |> w4expect("Impossible")
  if level.no_enemy() {
    return
  }
  level.time_count += 1
  if level.time_count < level.spawn_cd {
    return
  }
  let collisions = entities.tanks.map(fn(t) { t.collision() })
  let pos = get_free_pos(collisions, entities.random_state)
  if pos.is_empty() {
    return
  }
  let tank_model = level.enemies.pop()
  let tank_model = tank_model
    |> w4expect("No tank model. Please check if no_enemy function is correct")
  let tank = Tank::new(pos |> w4expect("Impossible"), Up, tank_model, Enemy)
  entities.tanks.push(tank)
  level.time_count -= level.spawn_cd
}

fn get_free_pos(
  occupied : Array[Collision],
  rand_state : @random.Rand 
) -> Pos? {
  let pos1 = { x: border_left, y: border_top }
  let pos2 = { x: border_right - tank_width, y: border_top }
  let pos3 = { x: border_right - tank_width, y: border_bottom - tank_width }
  let pos4 = { x: border_left, y: border_bottom - tank_width }
  for
    pos in [pos1, pos2, pos3, pos4].shuffle(
      rand=fn(upper) { rand_state.int(limit=upper) },
    ) {
    let col = Collision::square(pos, tank_width)
    if occupied.iter().all(fn(collision) { not(col.collide(collision)) }) {
      return Some(pos)
    }
  }
  None
}
