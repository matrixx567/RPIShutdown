#!/usr/bin/env python

#import
import RPi.GPIO as GPIO
import time
import os

# Define GPIO to uC
RPI_TX = 30
RPI_RX = 28

def main():
    # Main program block

    GPIO.setmode(GPIO.BCM)       # Use BCM GPIO numbers
    GPIO.setup(RPI_RX,GPIO.IN)
    GPIO.setup(RPI_TX,GPIO.OUT)
    print("Shutdown Script: Startup")
    GPIO.output(RPI_TX, True)
    while True:
        if(GPIO.input(RPI_RX)):
            GPIO.output(RPI_TX,False)
            print("Shutdown Script: Shutdown")
            os.system("sudo shutdown -h now")
            break
        time.sleep(1)
    GPIO.output(RPI_TX,False)
    GPIO.cleanup();

if __name__ == '__main__':
    main()
