#include "wamr.h"
#include "runtime.h"

void wrap_w4_runtimeBlit(wasm_exec_env_t exec_env, const uint8_t* sprite, int x, int y, int width, int height, int flags) {
    w4_runtimeBlit(sprite, x, y, width, height, flags);
}
void wrap_w4_runtimeBlitSub(wasm_exec_env_t exec_env, const uint8_t* sprite, int x, int y, int width, int height, int srcX, int srcY, int stride, int flags) {
    w4_runtimeBlitSub(sprite, x, y, width, height, srcX, srcY, stride, flags);
}
void wrap_w4_runtimeLine(wasm_exec_env_t exec_env, int x1, int y1, int x2, int y2) {
    w4_runtimeLine(x1, y1, x2, y2);
}
void wrap_w4_runtimeHLine(wasm_exec_env_t exec_env, int x, int y, int len) {
    w4_runtimeHLine(x, y, len);
}
void wrap_w4_runtimeVLine(wasm_exec_env_t exec_env, int x, int y, int len) {
    w4_runtimeVLine(x, y, len);
}
void wrap_w4_runtimeOval(wasm_exec_env_t exec_env, int x, int y, int width, int height) {
    w4_runtimeOval(x, y, width, height);
}
void wrap_w4_runtimeRect(wasm_exec_env_t exec_env, int x, int y, int width, int height) {
    w4_runtimeRect(x, y, width, height);
}
void wrap_w4_runtimeText(wasm_exec_env_t exec_env, const uint8_t* str, int x, int y) {
    w4_runtimeText(str, x, y);
}
void wrap_w4_runtimeTextUtf8(wasm_exec_env_t exec_env, const uint8_t* str, int byteLength, int x, int y) {
    w4_runtimeTextUtf8(str, byteLength, x, y);
}
void wrap_w4_runtimeTextUtf16(wasm_exec_env_t exec_env, const uint16_t* str, int byteLength, int x, int y) {
    w4_runtimeTextUtf16(str, byteLength, x, y);
}
void wrap_w4_runtimeTone(wasm_exec_env_t exec_env, int frequency, int duration, int volume, int flags) {
    w4_runtimeTone(frequency, duration, volume, flags);
}
int wrap_w4_runtimeDiskr(wasm_exec_env_t exec_env, uint8_t* dest, int size) {
    return w4_runtimeDiskr(dest, size);
}
int wrap_w4_runtimeDiskw(wasm_exec_env_t exec_env, const uint8_t* src, int size) {
    return w4_runtimeDiskw(src, size);
}
void wrap_w4_runtimeTrace(wasm_exec_env_t exec_env, const uint8_t* str) {
    w4_runtimeTrace(str);
}
void wrap_w4_runtimeTraceUtf8(wasm_exec_env_t exec_env, const uint8_t* str, int byteLength) {
    w4_runtimeTraceUtf8(str, byteLength);
}
void wrap_w4_runtimeTraceUtf16(wasm_exec_env_t exec_env, const uint16_t* str, int byteLength) {
    w4_runtimeTraceUtf16(str, byteLength);
}
void wrap_w4_runtimeTracef(wasm_exec_env_t exec_env, const uint8_t* str, const void* stack) {
    w4_runtimeTracef(str, stack);
}

void just_print_int(wasm_exec_env_t exec_env, int a) {
    printf("JUST PRINT INT %d\n", a);
}

static NativeSymbol native_symbols[] =
{
    {
        "just_print_int",
        just_print_int,
        "(i)"
    },
    {
        "blit",
     	wrap_w4_runtimeBlit,
        "(*iiiii)"
    },
    {
        "blitSub",
     	wrap_w4_runtimeBlitSub,
        "(*iiiiiiii)"
    },
    {
        "line",
     	wrap_w4_runtimeLine,
        "(iiii)"
    },
    {
        "hline",
     	wrap_w4_runtimeHLine,
        "(iii)"
    },
    {
        "vline",
     	wrap_w4_runtimeVLine,
        "(iii)"
    },
    {
        "oval",
     	wrap_w4_runtimeOval,
        "(iiii)"
    },
    {
        "rect",
     	wrap_w4_runtimeRect,
        "(iiii)"
    },
    {
        "text",
     	wrap_w4_runtimeText,
        "(*ii)"
    },

    {
        "textUtf8",
     	wrap_w4_runtimeTextUtf8,
        "(*~ii)"
    },
    {
        "textUtf16",
     	wrap_w4_runtimeTextUtf16,
        "(*~ii)"
    },

    {
        "tone",
     	wrap_w4_runtimeTone,
        "(iiii)"
    },

    {
        "diskr",
     	wrap_w4_runtimeDiskr,
        "(*~)i"
    },
    {
        "diskw",
     	wrap_w4_runtimeDiskw,
        "(*~)i"
    },

    {
        "trace",
     	wrap_w4_runtimeTrace,
        "(*)"
    },

    {
        "traceUtf8",
     	wrap_w4_runtimeTraceUtf8,
        "(*~)"
    },
    {
        "traceUtf16",
     	wrap_w4_runtimeTraceUtf16,
        "(*~)"
    },
    {
        "tracef",
     	wrap_w4_runtimeTracef,
        "($*)"
    },
};

