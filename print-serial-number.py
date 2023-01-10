#!/usr/bin/env python3

import hashlib
import serial
import string
import sh
import argparse
import sys

class SerialNumberGenerator:

    def __init__(self, binary_file, port):
        self.binary_file = binary_file
        self.port = port

    def hex_to_num(self, hex_str):
        # Check if the hex string is valid
        if not all(c in string.hexdigits for c in hex_str):
            raise ValueError("Invalid hex string")
    
        # Convert the hex string to bytes
        try:
            hex_bytes = bytes.fromhex(hex_str)
        except ValueError:
            raise ValueError("Invalid hex string")
    
        # Compute the SHA-256 hash of the hex bytes
        hash = hashlib.sha256(hex_bytes).hexdigest()
    
        # Truncate the hash to 6 characters
        hash = hash[:6]
    
        # Return the hash as an ASCII string
        return hash

    def to_two_digit_hex(self, hex_str):
        # Pad the hex string with a leading zero if it is a single-digit string
        if len(hex_str) == 1:
            hex_str = "0" + hex_str
        return hex_str

    def convert_hex_string(self, hex_str):
        # Split the hex string into a list of hexadecimal values
        hex_values = hex_str.split()
        # Convert each hexadecimal value to a two-digit string
        hex_values = [self.to_two_digit_hex(x) for x in hex_values]
        # Join the list of hexadecimal values into a single string
        hex_str = "".join(hex_values)
        return hex_str

    def generate(self):
        print('flash the firmware that prints STM32 unique ID')
        sh.python3('/usr/bin/flash-firmware.py', f=self.binary_file, p=self.port)
        print('done')
        print('')

        # Open the serial port
        with serial.Serial(self.port, baudrate=9600, timeout=1) as ser:
            # Read a line of input from the serial port
            hex_str = ser.readline().decode("utf-8").strip()
            hex_str = self.convert_hex_string(hex_str)

            print(f"CPU ID = 0x{hex_str}")

            # Remove the spaces in the hex string
            hex_str = hex_str.replace(" ", "")

            # Convert the hex string to a 6-character ASCII string
            result = self.hex_to_num(hex_str)
            # Print the result to the console
            print(f"Serial Number = {result}")
            print()

        # Close the serial port
        ser.close()    
        return result


def main():

    parser = argparse.ArgumentParser(
        description='Printing ROSbot XL serial number')

    parser.add_argument(
        "-f",
        "--file",
        nargs='?',
        default="/firmware_read_cpu_id.bin",
        help="Path to a firmware file. Default: /firmware_read_cpu_id.bin")
    parser.add_argument(
        "-p",
        "--port",
        nargs='?',
        default="/dev/ttyUSB0",
        help="Path to serial connection. Default: /dev/ttyUSB0")

    binary_file = parser.parse_args().file
    port = parser.parse_args().port

    sn = SerialNumberGenerator(binary_file, port)
    serialNumber = sn.generate()

    sys.exit(serialNumber)

if __name__ == "__main__":
    main()





