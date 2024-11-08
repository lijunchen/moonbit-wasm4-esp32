cmake_minimum_required(VERSION 3.10)
project(MyGLFWApp C)

# Specify C standard (optional)
set(CMAKE_C_STANDARD 11)

# Add GLFW as a subdirectory
add_subdirectory(vendor/glfw)

# Add your executable
add_executable(myapp src/main.c)

# Link GLFW and necessary macOS frameworks
target_link_libraries(myapp PRIVATE glfw)

# macOS specific: Link required system frameworks
if (APPLE)
    target_link_libraries(myapp PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit" "-framework CoreVideo")
endif()