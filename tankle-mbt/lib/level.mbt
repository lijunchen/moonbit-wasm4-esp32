pub struct Level {
  enemies : MultiQueue[TankModel]
  spawn_cd : Int
  mut time_count : Int
}

fn make_levels() -> Array[Level] {
  [
    Level::new([(greenQ_model, 6), (wolf_model, 4)], 60),
    Level::new([(greenQ_model, 4), (wolf_model, 4), (greenW_model, 4)], 60),
    Level::new([(greenQ_model, 20)], 20),
    Level::new([(greenW_model, 4), (superwolf_model, 4), (rhino_model, 4)], 40),
    Level::new([(superrhino_model, 5), (superwolf_model, 5), (superrhino_model, 5)], 60),
    Level::new([(blackpig_model, 1), (superwolf_model, 10)], 4),
    Level::new([(redpig_model, 4)], 10)
  ]
}

fn Level::new(enemies : Array[(TankModel, UInt)], spawn_cd : Int) -> Level {
  { enemies: MultiQueue::new(enemies), spawn_cd, time_count: 0 }
}

fn Level::no_enemy(self : Level) -> Bool {
  self.enemies.queue.retain(fn(node) { node.num > 0 })
  self.enemies.queue.is_empty()
}
