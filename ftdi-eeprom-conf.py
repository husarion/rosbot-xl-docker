#!/usr/bin/env python3

from pyftdi.ftdi import Ftdi
from pyftdi.eeprom import FtdiEeprom

device = 'ftdi://ftdi:ft-x:/1'

ftdi = Ftdi()
ftdi.open_from_url(url=device)

eeprom = FtdiEeprom()
eeprom.connect(ftdi)

eeprom.set_property('cbus_func_0','GPIO')
eeprom.set_property('cbus_func_1','GPIO')

eeprom.commit(False)
eeprom.reset_device()
