# BlueNRG-1-VSCode
This repo is a template for setting up VSCode to compile and debug code for the EMB-1061 BlueNRG-1 module

Watch this video for an introduction to this project

[![Watch this video](https://img.youtube.com/vi/VH6JlEohDSQ/0.jpg)](https://youtu.be/VH6JlEohDSQ)

## Please note:
This is an ongoing project and there are some issues and jank. Things like printing floats may not work correctly. Please take this into account when you decide to use this chip.

It's still better than paying $1500 a year for licenses to IAR or MDK5 though...

## Features

- OTA
- Low Power (multiple years on a coin cell)

## Setup
1. [Fork this template](https://github.com/ctjet/BlueNRG-1-VSCode/generate) for your own project and clone to your computer
2. Download and install VSCode and open your cloned project
3. Install the Cortex-Debug extension by marus25
    1. Search marus25.cortex-debug in the extensions menu and install
4. Install the [Arm Cortex Debug Tools](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
5. In your opened project in VSCode, open and modify .vscode/settings.json as follows:
    1. In the integrated terminal run the getShort.bat with your Arm Cortex Debug Tools version's bin file location as the argument, for example:
        `./getshort.bat "C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2020-q4-major\bin"`
    2. Copy the output and put it into the "cortex-debug.armToolchainPath" setting, for example:
        '"cortex-debug.armToolchainPath": "C:/PROGRA\~2/GNUARM\~1/102020~1/bin",'
    3. Open your Arm Cortex Debug Tools version's file location in explorer and navigate to \\lib\\gcc\\arm-none-eabi and copy the name of the file at that location. 
        - My full path was C:\\Program Files (x86)\\GNU Arm Embedded Toolchain\\10 2020-q4-major\\lib\\gcc\\arm-none-eabi
        - The file name I copied was 10.2.1
    4. Replace the setting "armToolchainVersion" with the file name you copied, for example:
        `"armToolchainVersion": "10.2.1",`
    5. (Optional if using JLink) Copy your JLink Server's install location into the setting "cortex-debug.JLinkGDBServerPath", for example:
        '"cortex-debug.JLinkGDBServerPath": "C:/Program Files (x86)/SEGGER/JLink_V635c/JLinkGDBServerCL.exe"'
6. Install the [BlueNRG-1_2 DK](https://www.st.com/content/st_com/en/products/embedded-software/evaluation-tool-software/stsw-bluenrg1-dk.html)
    1. If building the project doesn't work, you may need to update the settings.json with the file location. Check and make sure that the  "bluenrgDkLocation" setting's value is where your BlueNRG DK installed. If it wasn't, find it and run the getshort.bat with its lib file location.
        - For example, my install was located in `C:\Users\<USER>\ST\BlueNRG-1_2 DK 3.2.1\Library` so my setting looks like `"bluenrgDkLocation": "C:/Users/<USER>/ST/BLUENR~1.1/Library"`

You should be done! 

## Building and running the example project
1. Press ctrl+shift+b to build the project, if everything is set up, it should build!
2. Navigate to the run menu on the left and set your debug configuration to "Debug (ST-Link)" or "Debug (JLink)" depending on which debug device you are using
    - You will also need to connect your debug device to your ST-Link or JLink using the SWDIO, SWCLK, and GND pins. **NOTE:** I do not recommend using the 3.3V line to power your BlueNRG module, it causes errors. Please use an external power supply or coin cell between 1.7 to 3.6 V
3. Click run or press f5, a debug window should pop up. Please note that this also executes the make task, so you do not need to press ctrl+shift+b every time you want to build and upload. To modify this behavior, edit the  .vscode/launch.json file
4. You should be able to step through your program, or click continue to let it run. When running it should blink the LED (GPIO_Pin_14 on my dev board) and also become a BLE Beacon. You should be able to see the BLE device through a BLE sniffer on your phone

## File locations explanation

Here is the location of where the BlueNRG-1 DK is from https://www.st.com/content/st_com/en/products/embedded-software/evaluation-tool-software/stsw-bluenrg1-dk.html When you install this the actual location of the DK files should be `C:/Users/<your user>/ST/BlueNRG-1_2 DK 3.2.1`

## BlueNRG-1 Dev Board

### You can support this project by picking up a Development Board from [here](https://www.tindie.com/products/22570/)

![pinout](/images/pinout.svg)

Design files can be found [here](https://oshwlab.com/cmarting99/discutil_copy)


## Troubleshooting
