fn shoot_sound() -> Unit {
  @wasm4.tone(
    (860, 150),
    @wasm4.ADSR::new(0, release=0, decay=6, attack=5),
    @wasm4.ADSRVolume::new(100),
    @wasm4.ToneFlag::new(
      channel=@wasm4.ToneChannel::Pulse1,
      mode=@wasm4.ToneMode::Duty_1_8,
    ),
  )
}

fn explode_sound() -> Unit {
    @wasm4.tone(
    (150, 240),
    @wasm4.ADSR::new(0, release=0, decay=15, attack=20),
    @wasm4.ADSRVolume::new(100),
    @wasm4.ToneFlag::new(
      channel=@wasm4.ToneChannel::Noise,
    ),
  )
}