set(idf_ver "${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}")


set(APP_SOURCES
    "main.c"
    "display.c"
    "framebuffer.c"
    "runtime.c"
    "strnlen.c"
    "tinypong.c"
    "util.c"
   
    "protocol_examples_utils.c"
    "wifi.c"
    "http.c"
    "control.c"
    "wamr.c"
    )

idf_component_register(SRCS ${APP_SOURCES}
                       INCLUDE_DIRS ""
                       INCLUDE_DIRS ".")


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
