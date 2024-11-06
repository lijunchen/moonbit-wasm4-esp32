struct Tank {
  mut live : Bool
  mut pos : Pos
  mut direction : Direction
  mut velocity : Pos
  mut canon_heat : Int
  mut health : Int
  model : TankModel
  team : Team
}

enum Direction {
  Up
  Down
  Left
  Right
} derive(Eq)

enum Team {
  Enemy
  Player
  Bonus
} derive(Eq)

struct TankModel {
  power : Int
  mut speed : Int
  mut cd : Int
  mut traj : Int
  sprite : Sprite
  controller : Controller
  mut max_health : Int
  bonus : Bonus
}

impl Clone for TankModel with clone(self : TankModel) -> TankModel {
  {
    power: self.power,
    speed: self.speed,
    cd: self.cd,
    traj: self.traj,
    sprite: self.sprite,
    controller: self.controller,
    max_health: self.max_health,
    bonus: None,
  }
}

let player_model : TankModel = {
  power: 1,
  speed: 2,
  cd: 20,
  traj: 4,
  sprite: player_sprite,
  controller: player_controller,
  max_health: 5,
  bonus: None,
}

let greenQ_model : TankModel = {
  power: 1,
  speed: 1,
  cd: 40,
  traj: 3,
  sprite: greenQ_sprite,
  controller: zombie_controller,
  max_health: 1,
  bonus: None,
}

let greenW_model : TankModel = {
  power: 1,
  speed: 1,
  cd: 25,
  traj: 6,
  sprite: greenQ_sprite,
  controller: zombie_controller,
  max_health: 1,
  bonus: None,
}

let wolf_model : TankModel = {
  power: 1,
  speed: 2,
  cd: 40,
  traj: 3,
  sprite: wolf_sprite,
  controller: zombie_controller,
  max_health: 1,
  bonus: None,
}

let superwolf_model : TankModel = {
  power: 1,
  speed: 4,
  cd: 30,
  traj: 5,
  sprite: wolf_sprite,
  controller: zombie_controller,
  max_health: 1,
  bonus: None,
}

let rhino_model : TankModel = {
  power: 1,
  speed: 1,
  cd: 40,
  traj: 3,
  sprite: rhino_sprite,
  controller: zombie_controller,
  max_health: 3,
  bonus: None,
}

let superrhino_model : TankModel = {
  power: 2,
  speed: 1,
  cd: 40,
  traj: 4,
  sprite: rhino_sprite,
  controller: zombie_controller,
  max_health: 4,
  bonus: None,
}

let blackpig_model : TankModel = {
  power: 2,
  speed: 1,
  cd: 60,
  traj: 6,
  sprite: blackpig_sprite,
  controller: zombie_controller,
  max_health: 10,
  bonus: None,
}

let redpig_model : TankModel = {
  power: 2,
  speed: 1,
  cd: 20,
  traj: 8,
  sprite: blackpig_sprite,
  controller: zombie_controller,
  max_health: 10,
  bonus: None,
}

fn Tank::new(
  position : Pos,
  direction : Direction,
  model : TankModel,
  team : Team
) -> Tank {
  {
    live: true,
    pos: position,
    direction,
    velocity: { x: 0, y: 0 },
    canon_heat: 0,
    model,
    team,
    health: model.max_health,
  }
}

fn Tank::draw(self : Tank) -> Unit {
  self.model.sprite.blit(self.pos.x, self.pos.y, self.direction)
}

fn Tank::rotate(self : Tank, dir : Direction?) -> Unit {
  match dir {
    Some(dir) => {
      self.direction = dir
      self.velocity = vectorize(self.model.speed, dir)
    }
    None => self.velocity = { x: 0, y: 0 }
  }
}

fn Tank::shoot(self : Tank) -> Bullet? {
  if self.canon_heat == 0 {
    let center = {
      x: self.pos.x + tank_width / 2 - bullet_width / 2,
      y: self.pos.y + tank_width / 2 - bullet_width / 2,
    }
    let vec = vectorize(tank_width / 2 + bullet_width / 2, self.direction)
    let bullet_pos = center + vec
    self.canon_heat += self.model.cd
    shoot_sound()
    Some(
      Bullet::new(
        bullet_pos,
        self.direction,
        self.model.traj,
        self.model.power,
        self.team,
      ),
    )
  } else {
    None
  }
}

fn Tank::collision(self : Tank) -> Collision {
  Collision::square(self.pos, self.model.sprite.width)
}

fn Tank::is_live(self : Tank) -> Bool {
  self.live
}

fn Tank::hurt(self : Tank, damage : Int) -> Unit {
  self.health -= damage
  if self.health <= 0 {
    self.live = false
    explode_sound()
  }
}
