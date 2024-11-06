fn w4panic[T](msg : String) -> T {
  @wasm4.trace(msg)
  panic()
}

fn w4expect[T](opt : Option[T], msg : String) -> T {
  match opt {
    Some(v) => v 
    None => w4panic(msg)
  }
}
