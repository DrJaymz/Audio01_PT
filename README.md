# Audio01_PT

This is a test project for streaming audio to a TTGO-DISPLAY board with SPI display.

Things I had to do were create a custom partition because the audio tools use a lot of space, and turn off wifi_power_saving because otherwise it seems to harm streaming.

22, 15, 13 are connected to MAX98357A  

The audiotools config needed a local copy to up the buffers and to specify pin13 which is non-standard, I think standard is 14 which this board doesn't have, or its being used for the display.
