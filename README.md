# tidbit
Custom firmware for Tidbit Numpad.

Has 2 Layers/Modes: Default and RGB
- Current layer displayed on OLED display
- To change layers, rapidly tap the rotary encoder twice

Default Layer


![Layer 11](https://user-images.githubusercontent.com/99014835/169425500-d2693378-a636-49e0-bb5b-bcff5cc0cf23.jpg)
- Default numberpad functions
- Rotary encoder serves as Backslash, unless rapidly tapped twice
- Turning rotary encoder changes volume
- Bit-C LED is off when Num-Lock is off, on when Num-Lock is on
- Num-Lock is toggled in the RGB Layer

RGB Layer


![Layer 22](https://user-images.githubusercontent.com/99014835/169425563-b3725d6f-739e-4f0a-9ba6-7008bfc6c15d.jpg)
- Has multiple RGB modes and setting selections
- Turning rotary encoder scrolls up/down
- Subtract key toggles Num-Lock
- Add key launches Calculator app and automatically moves to Default Layer

Build Video
- 
https://www.youtube.com/watch?v=_d1RjwxExso&t=190s

Flashing Instructions
-
1. Install MSYS2 and update its packages: https://www.msys2.org/
2. Download qmk_firmware: https://github.com/qmk/qmk_firmware
3. Download nullbits tidbit firmware: https://github.com/qmk/qmk_firmware/tree/master/keyboards/nullbitsco/tidbit
4. Move the tidbit folder into the qmk_firmware folder, under keyboards
5. Download Mechbits tidbit folder (this repo) and rename as "oledd" so that it doesn't replace the original "oled" file in the nullbits tidbit firmware
6. Move the oledd folder into the keymaps folder of the nullbits tidbit folder (which should now be inside the qmk_firmware)
7. Open MSYS2 MinGW 64-bit (BLUE LOGO)
8. Set-up with commands "pacman --needed --noconfirm --disable-download-timeout -S git mingw-w64-x86_64-toolchain mingw-w64-x86_64-python3-pip", "python3 -m pip install qmk", and "qmk setup"
9. Navigate to the qmk_firmware folder using cd (change directory)
10. When inside the qmk_firmware, use the compile command "qmk compile -kb tidbit -km oledd"
11. (Delete some keyboard folders if MSYS2 has trouble compiling)
12. The tidbit_oledd.hex file has been created in the qmk_firmware folder
13. Download QMK Toolbox: https://github.com/qmk/qmk_toolbox/releases
14. In QMK Toolbox, navigate to the tidbit_oledd.hex file
15. Have MCU set to atmega32u4
16. Connect numpad and put in flashing mode
17. Flash!
