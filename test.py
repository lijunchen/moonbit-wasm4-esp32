import serial
import ctypes

ser = serial.Serial(
    port="/dev/cu.wchusbserial58A60725371",
    baudrate=115200,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    timeout=1,
)

if ser.is_open:
    print("Serial port is open.")

fast_snake = "./fast-snake/target/wasm/release/build/snake.wasm"
slow_snake = "./snake-mbt/target/wasm/release/build/snake.wasm"
tankle = "tankle-mbt/target/wasm/release/build/tankle.wasm"

wasm_path = tankle

with open(wasm_path, "rb") as fp:
    payload = fp.read()


def to_bytes(value: int) -> bytes:
    return ctypes.c_int32(value).value.to_bytes(4, "little")


length = 4 + 4 + len(payload)
frame_type = 1234
data = to_bytes(length) + to_bytes(frame_type) + payload
print(data)
print(type(data))
print(len(data))
checksum = ctypes.c_int32(0)
for byte in payload:
    checksum = ctypes.c_int32(checksum.value + byte)
print("Checksum:", checksum.value)
ser.write(data)
