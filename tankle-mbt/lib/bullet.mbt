struct Bullet {
  mut live : Bool
  mut pos : Pos
  direction : Direction
  speed : Int
  power : Int
  team : Team
  sprite : Sprite
}

fn Bullet::new(
  pos : Pos,
  dir : Direction,
  speed : Int,
  damage : Int,
  team : Team
) -> Bullet {
  {
    live: true,
    pos,
    direction: dir,
    speed,
    power: damage,
    team,
    sprite: bullet_sprite,
  }
}

fn Bullet::collision(self : Bullet) -> Collision {
  Collision::square(self.pos, self.sprite.width)
}

fn Bullet::draw(self : Bullet) -> Unit {
  self.sprite.blit(self.pos.x, self.pos.y, self.direction)
}

fn Bullet::is_live(self : Bullet) -> Bool {
  self.live
}

fn Bullet::die(self : Bullet) -> Unit {
  self.live = false
}
