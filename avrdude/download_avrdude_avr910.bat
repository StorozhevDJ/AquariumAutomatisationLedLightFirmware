avrdude.exe -p m8 -c avr910 -P COM5 -b 115200 -U flash:w:"..\Debug\Exe\my_LED_lamp.flash.hex":a
pause