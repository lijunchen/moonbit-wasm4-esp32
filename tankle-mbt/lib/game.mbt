struct Entities {
  tanks : Array[Tank]
  bullets : Array[Bullet]
  mut scene : Scene
  pressedpad : @w4x.PressedPad
  mut levels : Array[Level]
  mut level_idx : Int
  random_state : @random.Rand
}

fn default_player_tank() -> Tank {
  Tank::new({ x: 60, y: 60 }, Up, Clone::clone(player_model), Player)
}

fn Entities::default() -> Entities {
  {
    tanks: [default_player_tank()],
    bullets: [],
    scene: Welcome,
    pressedpad: @w4x.PressedPad::new(),
    levels: make_levels(),
    level_idx: 0,
    random_state: @random.new(),
  }
}

fn Entities::find_player(self : Entities) -> Tank? {
  self.tanks.filter(fn(tank) { tank.team == Player }).get(0)
}

fn Entities::reset(self : Entities) -> Unit {
  self.levels = make_levels()
  self.bullets.clear()
  self.tanks.clear()
  self.tanks.push(default_player_tank())
  self.level_idx = 0
}

let world : @world.World[Entities] = @world.World::default()

fn scene_is(scene : Scene) -> (Entities) -> Bool {
  fn(entities) { entities.scene == scene }
}

pub fn start() -> Unit {
  world.add_system(switch_scene_system, 1)
  world.add_system(spawn_enemy_system, 1, condition=If(scene_is(Game)))
  world.add_system(control_system, 2, condition=@world.If(scene_is(Game)).or(If(scene_is(Bonus))))
  world.add_system(move_system, 2, condition=@world.If(scene_is(Game)).or(If(scene_is(Bonus))))
  world.add_system(hit_system, 1, condition=@world.If(scene_is(Game)).or(If(scene_is(Bonus))))
  world.add_system(heat_system, 2, condition=@world.If(scene_is(Game)).or(If(scene_is(Bonus))))
  world.add_system(life_system, 1, condition=@world.If(scene_is(Game)).or(If(scene_is(Bonus))))
  world.add_system(render_system, 1)
}

pub fn update() -> Unit {
  world.update_frame()
}
