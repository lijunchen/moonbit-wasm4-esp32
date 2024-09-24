set(idf_ver "${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}")

if (NOT CMAKE_BUILD_EARLY_EXPANSION)
    idf_build_get_property(build_dir BUILD_DIR)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/wasm3/source ${build_dir}/m3)
endif()


set(APP_SOURCES
    "main.c"
    "display.c"
    "framebuffer.c"
    "runtime.c"
    "strnlen.c"
    "tinypong.c"
    "util.c"
    "wasm_wasm3.c"
    "protocol_examples_utils.c"
    "wifi.c"
    "http.c"
    "control.c"
    "wamr.c"
    "tankle.c"
    )

idf_component_register(SRCS ${APP_SOURCES}
                       INCLUDE_DIRS ""
                       LDFRAGMENTS linker.lf
                       INCLUDE_DIRS ".")

if (idf_ver STREQUAL "4.0")
    # IDF v4.0 links apps with -nostdlib, so need to explicitly list the dependencies.
    add_library(m3_deps INTERFACE)
    target_link_libraries(m3_deps INTERFACE c m gcc)
    target_link_libraries(${COMPONENT_TARGET} PRIVATE m3 m3_deps)
else()
    # For IDF v4.1 and later, no tricks required.
    target_link_libraries(${COMPONENT_TARGET} PRIVATE m3)
endif()

target_compile_options(m3 PUBLIC -DM3_IN_IRAM -DESP32 -O3 -freorder-blocks)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-pointer-sign
    -Wno-error=format
    -Wno-error=return-type
    -Wno-error=unused-variable
    -Wno-error=type-limits
    )

if(DEFINED ENV{ESP_WIFI_PASSWORD})
    add_definitions(-DESP_WIFI_PASSWORD="$ENV{ESP_WIFI_PASSWORD}")
else()
    message(FATAL_ERROR "Environment variable ESP_WIFI_PASSWORD is not set")
endif()

if(DEFINED ENV{ESP_WIFI_SSID})
    add_definitions(-DESP_WIFI_SSID="$ENV{ESP_WIFI_SSID}")
else()
    message(FATAL_ERROR "Environment variable ESP_WIFI_SSID is not set")
endif()