extern unsigned char __tinypong_wasm[];
extern unsigned int __tinypong_wasm_len;

void init_wamr() {
    /* Setup variables for instantiating and running the wasm module */
    wasm_module_t wasm_module = NULL;
    wasm_module_inst_t wasm_module_inst = NULL;
    char error_buf[128];
    RuntimeInitArgs init_args;

    /* Configure memory allocation */
    memset(&init_args, 0, sizeof(RuntimeInitArgs));
    init_args.mem_alloc_type = Alloc_With_Allocator;
    init_args.mem_alloc_option.allocator.malloc_func = (void *)os_malloc;
    init_args.mem_alloc_option.allocator.realloc_func = (void *)os_realloc;
    init_args.mem_alloc_option.allocator.free_func = (void *)os_free;

    init_args.native_module_name = "env";
    init_args.native_symbols = native_symbols;
    init_args.n_native_symbols = sizeof(native_symbols) / sizeof(NativeSymbol);

    heap_caps_print_heap_info(MALLOC_CAP_8BIT);

    printf("Initialize WASM runtime\n");
    /* Initialize runtime environment */
    if (!wasm_runtime_full_init(&init_args)) {
        printf("Init runtime failed.\n");
        return;
    }

    printf("Load the wasm module from memory\n");
    printf("tinywasm: %p, len: %d\n", __tinypong_wasm, __tinypong_wasm_len);
    int count = 0;
    // for (int i = 0; i < __tinypong_wasm_len; i++) {
    //     count += __tinypong_wasm[i];
    // }
    // printf("count: %d\n", count);
    // count = 0;
    // for (int i = 0; i < __tinypong_wasm_len; i++) {
    //     __tinypong_wasm[i] = __tinypong_wasm[i];
    //     count += __tinypong_wasm[i];
    // }
    printf("count: %d\n", count);
    printf("memory test done\n");
    heap_caps_print_heap_info(MALLOC_CAP_8BIT);
    wasm_module = wasm_runtime_load(__tinypong_wasm, __tinypong_wasm_len, error_buf, sizeof(error_buf));
    if (!wasm_module) {
        printf("Failed to load wasm module: %s\n", error_buf);
        return;
    }

    printf("Instantiate the wasm module\n");
    wasm_module_inst = wasm_runtime_instantiate(wasm_module, 16 * 1024, 16 * 1024, error_buf, sizeof(error_buf));
    if (!wasm_module_inst) {
        printf("Failed to instantiate wasm module: %s\n", error_buf);
        wasm_runtime_unload(wasm_module);
        return;
    }


    printf("Call the wasm function if needed\n");
    wasm_function_inst_t start = wasm_runtime_lookup_function(wasm_module_inst, "start");
    printf("start: %p\n", start);
    wasm_function_inst_t update = wasm_runtime_lookup_function(wasm_module_inst, "update");
    printf("update: %p\n", update);
    wasm_exec_env_t exec_env = wasm_runtime_create_exec_env(wasm_module_inst, 2 * 1024);
    int ret = wasm_runtime_call_wasm(exec_env, start, 0, NULL);
    if (!ret) {
        printf("Failed to call wasm function [start].\n");
    }

    /* Clean up */
    wasm_runtime_deinstantiate(wasm_module_inst);
    wasm_runtime_unload(wasm_module);
    wasm_runtime_destroy();
    printf("WASM runtime destroyed.\n");
}