fn heat_system(entities : Entities) -> Unit {
  for tank in entities.tanks {
    if tank.canon_heat > 0 {
      tank.canon_heat -= 1
    }
  }
}
