enum Bonus {
  None
  Armor(Int)
  Traj(Int)
  Cd(Int)
  Speed(Int)
  // Refill
}

let armor_bonus : TankModel = {
  power: 1,
  speed: 1,
  cd: 60,
  traj: 1,
  sprite: armor_sprite,
  controller: stand_controller,
  max_health: 1,
  bonus: Armor(2),
}

let traj_bonus : TankModel = {
  power: 1,
  speed: 1,
  cd: 60,
  traj: 1,
  sprite: traj_sprite,
  controller: stand_controller,
  max_health: 1,
  bonus: Traj(2),
}

let cd_bonus : TankModel = {
  power: 1,
  speed: 1,
  cd: 60,
  traj: 1,
  sprite: cd_sprite,
  controller: stand_controller,
  max_health: 1,
  bonus: Cd(3),
}

let speed_bonus : TankModel = {
  power: 1,
  speed: 1,
  cd: 60,
  traj: 1,
  sprite: speed_sprite,
  controller: stand_controller,
  max_health: 1,
  bonus: Speed(1),
}

let armor_max = 12

let traj_max = 10

let cd_min = 5

let speed_max = 4

fn make_bonus() -> Array[Tank] {
  [
    Tank::new({ x: 48, y: 60 }, Up, armor_bonus, Bonus),
    Tank::new({ x: 48, y: 84 }, Up, traj_bonus, Bonus),
    Tank::new({ x: 48, y: 108 }, Up, cd_bonus, Bonus),
    Tank::new({ x: 48, y: 132 }, Up, speed_bonus, Bonus),
  ]
}

fn Bonus::up(self : Bonus, tank : Tank) -> Unit {
  match self {
    None => ignore(None)
    Armor(i) =>
      if tank.model.max_health < armor_max {
        tank.model.max_health += i
      }
    Traj(i) => if tank.model.traj < traj_max { tank.model.traj += i }
    Cd(i) => if tank.model.cd > cd_min { tank.model.cd -= i }
    Speed(i) => if tank.model.speed < speed_max { tank.model.speed += i }
  }
  // Refill => tank.health = tank.model.max_health
  tank.health = tank.model.max_health
}
