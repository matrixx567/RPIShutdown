.. _rpi-software:


Raspberry PI Software
========================

The Raspberry PI runs a simple script that checks periodically the signal line
of the microcontroller.

The Python script uses the preinstalled GPIO library (Raspbian).
If the microcontroller sends a shutdown command to the Raspberry PI (input line ``RPI_RX`` goes high)
the script executes the halt command and turn off the Raspberry PI.

Running the shutdown script as a daemon
---------------------------------------

The source folder for the Raspberry PI software also contains a shell script, which is used
to set up the RPIShutdown software as a background daemon.

http://blog.scphillips.com/2013/07/getting-a-python-script-to-run-in-the-background-as-a-service-on-boot/

