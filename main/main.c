/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

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

#include "wasm3.h"
#include "fib32.wasm.h"

#include "apu.h"
#include "runtime.h"
#include "wasm0.h"
#include "window.h"
#include "util.h"
#include "lcd.h"
#include <esp_task_wdt.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include "esp_http_server.h"
#include "wifi.h"
#include "http.h"
#include "wamr.h"
#include "iot_button.h"

#define INTERVAL 400
#define WAIT vTaskDelay(INTERVAL)

#define IWASM_MAIN_STACK_SIZE 5120

TickType_t FillTest(TFT_t * dev, int width, int height) {
    TickType_t startTick, endTick, diffTick;
    startTick = xTaskGetTickCount();

    {
        for (int i = 0; i < 2; i++) {
            TickType_t startTick, endTick, diffTick;
            startTick = xTaskGetTickCount();
            lcdFillScreen(dev, WHITE);
            lcdFillScreen(dev, GREEN);
            lcdFillScreen(dev, BLACK);
            endTick = xTaskGetTickCount();
            diffTick = endTick - startTick;
            ESP_LOGI(__FUNCTION__, "write a line elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
        }
    }


    endTick = xTaskGetTickCount();
    diffTick = endTick - startTick;
    ESP_LOGI(__FUNCTION__, "elapsed time[ms]:%"PRIu32,diffTick*portTICK_PERIOD_MS);
    return diffTick;
}

TFT_t dev;

#define CUSTOM_MOSI_GPIO 23
#define CUSTOM_SCLK_GPIO 20
#define CUSTOM_CS_GPIO -1
#define CUSTOM_DC_GPIO 21
#define CUSTOM_RESET_GPIO 22
#define CUSTOM_BL_GPIO -1

void ST7789(void *pvParameters)
{
    // set font file
    FontxFile fx16G[2];
    FontxFile fx24G[2];
    FontxFile fx32G[2];
    FontxFile fx32L[2];
    InitFontx(fx16G,"/spiffs/ILGH16XB.FNT",""); // 8x16Dot Gothic
    InitFontx(fx24G,"/spiffs/ILGH24XB.FNT",""); // 12x24Dot Gothic
    InitFontx(fx32G,"/spiffs/ILGH32XB.FNT",""); // 16x32Dot Gothic
    InitFontx(fx32L,"/spiffs/LATIN32B.FNT",""); // 16x32Dot Latin

    FontxFile fx16M[2];
    FontxFile fx24M[2];
    FontxFile fx32M[2];
    InitFontx(fx16M,"/spiffs/ILMH16XB.FNT",""); // 8x16Dot Mincyo
    InitFontx(fx24M,"/spiffs/ILMH24XB.FNT",""); // 12x24Dot Mincyo
    InitFontx(fx32M,"/spiffs/ILMH32XB.FNT",""); // 16x32Dot Mincyo
    
    // Change SPI Clock Frequency
    //spi_clock_speed(40000000); // 40MHz
    //spi_clock_speed(60000000); // 60MHz

    spi_master_init(&dev, CUSTOM_MOSI_GPIO, CUSTOM_SCLK_GPIO, CUSTOM_CS_GPIO, CUSTOM_DC_GPIO, CUSTOM_RESET_GPIO, CUSTOM_BL_GPIO);
    lcdInit(&dev, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);

    lcdFillScreen(&dev, BLACK);
}

#define FATAL(msg, ...) { printf("Fatal: " msg "\n", ##__VA_ARGS__); return; }

static void run_wasm(void)
{
    M3Result result = m3Err_none;

    uint8_t* wasm = (uint8_t*)fib32_wasm;
    uint32_t fsize = fib32_wasm_len;

    printf("Loading WebAssembly...\n");
    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("m3_NewEnvironment failed");

    IM3Runtime runtime = m3_NewRuntime (env, 1024, NULL);
    if (!runtime) FATAL("m3_NewRuntime failed");

    IM3Module module;
    result = m3_ParseModule (env, &module, wasm, fsize);
    if (result) FATAL("m3_ParseModule: %s", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("m3_LoadModule: %s", result);

    IM3Function f;
    result = m3_FindFunction (&f, runtime, "fib");
    if (result) FATAL("m3_FindFunction: %s", result);

    printf("Running...\n");

    result = m3_CallV(f, 24);
    if (result) FATAL("m3_Call: %s", result);

    unsigned value = 0;
    result = m3_GetResultsV (f, &value);
    if (result) FATAL("m3_GetResults: %s", result);

    printf("Result: %u\n", value);
}

extern unsigned char __tinypong_wasm[];
extern unsigned int __tinypong_wasm_len;

void w4_windowBoot ();

uint16_t last[160*160];

extern void* wamr_get_phy_memory();

void run_wasm4(void *pvParameters) {
    w4_Disk disk = {0};
    printf("Step 0\n");
    int len = __tinypong_wasm_len;
    printf("Step 1\n");
    uint8_t* memory = wamr_get_phy_memory();
    printf("Step 2, memory %p\n", memory);
    w4_runtimeInit(memory, &disk);
    printf("Step 3\n");
    // w4_wasmLoadModule(__tinypong_wasm, len);
    printf("Step 4");
    w4_windowBoot();
}

extern void set_player_state(int player, uint8_t state);

static void button_left(void* arg, void* usr_data)
{
    printf("set player 0 state to 16\n");
    set_player_state(0, 16);
}

static void button_right(void* arg, void* usr_data)
{
    printf("set player 0 state to 32\n");
    set_player_state(0, 32);
}

static void button_up(void* arg, void* usr_data)
{
    printf("set player 0 state to 64\n");
    set_player_state(0, 64);
}

static void button_down(void* arg, void* usr_data)
{
    printf("set player 0 state to 128\n");
    set_player_state(0, 128);
}

static void button_x(void* arg, void* usr_data)
{
    printf("set player 0 state to 1\n");
    set_player_state(0, 1);
}


void app_main(void)
{
    printf("Hello world!\n");

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

    // printf("\nWasm3 v" M3_VERSION " on " CONFIG_IDF_TARGET ", build " __DATE__ " " __TIME__ "\n");
    // run_wasm();

    // create gpio button
    {
        button_config_t gpio_btn_cfg = {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
            .short_press_time = 100,
            .gpio_button_config = {
                .gpio_num = GPIO_NUM_2,
                .active_level = 0,
            },
        };
        button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
        if (NULL == gpio_btn) {
            printf("Button create failed\n");
        }
        iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_up, (void *)BUTTON_SINGLE_CLICK);
    }
    {
        button_config_t gpio_btn_cfg = {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
            .short_press_time = 100,
            .gpio_button_config = {
                .gpio_num = GPIO_NUM_3,
                .active_level = 0,
            },
        };
        button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
        if (NULL == gpio_btn) {
            printf("Button create failed\n");
        }
        iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_right, (void *)BUTTON_SINGLE_CLICK);
    }
    {
        button_config_t gpio_btn_cfg = {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
            .short_press_time = 100,
            .gpio_button_config = {
                .gpio_num = GPIO_NUM_10,
                .active_level = 0,
            },
        };
        button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
        if (NULL == gpio_btn) {
            printf("Button create failed\n");
        }
        iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_left, (void *)BUTTON_SINGLE_CLICK);
    }
    {
        button_config_t gpio_btn_cfg = {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
            .short_press_time = 100,
            .gpio_button_config = {
                .gpio_num = GPIO_NUM_11,
                .active_level = 0,
            },
        };
        button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
        if (NULL == gpio_btn) {
            printf("Button create failed\n");
        }
        iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_down, (void *)BUTTON_SINGLE_CLICK);
    }


    nvs_flash_init();
    // wifi_init_sta();

    ST7789(NULL);
    FillTest(&dev, CONFIG_WIDTH, CONFIG_HEIGHT);
    
    printf("WAMR test begin\n");

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

    static httpd_handle_t server = NULL;


    /* Start the server for the first time */
    // server = start_webserver();


    // run_wasm4(NULL);

    xTaskCreate(run_wasm4, "wasm4", 1024*6, NULL, 2, NULL);

    while (1) {
        printf("Main loop is running\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Delay for 2 seconds
    }
}
