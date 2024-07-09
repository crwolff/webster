include(../main/version.cmake)

idf_component_register(SRCS "httpd.c" "main.c" "nvs.c" "ota.c" "usb.c" "wifi.c"
                    INCLUDE_DIRS "."
                    REQUIRES "nvs_flash"
                    PRIV_REQUIRES "app_update" "driver" "esp_http_server" "esp_wifi"
                    EMBED_FILES "www-data/favicon.ico" "www-data/index.html" "www-data/config.html")
