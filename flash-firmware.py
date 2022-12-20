#!/usr/bin/python3

import sh
import time
import sys
import argparse
from pyftdi.ftdi import Ftdi

# CBUS0 - BOOT0
# CBUS1 - RST

class FirmwareFlasher:
    def __init__(self, binary_file, port):

        # self.ftdi = Ftdi.create_from_url('ftdi://ftdi:ft-x:DK0AM0V0/1')
        self.device = 'ftdi://ftdi:ft-x:DK0AM0V0/1'
        self.ftdi = Ftdi()

        self.binary_file = binary_file
        self.max_approach_no = 5
        self.port = port

    def enter_bootloader_mode(self):

        self.ftdi.open_from_url(url=self.device)
        self.ftdi.set_cbus_direction(0b11,0b11) # set CBUS0 and CBUS1 to output
        time.sleep(0.1)
        self.ftdi.set_cbus_gpio(0b11) #set CBUS0 to 1 and RST to 1
        time.sleep(0.1)
        self.ftdi.set_cbus_gpio(0b01) #set CBUS0 to 1 and RST to 0
        time.sleep(0.1)
        # self.ftdi.set_cbus_direction(0b11,0b00) # set CBUS0 and CBUS1 to input
        time.sleep(0.1)
        self.ftdi.close()


    def exit_bootloader_mode(self):

        self.ftdi.open_from_url(url=self.device)
        self.ftdi.set_cbus_direction(0b11,0b11) # set CBUS0 and CBUS1 to output
        time.sleep(0.1)
        self.ftdi.set_cbus_gpio(0b10) #set CBUS0 to 1 and RST to 1
        time.sleep(0.1)
        self.ftdi.set_cbus_gpio(0b00) #set CBUS0 to 1 and RST to 0
        time.sleep(0.1)
        # self.ftdi.set_cbus_direction(0b11,0b00) # set CBUS0 and CBUS1 to input
        time.sleep(0.1)
        self.ftdi.close()


    def flash_firmware(self):

        self.enter_bootloader_mode()
        sh.usbreset("0403:6015")    # workaround: using pyftdi causes laggy serial port. This line is like unplug/plug for USB port
        sh.stm32flash(self.port, "-v", w=self.binary_file, b="115200", _out=sys.stdout)
        self.exit_bootloader_mode()
        sh.usbreset("0403:6015")


def main():

    parser = argparse.ArgumentParser(
        description='Flashing the firmware on STM32 microcontroller in ROSbot XL')

    parser.add_argument(
        "file",
        nargs='?',
        default="/firmware.bin",
        help="Path to a firmware file. Default: /firmware.bin")
    parser.add_argument(
        "port",
        nargs='?',
        default="/dev/ttyUSB0",
        help="Path to serial connection. Default: /dev/ttyUSB0")

    binary_file = parser.parse_args().file
    port = parser.parse_args().port

    flasher = FirmwareFlasher(binary_file, port)
    flasher.flash_firmware()
    print("Done.")


if __name__ == "__main__":
    main()