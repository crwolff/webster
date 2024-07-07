/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <esp_log.h>

static const char *TAG = "webster";

extern int32_t web_server_down;

/* Forware declaration */
void nvs_init(void);
void wifi_init(void);
void httpd_init(void);

/* Main application */
void app_main(void)
{
    // Initialize NVS subsystem
    nvs_init();

    // Initialize the WiFi
    wifi_init();

    // Start web server
    httpd_init();

    // Watch for connection loss and try reconnect
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
