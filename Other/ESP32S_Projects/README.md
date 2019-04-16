# ESP32S_Projects
Projects I have done with the ESP32S by nodemcu


SerialToSerialBT_LEDWRITER:
Tests the Digital out and "Analog" out of the esp32s board.
notes: analog out is extremely tedious. Standard analog out should be written by espressif,
        will update code if thats is ever done. Until then, I should play around with it until I can 
        make a pseudo-knockoff analogWrite-esque function. 0-255
EDIT: although analogWrite is not present you can use the dac controler to get true analog out.
        Check out: https://esp-idf.readthedocs.io/en/v2.0/api/peripherals/dac.html LINK 1
        also for PWM
        Check out: https://techtutorialsx.com/2017/06/15/esp32-arduino-led-pwm-fading/ LINK 2
        
I might simplify the PWM to get arduino-like analogWrite since according to arduino, analogWrite is not a true analog output
check: https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/

tldr; need to make analogWrite Functions(which are actually PWM) using LINK 2. may also simplify true analog out using LINK 1.
