#include <GLFW/glfw3.h>
#include <stdio.h>

#include "runtime.h"
#include "wamr.h"

// Vertex data for a simple triangle
const float vertices[] = {
    0.0f,  0.5f,  0.0f, // Top vertex
    -0.5f, -0.5f, 0.0f, // Bottom-left vertex
    0.5f,  -0.5f, 0.0f  // Bottom-right vertex
};

GLFWwindow* window = NULL;

char card_data[48 * 1024];
int card_length = 0;
extern uint32_t stop;

extern void* wamr_get_phy_memory();

extern void w4_windowBoot();

void run_wasm4() {
  w4_Disk disk = {0};
  uint8_t* memory = wamr_get_phy_memory();
  w4_runtimeInit(memory, &disk);
  // w4_wasmLoadModule(__tinypong_wasm, len);
  w4_windowBoot();
}

int main(void) {
  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  // Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(160, 160, "Display Triangle with GLFW", NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to open GLFW window\n");
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Set the viewport size to match the window size
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  init_wamr();
  run_wasm4();

  // Clean up and exit
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}