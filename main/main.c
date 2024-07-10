/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "esp_event.h"
#include <esp_log.h>

static const char *TAG = "webster";

/* Forware declaration */
void nvs_init(void);
void usb_init(void);
bool wifi_isup(void);
void wifi_init(void);
void httpd_init(void);

/* Main application */
void app_main(void)
{
    // Turn on event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize NVS subsystem
    nvs_init();

    // Connect USB
    usb_init();

    // Initialize the WiFi
    wifi_init();

    // Start web server
    httpd_init();

    // Watch for connection loss and try reconnect
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        if (!wifi_isup()) {     // WiFi failed, re-init
            ESP_LOGI(TAG, "WiFi down, attempting restart");
            wifi_init();
        }
    }
}
