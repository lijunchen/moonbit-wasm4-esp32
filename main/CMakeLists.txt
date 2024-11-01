set(idf_ver "${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}")


set(APP_SOURCES
    "main.c"
    "display.c"
    "framebuffer.c"
    "runtime.c"
    "strnlen.c"
    "gamecard.c"
    "util.c"
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
