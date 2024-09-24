#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "st7789.h"
#include <string.h>

#include "runtime.h"
#include "wasm0.h"
#include "window.h"
#include "util.h"
#include "lcd.h"
#include <esp_task_wdt.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include "esp_http_server.h"
#include "wamr.h"


#define INTERVAL 400
#define WAIT vTaskDelay(INTERVAL)

#define IWASM_MAIN_STACK_SIZE 5120

#define FATAL(msg, ...) { printf("Fatal: " msg "\n", ##__VA_ARGS__); return; }


extern unsigned char __game_card[];
extern unsigned int __game_card_len;

void w4_windowBoot ();

uint16_t last[160*160];

extern void* wamr_get_phy_memory();

void run_wasm4(void *pvParameters) {
    w4_Disk disk = {0};
    uint8_t* memory = wamr_get_phy_memory();
    w4_runtimeInit(memory, &disk);
    // w4_wasmLoadModule(__tinypong_wasm, len);
    w4_windowBoot();
}

void app_main(void)
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    nvs_flash_init();
    ST7789(NULL);
    FillTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
    pthread_t t;
    int res;
    pthread_attr_t tattr;
    pthread_attr_init(&tattr);
    pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setstacksize(&tattr, IWASM_MAIN_STACK_SIZE);
    res = pthread_create(&t, &tattr, init_wamr, (void *)NULL);
    assert(res == 0);
    res = pthread_join(t, NULL);
    assert(res == 0);
    xTaskCreate(run_wasm4, "wasm4", 1024*6, NULL, 2, NULL);
    while (1) {
        printf("Main loop is running\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
