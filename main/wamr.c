#include "wamr.h"
#include "runtime.h"

void wrap_w4_runtimeBlit(wasm_exec_env_t exec_env, const uint8_t* sprite, int x,
                         int y, int width, int height, int flags) {
  // printf("Call wrap_w4_runtimeBlit\n");
  w4_runtimeBlit(sprite, x, y, width, height, flags);
}
void wrap_w4_runtimeBlitSub(wasm_exec_env_t exec_env, const uint8_t* sprite,
                            int x, int y, int width, int height, int srcX,
                            int srcY, int stride, int flags) {
  // printf("Call wrap_w4_runtimeBlitSub\n");
  w4_runtimeBlitSub(sprite, x, y, width, height, srcX, srcY, stride, flags);
}
void wrap_w4_runtimeLine(wasm_exec_env_t exec_env, int x1, int y1, int x2,
                         int y2) {
  // printf("Call wrap_w4_runtimeLine\n");
  w4_runtimeLine(x1, y1, x2, y2);
}
void wrap_w4_runtimeHLine(wasm_exec_env_t exec_env, int x, int y, int len) {
  // printf("Call wrap_w4_runtimeHLine\n");
  w4_runtimeHLine(x, y, len);
}
void wrap_w4_runtimeVLine(wasm_exec_env_t exec_env, int x, int y, int len) {
  // printf("Call wrap_w4_runtimeVLine\n");
  w4_runtimeVLine(x, y, len);
}
void wrap_w4_runtimeOval(wasm_exec_env_t exec_env, int x, int y, int width,
                         int height) {
  // printf("Call wrap_w4_runtimeOval\n");
  w4_runtimeOval(x, y, width, height);
}
void wrap_w4_runtimeRect(wasm_exec_env_t exec_env, int x, int y, int width,
                         int height) {
  // printf("Call wrap_w4_runtimeRect\n");
  w4_runtimeRect(x, y, width, height);
}
void wrap_w4_runtimeText(wasm_exec_env_t exec_env, const uint8_t* str, int x,
                         int y) {
  // printf("Call wrap_w4_runtimeText\n");
  w4_runtimeText(str, x, y);
}
void wrap_w4_runtimeTextUtf8(wasm_exec_env_t exec_env, const uint8_t* str,
                             int byteLength, int x, int y) {
  // printf("Call wrap_w4_runtimeTextUtf8\n");
  w4_runtimeTextUtf8(str, byteLength, x, y);
}
void wrap_w4_runtimeTextUtf16(wasm_exec_env_t exec_env, const uint16_t* str,
                              int byteLength, int x, int y) {
  // printf("Call wrap_w4_runtimeTextUtf16\n");
  w4_runtimeTextUtf16(str, byteLength, x, y);
}
void wrap_w4_runtimeTone(wasm_exec_env_t exec_env, int frequency, int duration,
                         int volume, int flags) {
  // printf("Call wrap_w4_runtimeTone\n");
  w4_runtimeTone(frequency, duration, volume, flags);
}
int wrap_w4_runtimeDiskr(wasm_exec_env_t exec_env, uint8_t* dest, int size) {
  // printf("Call wrap_w4_runtimeDiskr\n");
  return w4_runtimeDiskr(dest, size);
}
int wrap_w4_runtimeDiskw(wasm_exec_env_t exec_env, const uint8_t* src,
                         int size) {
  // printf("Call wrap_w4_runtimeDiskw\n");
  return w4_runtimeDiskw(src, size);
}
void wrap_w4_runtimeTrace(wasm_exec_env_t exec_env, const uint8_t* str) {
  // printf("Call wrap_w4_runtimeTrace\n");
  w4_runtimeTrace(str);
}
void wrap_w4_runtimeTraceUtf8(wasm_exec_env_t exec_env, const uint8_t* str,
                              int byteLength) {
  // printf("Call wrap_w4_runtimeTraceUtf8\n");
  w4_runtimeTraceUtf8(str, byteLength);
}
void wrap_w4_runtimeTraceUtf16(wasm_exec_env_t exec_env, const uint16_t* str,
                               int byteLength) {
  // printf("Call wrap_w4_runtimeTraceUtf16\n");
  w4_runtimeTraceUtf16(str, byteLength);
}
void wrap_w4_runtimeTracef(wasm_exec_env_t exec_env, const uint8_t* str,
                           const void* stack) {
  // printf("Call wrap_w4_runtimeTracef\n");
  w4_runtimeTracef(str, stack);
}

void just_print_int(wasm_exec_env_t exec_env, int a) {
  printf("JUST PRINT INT %d\n", a);
}

static NativeSymbol native_symbols[] = {
    {"just_print_int", just_print_int, "(i)"},
    {"blit", wrap_w4_runtimeBlit, "(*iiiii)"},
    {"blitSub", wrap_w4_runtimeBlitSub, "(*iiiiiiii)"},
    {"line", wrap_w4_runtimeLine, "(iiii)"},
    {"hline", wrap_w4_runtimeHLine, "(iii)"},
    {"vline", wrap_w4_runtimeVLine, "(iii)"},
    {"oval", wrap_w4_runtimeOval, "(iiii)"},
    {"rect", wrap_w4_runtimeRect, "(iiii)"},
    {"text", wrap_w4_runtimeText, "(*ii)"},

    {"textUtf8", wrap_w4_runtimeTextUtf8, "(iiii)"},
    {"textUtf16", wrap_w4_runtimeTextUtf16, "(iiii)"},

    {"tone", wrap_w4_runtimeTone, "(iiii)"},

    {"diskr", wrap_w4_runtimeDiskr, "(ii)i"},
    {"diskw", wrap_w4_runtimeDiskw, "(ii)i"},

    {"trace", wrap_w4_runtimeTrace, "(i)"},

    {"traceUtf8", wrap_w4_runtimeTraceUtf8, "(ii)"},
    {"traceUtf16", wrap_w4_runtimeTraceUtf16, "(ii)"},
    {"tracef", wrap_w4_runtimeTracef, "(ii)"},
};

