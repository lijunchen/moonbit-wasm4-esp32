fn life_system(entities : Entities) -> Unit {
  for tank in entities.tanks {
    if not(tank.is_live()) && tank.team == Bonus {
      let player = entities.find_player() |> w4expect("Impossible")
      tank.model.bonus.up(player)
    }
  }
  entities.tanks.retain(fn(t) { t.is_live() })
  entities.bullets.retain(fn(b) { b.is_live() })
}
