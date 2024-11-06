struct Pos {
  mut x : Int
  mut y : Int
} derive(Eq)

fn vectorize(scaler: Int, direction: Direction) -> Pos {
  match direction {
    Up => {
      x: 0,
      y: -scaler
    }
    Down => {
      x: 0,
      y: scaler
    }
    Left => {
      x: -scaler,
      y: 0
    }
    Right => {
      x: scaler,
      y: 0
    }
  }
}

fn Pos::op_add(pos: Pos, velocity: Pos) -> Pos {
  {
    x: pos.x + velocity.x,
    y: pos.y + velocity.y
  }
}
