#include <GLFW/glfw3.h>
#include <stdio.h>

// Vertex data for a simple triangle
const float vertices[] = {
    0.0f, 0.5f, 0.0f,   // Top vertex
    -0.5f, -0.5f, 0.0f, // Bottom-left vertex
    0.5f, -0.5f, 0.0f   // Bottom-right vertex
};

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(640, 480, "Display Triangle with GLFW", NULL, NULL);
    if (!window)
    {
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

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Set up basic OpenGL to draw the triangle
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Disable client state
        glDisableClientState(GL_VERTEX_ARRAY);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}