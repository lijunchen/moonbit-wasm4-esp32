# WASM-4 on ESP32

[WASM-4](https://github.com/aduros/wasm4) is a low-level fantasy game console designed for building small games with WebAssembly. It features a runtime implemented in C, utilizing the [wasm3](https://github.com/wasm3/wasm3) WebAssembly interpreter.

A straightforward idea is to run it on an actual MCU.

## Video Demo

https://github.com/user-attachments/assets/ae7df0f5-ed21-4b23-a0c1-739baad3ae31

## ESP32-C3 Port

The following are the hardware specifications of WASM-4:
> - Display: 160x160 pixels, 4 customizable colors, updated at 60 Hz.
> - Memory: 64 KB linear RAM, memory-mapped I/O, save states.
> - Cartridge Size Limit: 64 KB.
> - Input: Keyboard, mouse, touchscreen, up to 4 gamepads.
> - Audio: 2 pulse wave channels, 1 triangle wave channel, 1 noise channel.
> - Disk Storage: 1024 bytes.

WASM-4 simulates a machine with these specs. However, if we run it on an MCU, we can skip certain parts of the simulator. For instance, the display can be replaced with a real LCD, and the input can be handled by physical buttons.

I decide to port it to the ESP32-C3. It has a powerful CPU and sufficient ROM and RAM to run a WebAssembly interpreter. Additionally, it features a WiFi module, allowing us to upload games wirelessly without needing a USB cable or flashing the firmware. WiFi can also be used to control player input, eliminating the need for physical buttons.

The WASM-4 display is 160x160 pixels, but finding a 160x160 LCD proved difficult. So, I opted for a 1.54-inch 240x240 display. This screen leaves 40 pixels on the side unused, but that's acceptable. The screen is driven by the ST7789, and I’m using the [esp-idf-st7789](https://github.com/nopnop2002/esp-idf-st7789) driver to interface with it.

The plan is simple.

Before debugging on the ESP32, we need to debug on the native platform. WASM-4 is implemented for Linux, so it includes a GUI. The first step is to remove the GUI. WASM-4 has a buffer to store the screen, so we only need to remove all GUI-related code and retain the buffer. To debug this, we can save the buffer to an image file and verify its correctness. After that, we can proceed to debug it on the MCU.

Next, we move on to the hardware part. The first task is to power up the screen and display the buffer. After that, we can set up an HTTP server on the MCU to receive game data and player input. I plan to address the audio part last.

## Current Status

🚧 Still under construction. 
- [x] Removed unused dependencies of WASM-4
    - [x] GUI
    - [x] Audio
- [x] Redirected WASM-4 screen to a 2D array
- [x] Able to run tinypong.wasm
- [ ] Player controls via HTTP
- [ ] Hot-load custom games via HTTP
- [ ] Audio support

## Other
- GPIO should be configured according to your specific ESP32 board.
