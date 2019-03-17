#!/bin/bash
echo ""Miharix WiFi RC""
read -p "Zacnem programirati? Y/[N] : " programiraj
programiraj=${programiraj:="n"}
while [ $programiraj == 'y' ] || [ $programiraj == 'Y' ]
do
  echo "Zaganjam programiranje"

  esptool.py --port /dev/ttyUSB0 write_flash 0x000000 socket-6servo-advance.ino.bin 0x300000 socket-6servo-advance.spiffs.bin

  echo "Programiranje končano"

  read -p "Zacnem programirati naslednjega? [Y]/N : " programiraj
programiraj=${programiraj:="y"}
done