# PWMSerialOut
Arduino sample code for fast serial output with PWM

The sample code is intended to communicate with NeoPixel LEDs at 800kHz on an 8MHz Arduino.

__CAUTION__:  
The code slows down the MCU clock speed by a factor of 16.
This was done for easier debugging during development.

An extensive decription can be found on my blog post [Ride the PWM](https://kwasi-ich.de/blog/2019/11/15/pwm_rider/)
