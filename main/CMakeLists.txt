idf_component_register(SRCS "main.c" "nvs.c" "wifi.c"
                    INCLUDE_DIRS "."
                    REQUIRES "nvs_flash"
                    PRIV_REQUIRES "esp_wifi")
