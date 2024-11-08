import pygame
import sys
import serial
import ctypes

port = "/dev/cu.wchusbserial58A60725371"
ser = serial.Serial(
    port=port,
    baudrate=115200,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    timeout=1,
)


def to_bytes(value: int) -> bytes:
    return ctypes.c_int32(value).value.to_bytes(4, "little")


def send_command(frame_type: int, payload_value: int):
    frame_size = 12
    data = to_bytes(frame_size) + to_bytes(frame_type) + to_bytes(payload_value)
    print(f"Sending data: {data}")
    ser.write(data)


# Initialize pygame
pygame.init()

# Screen settings
width, height = 400, 300
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Configurable Gamepad Simulator")

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
BLUE = (0, 0, 255)
GRAY = (169, 169, 169)

# Button positions and sizes
arrow_size = 50
button_size = 40

# Rectangles for buttons
up_rect = pygame.Rect(150, 50, arrow_size, arrow_size)
down_rect = pygame.Rect(150, 150, arrow_size, arrow_size)
left_rect = pygame.Rect(100, 100, arrow_size, arrow_size)
right_rect = pygame.Rect(200, 100, arrow_size, arrow_size)
x_button_rect = pygame.Rect(300, 80, button_size, button_size)
y_button_rect = pygame.Rect(300, 140, button_size, button_size)

# Fonts
font = pygame.font.SysFont("PingFang", 30)

# Configurable key bindings
key_bindings = {
    "UP": pygame.K_UP,
    "DOWN": pygame.K_DOWN,
    "LEFT": pygame.K_LEFT,
    "RIGHT": pygame.K_RIGHT,
    "X": pygame.K_LSHIFT,
    "Y": pygame.K_LCTRL,
}


# Function to draw buttons
def draw_buttons():
    # Arrow buttons
    command = 0

    pygame.draw.rect(screen, RED if keys[key_bindings["UP"]] else GRAY, up_rect)
    if keys[key_bindings["UP"]]:
        command = command | 64
    pygame.draw.rect(screen, RED if keys[key_bindings["DOWN"]] else GRAY, down_rect)
    if keys[key_bindings["DOWN"]]:
        command = command | 128

    pygame.draw.rect(screen, RED if keys[key_bindings["LEFT"]] else GRAY, left_rect)
    if keys[key_bindings["LEFT"]]:
        command = command | 16
    pygame.draw.rect(screen, RED if keys[key_bindings["RIGHT"]] else GRAY, right_rect)
    if keys[key_bindings["RIGHT"]]:
        command = command | 32

    # X and Y buttons
    pygame.draw.rect(screen, BLUE if keys[key_bindings["X"]] else GRAY, x_button_rect)
    if keys[key_bindings["X"]]:
        command = command | 1
    pygame.draw.rect(screen, BLUE if keys[key_bindings["Y"]] else GRAY, y_button_rect)
    if keys[key_bindings["Y"]]:
        command = command | 2

    if command != 0:
        send_command(0x1122, command)

    # Text on buttons
    screen.blit(font.render("上", True, WHITE), (up_rect.x + 15, up_rect.y + 10))
    screen.blit(font.render("下", True, WHITE), (down_rect.x + 15, down_rect.y + 10))
    screen.blit(font.render("左", True, WHITE), (left_rect.x + 10, left_rect.y + 10))
    screen.blit(font.render("右", True, WHITE), (right_rect.x + 10, right_rect.y + 10))
    screen.blit(
        font.render("X", True, WHITE), (x_button_rect.x + 10, x_button_rect.y + 5)
    )
    screen.blit(
        font.render("Y", True, WHITE), (y_button_rect.x + 10, y_button_rect.y + 5)
    )


# Main loop
keys = {}
while True:
    screen.fill(BLACK)
    keys = pygame.key.get_pressed()

    # Draw buttons
    draw_buttons()

    # Event handling
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    pygame.display.flip()
