fn control_system(entities : Entities) -> Unit {
  for tank in entities.tanks {
    (tank.model.controller._)(tank, entities)
  }
}

type Controller (Tank, Entities) -> Unit

let player_controller : Controller = fn(
    tank : Tank,
    entities : Entities
  ) -> Unit {
    let pad = @w4x.get_gamepad()
    let dir = if pad.button_right {
      Some(Right)
    } else if pad.button_left {
      Some(Left)
    } else if pad.button_up {
      Some(Up)
    } else if pad.button_down {
      Some(Down)
    } else {
      None
    }
    tank.rotate(dir)
    if pad.button_1 {
      tank.shoot().map(fn(bullet) { entities.bullets.push(bullet) }) |> ignore
    }
  }
  |> Controller

let stand_controller : Controller = fn(
    _tank : Tank,
    _entities : Entities
  ) -> Unit {

  }
  |> Controller

let zombie_controller : Controller = fn(
    tank : Tank,
    entities : Entities
  ) -> Unit {
    let rate = entities.random_state.int(limit=400)
    if rate < 25 {
      let dir = if rate < 5 {
        Some(Up)
      } else if rate < 10 {
        Some(Down)
      } else if rate < 15 {
        Some(Left)
      } else if rate < 20 {
        Some(Right)
      } else {
        None
      }
      tank.rotate(dir)
    }
    tank.shoot().map(fn(bullet) { entities.bullets.push(bullet) }) |> ignore
  }
  |> Controller