# FencingBox

# 1 Introduction
Low-cost electric fencing scoring box. This circuit is memory optimised and ment to be uploaded to an ATTINY84 et al chip with an Arduino as ISP. Alongside this code, there is a custom PCB whose fabrication files can also be found on this GitHub.   

# 2 Uploading the code to the chip
For a proper guide please visit: https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP.

BURN THE BOOTLOADER TO SET FUSES CORRECTLY. If you dont, clock speeds and other stuff will be wrong (this is particularly crucial for the LED screen as pulse timings need to be very precise). To upload the code to the chip simply select the writer arduino (master) and upload the Arduino as ISP script to it. Next connect the writer arduino to the target chip by connecting the MOSI, MISO, SCK, RESET  (and VCC and GND) pins in their respective pins, i.e. writer MOSI --- target MOSI and so on. Check the following image for the wiring details on the writer Arduino (the target board has a header with these pins labled).

![This is an image](https://github.com/acroscarrillo/FencingBox/blob/main/readme_files/ICSP_explanation.png)

Once the wiring is done BURN THE BOOTLOADER and THEN upload the board's program through the writer arduino.


# 3 Wiring of the different components of the board
## An overview of the board's schematic:

![This is an image](https://github.com/acroscarrillo/FencingBox/blob/main/readme_files/schematic_diagram.PNG)

## A close up of the chip's schematic:

![This is an image](https://github.com/acroscarrillo/FencingBox/blob/main/readme_files/chip_wiring.PNG)

## PCB wiring design:

![This is an image](https://github.com/acroscarrillo/FencingBox/blob/main/readme_files/pcb_wiring_design.jpeg)


## Manufactured PCB:

![This is an image](https://github.com/acroscarrillo/FencingBox/blob/main/readme_files/pcb_picture.jpeg)

# License
For now, this project is an “All Rights Reserved” – no one has any right to use it without my express permission. See `LICENSE.md` file above.
