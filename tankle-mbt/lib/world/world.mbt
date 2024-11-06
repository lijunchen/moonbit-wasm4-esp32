struct System[E] {
  run : (E) -> Unit
  period : Int
  condition : RunCondition[E]
}

pub enum RunCondition[E] {
  Always
  If((E) -> Bool)
  Or(RunCondition[E], RunCondition[E])
  And(RunCondition[E], RunCondition[E])
}

pub fn RunCondition::or[E](
  a : RunCondition[E],
  b : RunCondition[E]
) -> RunCondition[E] {
  Or(a, b)
}

pub fn RunCondition::and[E](
  a : RunCondition[E],
  b : RunCondition[E]
) -> RunCondition[E] {
  And(a, b)
}

fn RunCondition::satisfy[E](self : RunCondition[E], entities : E) -> Bool {
  match self {
    Always => true
    If(f) => f(entities)
    Or(a, b) => a.satisfy(entities) || b.satisfy(entities)
    And(a, b) => a.satisfy(entities) && b.satisfy(entities)
  }
}

struct World[E] {
  entities : E
  systems : Array[System[E]]
  mut frame_counter : Int
}

pub fn World::default[E : Default]() -> World[E] {
  { entities: E::default(), systems: [], frame_counter: 0 }
}

pub fn World::add_system[E](
  self : World[E],
  system : (E) -> Unit,
  period : Int,
  ~condition : RunCondition[E] = Always
) -> Unit {
  self.systems.push({ run: system, period, condition })
}

pub fn World::update_frame[E](self : World[E]) -> Unit {
  self.frame_counter += 1
  for system in self.systems {
    if self.frame_counter % system.period == 0 &&
      system.condition.satisfy(self.entities) {
      (system.run)(self.entities)
    }
  }
}
