#include <GLFW/glfw3.h>
#include <stdio.h>

#include "control.h"
#include "runtime.h"
#include "wamr.h"

#include <sys/time.h>

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

#define WINDOW_WIDTH 160
#define WINDOW_HEIGHT 160

extern uint32_t pixels[160 * 160];

char global_heap_buffer[384 * 1024];

int main(void) {
  // Initialize GLFW
  if (!glfwInit()) {
    return -1;
  }

  // Create a windowed mode window and its OpenGL context
  GLFWwindow* window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Draw Pixels", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Set up an orthographic projection with (0, 0) at the top-left
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1,
          1); // Notice the flipped y-axis

  // Switch back to the model view matrix
  glMatrixMode(GL_MODELVIEW);

  // Set up OpenGL texture
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  init_wamr();

  w4_Disk disk = {0};
  uint8_t* memory = wamr_get_phy_memory();
  w4_runtimeInit(memory, &disk);

  while (true) {
    double timeStart = glfwGetTime();
    double timeEnd = timeStart + 1.0 / 60.0;
    int state = 0;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
      state |= W4_BUTTON_X;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
      state |= W4_BUTTON_Z;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      state |= W4_BUTTON_UP;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      state |= W4_BUTTON_DOWN;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      state |= W4_BUTTON_LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      state |= W4_BUTTON_RIGHT;
    }
    w4_runtimeSetGamepad(0, state);

    struct timeval start, end;
    gettimeofday(&start, NULL);
    w4_runtimeUpdate();
    gettimeofday(&end, NULL);
    double elapsed_time =
        (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
    printf("w4_runtimeUpdate time taken: %f us\n", elapsed_time);
    // Prepare the texture from the framebuffer data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Render the texture on a quad that fills the window
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(WINDOW_WIDTH, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
    double timeRemaining;
    while ((timeRemaining = timeEnd - glfwGetTime()) > 0) {
      glfwWaitEventsTimeout(timeRemaining);
    }
  }

  // Clean up and exit
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}