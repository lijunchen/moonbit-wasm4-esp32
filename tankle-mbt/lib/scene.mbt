enum Scene {
  Welcome
  Game
  Victory
  Defeat
  Bonus
} derive(Eq)

fn render_system(entities : Entities) -> Unit {
  match entities.scene {
    Welcome => render_welcome(entities)
    Game => render_game(entities)
    Victory => render_victory(entities)
    Defeat => render_defeat(entities)
    Bonus => render_bonus(entities)
  }
}

fn render_welcome(_entities : Entities) -> Unit {
  @wasm4.set_draw_colors(2, index=1)
  @wasm4.set_draw_colors(2, index=2)
  @wasm4.rect(0, 0, 160, 160)

  @wasm4.set_draw_colors(3)
  @wasm4.text("TANKLE", 30, 10)

  @wasm4.text("Press x to Start", 20, 40)
}

fn render_game(entities : Entities) -> Unit {
  render_game_panel(entities)
  @wasm4.set_draw_colors(1, index=1)
  @wasm4.set_draw_colors(1, index=2)
  @wasm4.rect(0, 16, 120, 144)
  entities.tanks.each(Tank::draw)
  entities.bullets.each(Bullet::draw)
}

fn life_sripe(maxhp : Int, life : Int) -> Unit {
  @wasm4.set_draw_colors(3, index=1)
  @wasm4.set_draw_colors(2, index=2)
  @wasm4.text("HP", 6, 4)
  let length = maxhp * 8 + 4
  @wasm4.set_draw_colors(3, index=1)
  @wasm4.set_draw_colors(3, index=2)
  @wasm4.rect(26, 3, length, 10)
  let length = (maxhp - life) * 8
  if length > 0 {
    @wasm4.set_draw_colors(2, index=1)
    @wasm4.set_draw_colors(2, index=2)
    @wasm4.rect(28 + life * 8, 5, length, 6)
  }
}

fn render_game_panel(entities : Entities) -> Unit {
  @wasm4.set_draw_colors(2, index=1)
  @wasm4.set_draw_colors(2, index=2)
  @wasm4.rect(120, 0, 40, 160)
  @wasm4.rect(0, 0, 120, 16)
  let player_tank = entities
    .find_player()
    .or(Tank::new({ x: 0, y: 0 }, Up, player_model, Player))
  life_sripe(player_tank.model.max_health, player_tank.health)
  @wasm4.set_draw_colors(3, index=1)
  @wasm4.set_draw_colors(2, index=2)
  @wasm4.text("SPEED", 122, 16)
  let speed = player_tank.model.speed
  @wasm4.text("\{speed}", 130, 26)
  @wasm4.text("TRAJ", 122, 56)
  let traj = player_tank.model.traj
  @wasm4.text("\{traj}", 130, 66)
  @wasm4.text("CD", 122, 96)
  let cd = player_tank.model.cd
  @wasm4.text("\{cd}", 130, 106)
  @wasm4.text("LV", 122, 136)
  let lv = entities.level_idx + 1
  @wasm4.text("\{lv}", 130, 146)
}

fn render_bonus(entities : Entities) -> Unit {
  render_game(entities)
}

fn render_victory(_entities : Entities) -> Unit {
  @wasm4.set_draw_colors(2, index=1)
  @wasm4.set_draw_colors(2, index=2)
  @wasm4.rect(0, 0, 160, 160)
  @wasm4.set_draw_colors(3, index=1)
  @wasm4.set_draw_colors(2, index=2)
  @wasm4.text("Victory", 30, 10)
}

fn render_defeat(_entities : Entities) -> Unit {
  @wasm4.set_draw_colors(2, index=1)
  @wasm4.set_draw_colors(2, index=2)
  @wasm4.rect(0, 0, 160, 160)
  @wasm4.set_draw_colors(3, index=1)
  @wasm4.set_draw_colors(2, index=2)
  @wasm4.text("Defeat", 30, 10)
}

fn switch_scene_system(entities : Entities) -> Unit {
  match entities.scene {
    Welcome => welcome_scene(entities)
    Game => game_scene(entities)
    Victory => victory_scene(entities)
    Defeat => defeat_scene(entities)
    Bonus => bonus_scene(entities)
  }
}

fn welcome_scene(entities : Entities) -> Unit {
  if entities.pressedpad.get_pressed_buttons().button_1 {
    entities.scene = Game
  }
}

fn game_scene(entities : Entities) -> Unit {
  let level = entities.levels.get(entities.level_idx) |> w4expect("Impossible")
  if entities.tanks.filter(fn(tank) { tank.team == Enemy }).is_empty() &&
    level.no_enemy() {
    if entities.levels.length() == entities.level_idx + 1 {
      entities.scene = Victory
      return
    }
    entities.tanks.push_iter(make_bonus().iter())
    entities.scene = Bonus
  }
  let player_tank = entities.find_player()
  if player_tank.is_empty() {
    entities.scene = Defeat
  }
}

fn bonus_scene(entities : Entities) -> Unit {
  if entities.tanks.filter(fn(tank) { tank.team == Bonus }).length() != 4 {
    entities.level_idx += 1
    entities.tanks.retain(fn(tank) { tank.team != Bonus })
    entities.scene = Game
  }
}

fn victory_scene(entities : Entities) -> Unit {
  if entities.pressedpad.get_pressed_buttons().button_1 {
    entities.reset()
    entities.scene = Welcome
  }
}

fn defeat_scene(entities : Entities) -> Unit {
  if entities.pressedpad.get_pressed_buttons().button_1 {
    entities.reset()
    entities.scene = Welcome
  }
}
