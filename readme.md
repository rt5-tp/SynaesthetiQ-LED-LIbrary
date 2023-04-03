# README

This requires fixes to the core frequnecy scaling and the SPI Buffer Size.

SPI

When using SPI the led string is the only device which can be connected to the SPI bus. Both digital (I2S/PCM) and analog (PWM) audio can be used.

Many distributions have a maximum SPI transfer of 4096 bytes. This can be changed in /boot/cmdline.txt by appending

    spidev.bufsiz=32768
On an RPi 3 you have to change the GPU core frequency to 250 MHz, otherwise the SPI clock has the wrong frequency.

Do this by adding the following line to /boot/config.txt and reboot:

    core_freq=250
On an RPi 4 you must set a fixed frequency to avoid the idle CPU scaling changing the SPI frequency and breaking the ws281x timings:

Do this by adding the following lines to /boot/config.txt and reboot:

    core_freq=500
    core_freq_min=500
SPI requires you to be in the gpio group if you wish to control your LEDs without root.

