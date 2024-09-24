# MoonBit WASM-4 on ESP32

```
idf.py set-target esp32c6
moon build -C ./snake-mbt --target wasm
xxd -n __game_card -i ./snake-mbt/target/wasm/release/build/snake.wasm > ./main/gamecard.c
idf.py flash
```
