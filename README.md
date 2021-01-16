# BlueNRG-1-VSCode
This repo is a template for setting up VSCode to compile and debug code for the EMB-1061 BlueNRG-1 module

## Please note:
This is an ongoing project and there are some issues and jank. Things like printing floats may not work correctly. Please take this into account when you decide to use this chip.

It's still better than paying $1500 a year for licenses to IAR or MDK5 though...

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
