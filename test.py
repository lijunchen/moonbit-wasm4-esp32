import serial
import ctypes
import argparse
import os

# Setup argparse to read command-line arguments
parser = argparse.ArgumentParser(
    description="Transmit a specified .wasm file over serial."
)
parser.add_argument(
    "wasm",
    choices=["fast-snake", "slow-snake", "tankle"],
    help="Choose which .wasm file to transmit",
)
args = parser.parse_args()

# Map each choice to its file path
wasm_paths = {
    "fast-snake": "./fast-snake/target/wasm/release/build/snake.wasm",
    "slow-snake": "./snake-mbt/target/wasm/release/build/snake.wasm",
    "tankle": "./tankle-mbt/target/wasm/release/build/tankle.wasm",
}

# Select the path based on the chosen option
wasm_path = wasm_paths[args.wasm]

# Check if the file exists
if not os.path.isfile(wasm_path):
    print(f"Error: File {wasm_path} not found.")
    exit(1)

port = "/dev/cu.wchusbserial58A60725371"
# Open serial port
ser = serial.Serial(
    port=port,
    baudrate=115200,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    timeout=1,
)

if ser.is_open:
    print("Serial port is open.")

# Read the chosen .wasm file
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

# Calculate checksum
checksum = ctypes.c_int32(0)
for byte in payload:
    checksum = ctypes.c_int32(checksum.value + byte)
print("Checksum:", checksum.value)

# Send data over serial
ser.write(data)
