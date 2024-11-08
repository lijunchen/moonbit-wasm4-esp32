cmake_minimum_required(VERSION 3.10)
project(MyGLFWApp C)

set(CMAKE_BUILD_TYPE Debug)

# Specify C standard
set(CMAKE_C_STANDARD 11)

# Set WAMR build options
set(WAMR_BUILD_PLATFORM "darwin")
set(WAMR_BUILD_TARGET "AARCH64")
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_FAST_INTERP 1)
set(WAMR_BUILD_AOT 1)
set(WAMR_BUILD_LIBC_BUILTIN 1)
set(WAMR_BUILD_LIBC_WASI 1)
set(WAMR_BUILD_SIMD 1)
set(WAMR_ROOT_DIR "${CMAKE_SOURCE_DIR}/vendor/wasm-micro-runtime")

# Include WAMR's CMake setup
include(${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})


# Add GLFW as a subdirectory
add_subdirectory(vendor/glfw)

# Add your executable and source files
add_executable(myapp
    src/control.c
    src/display.c
    src/framebuffer.c
    src/gamecard.c
    src/main.c
    src/runtime.c
    src/util.c
    src/wamr.c
)

# Link WAMR libraries and GLFW
target_link_libraries(myapp PRIVATE vmlib glfw)

# macOS specific: Link required system frameworks
if (APPLE)
    target_link_libraries(myapp PRIVATE "-framework Cocoa" "-framework OpenGL" "-framework IOKit" "-framework CoreVideo")
endif()