extern uint32_t stop;
extern char card_data[];
extern int card_length;

const extern unsigned char __game_card[];
extern unsigned int __game_card_len;

wasm_module_t wasm_module = NULL;
wasm_module_inst_t wasm_module_inst = NULL;
wasm_function_inst_t start = NULL;
wasm_function_inst_t update = NULL;
wasm_exec_env_t exec_env = NULL;
wasm_exec_env_t exec_env2 = NULL;

extern void run_wasm4(void* pvParameters);

int first = 1;

void load_tinypong() {
  char error_buf[128];

  if (first) {
    memcpy(card_data, __game_card, __game_card_len);
    card_length = __game_card_len;
    first = 0;
  }

  int checksum = 0;
  for (int i = 0; i < card_length; i++) {
    checksum += card_data[i];
  }
  printf("[LOAD TINYPING]: card_data: %p, checksum: %d\n", card_data, checksum);
  if (wasm_module != NULL) {
    wasm_runtime_unload(wasm_module);
    wasm_module = NULL;
  }
  wasm_module =
      wasm_runtime_load(card_data, card_length, error_buf, sizeof(error_buf));

  if (!wasm_module) {
    printf("Failed to load wasm module: %s\n", error_buf);
    return;
  }

  printf("Instantiate the wasm module\n");
  if (wasm_module_inst != NULL) {
    wasm_runtime_deinstantiate(wasm_module_inst);
    wasm_module_inst = NULL;
  }
  wasm_module_inst = wasm_runtime_instantiate(wasm_module, 16 * 1024, 64 * 1024,
                                              error_buf, sizeof(error_buf));
  if (!wasm_module_inst) {
    printf("Failed to instantiate wasm module: %s\n", error_buf);
    wasm_runtime_unload(wasm_module);
    return;
  }

  start = wasm_runtime_lookup_function(wasm_module_inst, "start");
  printf("start: %p\n", start);

  update = wasm_runtime_lookup_function(wasm_module_inst, "update");
  printf("update: %p\n", update);

  if (exec_env != NULL) {
    wasm_runtime_destroy_exec_env(exec_env);
    exec_env = NULL;
  }
  exec_env = wasm_runtime_create_exec_env(wasm_module_inst, 2 * 1024);

  if (exec_env2 != NULL) {
    wasm_runtime_destroy_exec_env(exec_env2);
    exec_env2 = NULL;
  }
}

void print_memory_info() { heap_caps_print_heap_info(MALLOC_CAP_8BIT); }

void init_wamr() {
  /* Setup variables for instantiating and running the wasm module */

  char error_buf[128];
  RuntimeInitArgs init_args;

  /* Configure memory allocation */
  memset(&init_args, 0, sizeof(RuntimeInitArgs));
  init_args.mem_alloc_type = Alloc_With_Allocator;
  init_args.mem_alloc_option.allocator.malloc_func = (void*)os_malloc;
  init_args.mem_alloc_option.allocator.realloc_func = (void*)os_realloc;
  init_args.mem_alloc_option.allocator.free_func = (void*)os_free;

  init_args.native_module_name = "env";
  init_args.native_symbols = native_symbols;
  init_args.n_native_symbols = sizeof(native_symbols) / sizeof(NativeSymbol);

  print_memory_info();

  char* p0 = (char*)os_malloc(1024);
  printf("os_malloc p0: %p\n", p0);
  char* p1 = (char*)os_malloc(1024);
  printf("os_malloc p1: %p\n", p1);

  printf("default game card address: %p\n", __game_card);
  printf("game card buffer address: %p\n", card_data);

  printf("Initialize WASM runtime\n");
  /* Initialize runtime environment */
  if (!wasm_runtime_full_init(&init_args)) {
    printf("Init runtime failed.\n");
    return;
  }

  while (true) {
    if (stop == 1) {
      printf("[WAMR] wait restart signal\n");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }
    printf("In loop\n");
    print_memory_info();
    load_tinypong();
    run_wasm4(NULL);
  }

  /* Clean up */
  // wasm_runtime_deinstantiate(wasm_module_inst);
  // wasm_runtime_unload(wasm_module);
  // wasm_runtime_destroy();
  // printf("WASM runtime destroyed.\n");
}

void* wamr_get_phy_memory() {
  return wasm_runtime_addr_app_to_native(wasm_module_inst, 0);
}

void w4_wasmCallStart() {
  if (start) {
    printf("Call start %p\n", start);
    wasm_runtime_call_wasm(exec_env, start, 0, NULL);
  }
}

void w4_wasmCallUpdate() {
  update = wasm_runtime_lookup_function(wasm_module_inst, "update");
  if (!exec_env2) {
    exec_env2 = wasm_runtime_create_exec_env(wasm_module_inst, 10 * 1024);
  }
  wasm_runtime_call_wasm(exec_env2, update, 0, NULL);
}