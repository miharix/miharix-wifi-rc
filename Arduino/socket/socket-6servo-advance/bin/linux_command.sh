pip install esptool

esptool.py --port /dev/ttyUSB0 write_flash 0x000000 socket-6servo-advance.ino.bin 0x300000 socket-6servo-advance.spiffs.bin