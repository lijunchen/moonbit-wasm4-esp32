struct Collision {
  pos : Pos
  size : Pos
}

fn Collision::rect(pos : Pos, size : Pos) -> Collision {
  { pos, size }
}

fn Collision::square(pos : Pos, size : Int) -> Collision {
  Collision::rect(pos, { x: size, y: size })
}

fn Collision::collide(a : Collision, b : Collision) -> Bool {
  let a_min = a.pos
  let a_max = a.pos + a.size
  let b_min = b.pos
  let b_max = b.pos + b.size
  a_min.x < b_max.x &&
  a_max.x > b_min.x &&
  a_min.y < b_max.y &&
  a_max.y > b_min.y
}

fn move_with_collide(
  object : Collision,
  velocity : Pos,
  collisions : Array[Collision]
) -> (Pos, Bool) {
  let pos = object.pos
  let size = object.size
  let new_pos = pos + velocity
  let ray_collision = make_ray_collision(object, velocity)
  let mut collide = false
  for collision in collisions {
    if Collision::collide(ray_collision, collision) {
      if velocity.x > 0 {
        new_pos.x = collision.pos.x - size.x
      } else if velocity.x < 0 {
        new_pos.x = collision.pos.x + collision.size.x
      }
      if velocity.y > 0 {
        new_pos.y = collision.pos.y - size.y
      } else if velocity.y < 0 {
        new_pos.y = collision.pos.y + collision.size.y
      }
      collide = true
    }
  }
  (new_pos, collide)
}

fn make_ray_collision(object : Collision, velocity : Pos) -> Collision {
  let x = if velocity.x >= 0 { object.pos.x } else { object.pos.x + velocity.x }
  let y = if velocity.y >= 0 { object.pos.y } else { object.pos.y + velocity.y }
  let pos = { x, y }
  let width = if velocity.x >= 0 {
    object.size.x + velocity.x
  } else {
    object.size.x - velocity.x
  }
  let height = if velocity.y >= 0 {
    object.size.y + velocity.y
  } else {
    object.size.y - velocity.y
  }
  let size = { x: width, y: height }
  Collision::rect(pos, size)
}

