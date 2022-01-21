Simple arduino example to control up to 8out x 1008pix ws2812 led strip with STM32F4 MCU.<br>
Based on DMA with double buffer mode. Dont use cpu time but eat more ram (74kb).<br>
You also need instal stm32 core by st https://github.com/stm32duino/Arduino_Core_STM32<br>
To change pixel color use function setpix(out, pixel, red, green, blue); <br>
tested on stm32f407/427/437/439 
