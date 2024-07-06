# webster
This project combines a web server and a USB keyboard to allow control over the boot sequence of a dual-boot machine.

The key sequence set in response to a POST command is deliberately limited to a sequence of F8's, 0-3 Down Arrows, followed by Enter. This enters the boot selection window, and blindly selects one of the items.

The code could be extended to allow more control over the key sequence being sent, but this is a fairly severe security hole. A malicious user could then send any sequence of keystrokes to your computer.

## Tool chain
ESP-IDF V5.2.2

## Project configuration
```
source ../esp-idf/export.sh
idf.py set-target esp32s3
idf.py menuconfig
-> Webkey Configuration -> SSID/Password
-> Compont -> LWIP -> netif hostname
```

## Build instructions
```
source ../esp-idf/export.sh
idf.py build
idf.py -p /dev/ttyUSB0 erase_flash
idf.py -p /dev/ttyUSB0 flash monitor
```

## Operation
To use programatically:
```
curl -X POST http://[hostname]/ctrl?key=b1
curl -X POST http://[hostname]/ctrl?key=b2
curl -X POST http://[hostname]/ctrl?key=b3
curl -X POST http://[hostname]/ctrl?key=b4
```

There is also a lovely web page at http://[hostname]/index.html that provides pushbuttons.
