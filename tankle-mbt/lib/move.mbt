fn move_system(entities : Entities) -> Unit {
  for tank in entities.tanks {
    let collisions = entities.tanks
      .map(fn(t) { t.collision() })
      .filter(fn(c) { c.pos != tank.pos })
    collisions.append(border_collisions)
    tank.pos = move_with_collide(tank.collision(), tank.velocity, collisions).0
  }
  for bullet in entities.bullets {
    let velocity = vectorize(bullet.speed, bullet.direction)
    let (pos, collide) = move_with_collide(bullet.collision(), velocity, border_collisions)
    if collide {
      bullet.die()
    }
    bullet.pos = pos
  }
}
