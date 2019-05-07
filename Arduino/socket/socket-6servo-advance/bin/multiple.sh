
#!/bin/bash
#
# Elapsed time.  Usage:
#
#   t=$(timer)
#   ... # do something
#   printf 'Elapsed time: %s\n' $(timer $t)
#      ===> Elapsed time: 0:01:12
#
#https://www.linuxjournal.com/content/use-date-command-measure-elapsed-time
#####################################################################
# If called with no arguments a new timer is returned.
# If called with arguments the first is used as a timer
# value and the elapsed time is returned in the form HH:MM:SS.
#
function timer()
{
    if [[ $# -eq 0 ]]; then
        echo $(date '+%s')
    else
        local  stime=$1
        etime=$(date '+%s')

        if [[ -z "$stime" ]]; then stime=$etime; fi

        dt=$((etime - stime))
        ds=$((dt % 60))
        dm=$(((dt / 60) % 60))
        dh=$((dt / 3600))
        printf '%d:%02d:%02d' $dh $dm $ds
    fi
}

# If invoked directly run test code.
if [[ $(basename $0 .sh) == 'timer' ]]; then
    t=$(timer)
    read -p 'Enter when ready...' p
    printf 'Elapsed time: %s\n' $(timer $t)
fi

## vim: tabstop=4: shiftwidth=4: noexpandtab:
## kate: tab-width 4; indent-width 4; replace-tabs false;

#glavna koda

echo "Miharix WiFi RC"
stevec=0
cas=$(timer)
read -p "Začnem programirati? Y/[N] : " programiraj
programiraj=${programiraj:="n"}
while [ $programiraj == 'y' ] || [ $programiraj == 'Y' ]
do
  stevec=$((stevec+1))
  printf '\nZaganjam programiranje %s. vezja\n' $stevec

  esptool.py --port /dev/ttyUSB0 write_flash 0x000000 socket-6servo-advance.ino.bin 0x300000 socket-6servo-advance.spiffs.bin

  printf '\nProgramiranje %s. vezja končano\n\n' $stevec

  read -p "Začnem programirati naslednjega? [Y]/N : " programiraj
programiraj=${programiraj:="y"}
done
printf '\n\nSprogramiranih je bilo %s vezij\n' $stevec
printf 'Za kar je bilo potrebno %s časa\n\n' $(timer $cas) 