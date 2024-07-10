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
#include "tinyusb.h"
#include "usb_descriptors.h"

static const char *TAG = "webster";

/* Forware declaration */
void nvs_init(void);
void usb_init(void);
bool wifi_isup(void);
void wifi_init(void);
void httpd_init(void);

extern uint32_t button_pressed;

/* Main application */
void app_main(void)
{
    uint8_t sequence;
    uint8_t key_active;

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

    // Main task - loop forever
    while (1) {
        // Watch for connection loss and try reconnect
        vTaskDelay(pdMS_TO_TICKS(1000));
        if (!wifi_isup()) {     // WiFi failed, re-init
            ESP_LOGI(TAG, "WiFi down, attempting restart");
            wifi_init();
        }

        // Web server sets button_pressed based on POST command
        // Record button_pressed so it can't change during sequence
        uint32_t btn = button_pressed;
        if ( btn ) {
            // Send key sequence
            //     30 spaces (halts grub autoboot)
            //     n down arrows corresponding to button number
            //     ENTER to start boot
            sequence = 33;
            key_active = 0;
            for(int i=0;(i < 6000) && (sequence != 0);i++) {
                if ( tud_suspended() ) {
                    // Wake up host if we are in suspend mode
                    // and REMOTE_WAKEUP feature is enabled by host
                    tud_remote_wakeup();
                }

                // Send next keypress in sequence
                if ( tud_hid_ready() ) {
                    if ( key_active == 0 ) {
                        uint8_t keycode[6] = { 0 };
                        if ( sequence == 1 ) {
                            keycode[0] = HID_KEY_ENTER;
                        } else if ( sequence <= btn ) {
                            keycode[0] = HID_KEY_ARROW_DOWN;
                        } else {
                            keycode[0] = HID_KEY_SPACE;
                        }
                        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
                        key_active = 1;
                        vTaskDelay(pdMS_TO_TICKS(10));
                    } else {
                        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
                        sequence = sequence - 1;
                        key_active = 0;
                        vTaskDelay(pdMS_TO_TICKS(500));
                    }
                } else {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            }
            if (sequence != 0)
                printf("Timeout before sequence ended\n");

            // Clear command and discard any that occurred during execution
            button_pressed = 0;
        }
    }
}
