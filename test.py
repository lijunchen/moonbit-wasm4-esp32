import serial
import ctypes

#   uart_config_t uart_cfg = {.baud_rate = 115200,
#                             .data_bits = UART_DATA_8_BITS,
#                             .parity = UART_PARITY_ODD,
#                             .stop_bits = UART_STOP_BITS_1,
#                             .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
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

# struct Frame {
#     length: u32,
#     type: u32,
#     data: [u8; length],
# }

fast_snake = "fast-snake"
slow_snake = "snake-mbt"

snake = fast_snake

with open(f"./{snake}/target/wasm/release/build/snake.wasm", "rb") as fp:
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
