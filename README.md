# MoonBit WASM-4 on ESP32

![1727189835964](https://github.com/user-attachments/assets/7b271c2b-14e0-44ce-bc88-7dc4b07546a5)

GPIO should be configured according to your specific ESP32 board.

```
idf.py set-target esp32c6
moon build -C ./snake-mbt --target wasm
xxd -n __game_card -i ./snake-mbt/target/wasm/release/build/snake.wasm > ./main/gamecard.c
idf.py flash
```

```
moon build -C ./fast-snake --target wasm
xxd -n __game_card -i ./fast-snake/target/wasm/release/build/snake.wasm > ./main/gamecard.c
```