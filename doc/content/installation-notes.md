# Installation notes
This page describes how to install the RPIShutdown to your Raspberry Pi.

## Flashing the microcontroller

The code for the microcontroller must be compiled and flashed.
In the actual version the RPIShutdown uses a relais for switching the power of the Raspberry Pi. The circuit is an open-collector output. The described [schematic](Schematic-description) uses a P-Mosfet, so the code of the microcontroller must be changed.

## Connecting the RPIShutdown to the Raspberry Pi

Note: Update electrical connections

## Installing the Raspberry Pi daemon

1. Copy `RPIShutdown.py` from `src_pi` to your HOME directory (e.g. `/home/pi`)
2. Move `RPIShutdown.sh` from `src_pi`to `/etc/init.d`  
   `sudo mv RPIShutdown.sh /etc/init.d`
3. Change file attributes  
   `sudo chmod 755 /etc/init.d/RPIShutdown.sh`
4. Startup the script  
   `sudo service RPIShutdown.sh start`
5. Add script to startup  
   `sudo update-rc.d RPIShutdown.sh defaults`

If you change the directory of `RPIShutdown.py`, you will also need to change the path variable in `RPIShutdown.sh`. 

With `sudo service RPIShutdown.sh status` the status of the daemon can be checked.
