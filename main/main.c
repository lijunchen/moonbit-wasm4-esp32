#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "st7789.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "control.h"
#include "lcd.h"
#include "runtime.h"
#include "util.h"
#include "wamr.h"
#include "wasm0.h"
#include "window.h"
#include <esp_system.h>
#include <nvs_flash.h>

#include <driver/gpio.h>
#include <driver/uart.h>
#include <esp_task_wdt.h>

#define INTERVAL 400
#define WAIT vTaskDelay(INTERVAL)

#define IWASM_MAIN_STACK_SIZE 5120

#define FATAL(msg, ...)                                                        \
  {                                                                            \
    printf("Fatal: " msg "\n", ##__VA_ARGS__);                                 \
    return;                                                                    \
  }

const extern unsigned char __game_card[];
extern unsigned int __game_card_len;

void w4_windowBoot();

uint16_t last[160 * 160];

extern void* wamr_get_phy_memory();

void run_wasm4(void* pvParameters) {
  w4_Disk disk = {0};
  uint8_t* memory = wamr_get_phy_memory();
  w4_runtimeInit(memory, &disk);
  // w4_wasmLoadModule(__tinypong_wasm, len);
  w4_windowBoot();
}

void feed_watchdog(void* pvParameter) {

  esp_task_wdt_add(NULL);

  for (;;) {
    printf("Feed 🐶\n");
    esp_task_wdt_reset();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void print_chip_info() {
  /* Print chip information */
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU core(s), %s%s%s%s, ", CONFIG_IDF_TARGET,
         chip_info.cores,
         (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
         (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
         (chip_info.features & CHIP_FEATURE_IEEE802154)
             ? ", 802.15.4 (Zigbee/Thread)"
             : "");

  unsigned major_rev = chip_info.revision / 100;
  unsigned minor_rev = chip_info.revision % 100;
  printf("silicon revision v%d.%d, ", major_rev, minor_rev);
  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
    printf("Get flash size failed");
    return;
  }

  printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
         (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded"
                                                       : "external");
  printf("Minimum free heap size: %" PRIu32 " bytes\n",
         esp_get_minimum_free_heap_size());
}

void init_watch_dog() {
  const esp_task_wdt_config_t cfg = {
      .timeout_ms = 30000,
      .idle_core_mask = 0,
      .trigger_panic = false,
  };
  esp_task_wdt_init(&cfg);
}

void fillTest() {
  printf("Fill Red\n");
  lcdFillScreen(&dev, RED);
  lcdDrawFinish(&dev);
  vTaskDelay(pdMS_TO_TICKS(100));

  printf("Fill Green\n");
  lcdFillScreen(&dev, GREEN);
  lcdDrawFinish(&dev);
  vTaskDelay(pdMS_TO_TICKS(100));

  printf("Fill Blue\n");
  lcdFillScreen(&dev, BLUE);
  lcdDrawFinish(&dev);
  vTaskDelay(pdMS_TO_TICKS(100));
}

#define BUF_SIZE (1024)

char card_data[48 * 1024];
int card_length = 0;

extern uint32_t stop;

void uart_rx_task(void* pvParameter) {
  uint8_t buf[1024];
  while (1) {
    uint32_t current = 0;
    uint32_t total_size = 0;
    int n = uart_read_bytes(UART_NUM_0, (uint8_t*)&total_size,
                            sizeof(total_size), pdMS_TO_TICKS(1000));
    if (n <= 0) {
      vTaskDelay(10 / portTICK_PERIOD_MS);
      continue;
    }
    printf("expected total_size: %ld\n", total_size);
    current += n;
    uint32_t type = 0;
    n = uart_read_bytes(UART_NUM_0, (uint8_t*)&type, sizeof(type),
                        pdMS_TO_TICKS(1000));
    if (n <= 0) {
      vTaskDelay(10 / portTICK_PERIOD_MS);
      continue;
    }
    printf("frame type: %ld\n", type);
    current += n;

    if (type == 1234) {
      stop = 1;
      int64_t t0 = esp_timer_get_time() / 1000;
      while (current < total_size) {
        int len =
            uart_read_bytes(UART_NUM_0, buf, sizeof(buf), pdMS_TO_TICKS(100));
        if (len > 0) {
          memcpy(card_data + current - 8, buf, len);
          current += len;
        }
      }
      int64_t t1 = esp_timer_get_time() / 1000;
      printf("received %d bytes in %lld ms\n", total_size, t1 - t0);
      card_length = total_size - 8;
      card_data[card_length] = '\0';
      int checksum = 0;
      for (int i = 0; i < card_length; i++) {
        checksum += card_data[i];
      }

      printf("card length: %d, checksum: %d\n", card_length, checksum);
      for (int i = 0; i < card_length && i < 32; i++) {
        char c = card_data[i];
        if (isprint(c)) {
          printf("%c", c);
        } else {
          if (c == '\n') {
            printf("\n");
          } else if (c == '\r') {
            printf("\n");
          } else {
            printf(".");
          }
        }
      }

      vTaskDelay(100 / portTICK_PERIOD_MS);
      fillTest();
      stop = 0;

      printf("\n");
    } else if (type == 0x1122) {
      int32_t payload = 0;
      n = uart_read_bytes(UART_NUM_0, (uint8_t*)&payload, sizeof(payload),
                          pdMS_TO_TICKS(1000));
      printf("received 0x1122, payload: %d\n", payload);
      set_player_state(0, (int8_t)payload);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void init_uart() {
  uart_config_t uart_cfg = {.baud_rate = 115200,
                            .data_bits = UART_DATA_8_BITS,
                            .parity = UART_PARITY_ODD,
                            .stop_bits = UART_STOP_BITS_1,
                            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
  uart_param_config(UART_NUM_0, &uart_cfg);
  uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
  printf("ESP32 ready to receive data over UART0...\n");
}

void app_main(void) {
  //   init_watch_dog();
  //   xTaskCreate(feed_watchdog, "feed_watchdog", 2048, NULL, 6, NULL);

  printf("init uart\n");
  vTaskDelay(100 / portTICK_PERIOD_MS);
  init_uart();
  printf("init uart FINISHED\n");

  init_button();
  nvs_flash_init();
  vTaskDelay(100 / portTICK_PERIOD_MS);
  ST7789(NULL);
  fillTest();

  //   // memory check
  //   for (int i = 0; i < 64 * 1024; i++) {
  //     card_data[i] = 1;
  //   }
  //   int checksum = 0;
  //   for (int i = 0; i < 64 * 1024; i++) {
  //     checksum += card_data[i];
  //   }
  //   printf("card_data %p, memory check: %d\n", card_data, checksum);

  xTaskCreate(uart_rx_task, "uart_rx_task", 1024 * 4, NULL, 10, NULL);

  pthread_t t;
  int res;
  pthread_attr_t tattr;
  pthread_attr_init(&tattr);
  pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setstacksize(&tattr, IWASM_MAIN_STACK_SIZE);
  res = pthread_create(&t, &tattr, init_wamr, (void*)NULL);

  while (1) {
    printf("Main loop is running\n");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
