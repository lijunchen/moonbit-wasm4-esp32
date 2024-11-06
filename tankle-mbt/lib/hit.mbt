fn hit_system(entities : Entities) -> Unit {
  for bullet in entities.bullets {
    for tank in entities.tanks {
      if tank.team != bullet.team && collide(bullet.collision(), tank.collision()) {
        bullet.die()
        tank.hurt(bullet.power)
      }
    }
  }
}
