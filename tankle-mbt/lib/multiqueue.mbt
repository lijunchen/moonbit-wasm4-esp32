struct MultiNode[T] {
  data : T
  mut num : UInt
}

struct MultiQueue[T] {
  queue : Array[MultiNode[T]]
}

trait Clone {
  clone(Self) -> Self
}

fn MultiQueue::new[T : Clone](nodes : Array[(T, UInt)]) -> MultiQueue[T] {
  { queue: nodes.map(fn(node) { MultiNode::{ data: node.0, num: node.1 } }) }
}

fn MultiQueue::pop[T : Clone](self : MultiQueue[T]) -> T? {
  self.queue.retain(fn(node) { node.num > 0 })
  self.queue
  .get(0)
  .map(
    fn(node) {
      node.num -= 1
      node.data.clone()
    },
  )
}
