# FencingBox

# 1 Introduction
Low-cost electric fencing scoring box. The firmware is memory optimised and ment to be uploaded to an ATTINY84 et al chip with an Arduino as ISP. Alongside this code, there is a custom PCB whose fabrication files can also be found on this GitHub.   

# 2 Uploading the code to the chip
For a proper guide please visit: https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP.

BURN THE BOOTLOADER TO SET FUSES CORRECTLY. If you dont, clock speeds and other stuff will be wrong (this is particularly crucial for the LED screen as pulse timings need to be very precise). To upload the code to the chip simply select the writer arduino (master) and upload the Arduino as ISP script to it. Next connect the writer arduino to the target chip by connecting the MOSI, MISO, SCK, RESET  (and VCC and GND) pins in their respective pins, i.e. writer MOSI --- target MOSI and so on. Check the following image for the wiring details on the writer Arduino (the target board has a header with these pins labled).

<p align="center">
  <img
    src="https://raw.githubusercontent.com/acroscarrillo/FencingBox/main/readme_files/ICSP_explanation.png"
    alt="ICSP explanation"
    width="700"
  />
</p>

Once the wiring is done BURN THE BOOTLOADER and THEN upload the board's program through the writer arduino.

# 3 PCB schematic

## - An overview of the board's schematic:
<p align="center">
  <img
    src="https://raw.githubusercontent.com/acroscarrillo/FencingBox/main/readme_files/schematic_diagram.PNG"
    alt="Board schematic overview"
    width="700"
  />
</p>

## - A close up of the chip's schematic:
<p align="center">
  <img
    src="https://raw.githubusercontent.com/acroscarrillo/FencingBox/main/readme_files/chip_wiring.PNG"
    alt="Chip wiring close-up"
    width="700"
  />
</p>

# 4 PCB manufacturing

## - PCB wiring design:
<p align="center">
  <img
    src="https://raw.githubusercontent.com/acroscarrillo/FencingBox/main/readme_files/pcb_wiring_design.png"
    alt="PCB wiring design"
    width="600"
  />
</p>

## - Manufactured PCB:
<p align="center">
  <img
    src="https://raw.githubusercontent.com/acroscarrillo/FencingBox/main/readme_files/pcb_picture.jpeg"
    alt="Manufactured PCB"
    width="600"
  />
</p>

# 5 Product design
Let's see the final product design, in this case I went for a simple 32x32 matrix LED screen.
## - Screen hinge prototype
The screen on a hinge posed the classic problem of what to do with the wires, here's a small prototype I made of the hinge and the wires passing through.
<p align="center">
  <img
    src="https://raw.githubusercontent.com/acroscarrillo/FencingBox/main/readme_files/hinge.png"
    alt="Hinge"
    width="600"
  />
</p>

## - Final product
Final design with the fencing weapon ports on each side, laptop-like screen design and buttons on the back. The choice for the buttons on the back comes from a variety of reasons: first they get covered when stored to avoid accidental operation and second, it allows for them to be big and placed against the ground for easy manipulation with the weapon tip, removing the need of using the hand (inconvient in fencing gear).
<p align="center">
  <img
    src="https://raw.githubusercontent.com/acroscarrillo/FencingBox/main/readme_files/finished_box.png"
    alt="Final product"
    width="600"
  />
</p>


# License
For now, this project is an “All Rights Reserved” – no one has any right to use it without my express permission. See `LICENSE.md` file above.
