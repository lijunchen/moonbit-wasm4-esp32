let border_left = 0

let border_right = 120

let border_top = 16

let border_bottom = 160

let border_collisions : Array[Collision] = [
  Collision::rect({ x: border_left, y: border_top - 1 }, { x: border_right - border_left, y: 1 }),
  Collision::rect({ x: border_left, y: border_bottom }, { x: border_right - border_left, y: 1 }),
  Collision::rect({ x: border_left - 1, y: border_top }, { x: 1, y: border_bottom - border_top }),
  Collision::rect({ x: border_right, y: border_top }, { x: 1, y: border_bottom - border_top }),
]