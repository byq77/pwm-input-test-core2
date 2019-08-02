# core2-mbed-template
Blinking leds project template for [Husarion CORE2](https://husarion.com/manuals/core2/), a board equipped with STM32 Cortex-M4 MCU. 
Project is prepared for development in Visual Studio Code with Mbed CLI tools.

<p align="center"><img width="800px" src="https://cdn.shopify.com/s/files/1/2545/8446/products/CORE2-ROS_1024x1024@2x.png?v=1520001976" alt="CORE2-ROS"/></p>

## Prerequisites
You need following tools:
* [Visual Studio Code IDE](https://code.visualstudio.com/)
* [GNU Arm Embedded version 6 toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* [STM32 ST-LINK Utility](https://www.st.com/en/development-tools/stsw-link004.html) (Windows)
* [stlink flasher](https://github.com/texane/stlink/blob/master/README.md) (Mac/Linux)
* [Mbed CLI](https://os.mbed.com/docs/mbed-os/v5.12/tools/installation-and-setup.html)

### Required Visual Studio Code extensions:
* [Microsoft C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) (`ms-vscode.cpptools`)
* [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) (`marus25.cortex-debug`)

## Mbed CLI installation

To install the tool follow the official documentation:
* [Linux](https://os.mbed.com/docs/mbed-os/v5.12/tools/linux.html)
* [Mac/Windows](https://os.mbed.com/docs/mbed-os/v5.12/tools/installation-and-setup.html)


After installation set the path to the binary directory of your GCC Arm Embedded Compiler installation:

```
$ mbed config -G GCC_ARM_PATH "C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin" 
```
To check current configuration run:

```
$ mbed config --list
```

## Preparing a workspace
Create a new folder `core2-mbed-workspace`. It will serve as workspace for your mbed projects.  Run:

```bash
$ mkdir core2-mbed-workspace && cd core2-mbed-workspace
```
Next step is to import `mbed-os` library. It will be used by all your projects. In your workspace folder run:

```bash
$ mbed import mbed-os
```

Set Mbed OS version to supported by this template:

```bash
$ cd mbed-os
$ mbed update mbed-os-5.12.4
```

During Mbed OS installation you can be asked to install additional python libraries. Switch to `mbed-os` dir and run:

```bash
$ pip install -r requirements.txt --user
```

Set path to `mbed-os` directory in Mbed CLI. These way all your projects can use one instance of the library (default configuration is to have separate instance of library for each project). Run:

```bash
$ mbed config -G MBED_OS_DIR <path to mbed-os>
```

Example:

```bash
$ mbed config -G MBED_OS_DIR "E:\mbed_projects\core2-mbed-workspace\mbed-os"
```

### Adding .mbedignore

In `mbed-os` directory create `.mbedignore` (filename starts with dot) file with following content:

```
features/cellular/*
features/cryptocell/*
features/deprecated_warnings/*
features/lorawan/*
features/lwipstack/*
features/nanostack/*
features/netsocket/*
features/nfc/*
features/unsupported/*
components/wifi/*
components/802.15.4_RF/*
targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407xG/device/TOOLCHAIN_GCC_ARM/STM32F407XG.ld
targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F407xG/device/TOOLCHAIN_GCC_ARM/startup_stm32f407xx.S
```

## How to use firmware

Open Visual Studio Code, press `CTRL + SHIFT + P` and type `Git: Clone` in Command Pallet. Copy and paste the URL to this repo that you will find at GitHub page. 

You will be prompted to select your repo location. Choose `core2-mbed-workspace` directory.

### Updating project files
Open `core2-mbed-template` in VSC. In `.vscode` directory find `settings.json` file and change the value of `C_cpp.default.compilerPath` with path to `arm-none-eabi-g++` location on your system:

Example (Windows):
```json
{
    "C_Cpp.default.compilerPath": "C:/Program Files (x86)/GNU Tools ARM Embedded/6 2017-q2-update/bin/arm-none-eabi-g++"
}
```

### Compilation tasks

To build and flash your firmware press `CTRL + SHIFT + P` and type `Tasks: Run Task` in Command Pallete. Here is the list of available tasks: 
* `BUILD (RELEASE)`
* `BUILD (DEBUG)`
* `FLASH FIRMWARE WHEN BOOTLOADER (RELEASE)`*
* `FLASH FIRMWARE WHEN BOOTLOADER (DEBUG)`  *
* `FLASH FIRMWARE NO BOOTLOADER (RELEASE)`  *
* `FLASH FIRMWARE NO BOOTLOADER (DEBUG)`    *
* `CREATE STATIC MBED-OS LIB (RELEASE)`
* `CREATE STATIC MBED-OS LIB (DEBUG)`
* `BUILD FROM STATIC LIB (RELEASE)`
* `BUILD FROM STATIC LIB (DEBUG)`
* `CLEAN DEBUG`
* `CLEAN RELEASE`

`*` *require ST-LINK programmer*

You can add new tasks and customize existing ones by editing `task.json` file. 

#### Building and uploading firmware (BOOTLOADER)

The software bootloader allows the use of Husarion Cloud. You can find it in `TARGET_CORE2/bootloader_1_0_0_cortex.hex`. The instruction how to flash it can be found [here](https://husarion.com/manuals/core2/#updating-core2-bootloader).

To build firmware use `BUILD (RELEASE)` or `BUILD (DEBUG)` tasks.

To flash firmware connect ST-LINK to debug connector of CORE2 and use `FLASH FIRMWARE WHEN BOOTLOADER (RELEASE)` or `FLASH FIRMWARE WHEN BOOTLOADER (DEBUG)` task.

#### Building and uploading firmware (NO BOOTLOADER)
> Before proceeding with the following steps make sure you conducted mass erase of the memory and made all flash memory sectors are write unprotected.

If you do not want use the bootloader just remove this lines from mbed_app.json:
```json
    "target.mbed_app_start":"0x08010000",
    "target.mbed_rom_start":"0x08000000",
    "target.mbed_rom_size":"0x100000"
```

To build firmware use `BUILD (RELEASE)` or `BUILD (DEBUG)` tasks.

To flash firmware connect ST-LINK to debug connector of CORE2 and use `FLASH FIRMWARE NO BOOTLOADER (RELEASE)` or `FLASH FIRMWARE NO BOOTLOADER (DEBUG)` task.

#### Flashing firmware using `core2-flasher`

```bash
$ arm-none-eabi-objcopy -O ihex firmware.elf firmware.hex 
$ ./core2-flasher firmware.hex
```

You will find `firmware.elf` in `./BUILD/RELEASE` or `./BUILD/DEBUG`.

Here you can learn where to find `core2-flasher` for your system:
https://husarion.com/manuals/core2/#updating-core2-bootloader

#### Flashing firmware using `stm32loader`
https://github.com/byq77/stm32loader

This tool allows you to upload firmware using RPi connector.

If you have the bootloader the first two sectors are write protected. Before uploading new firmware you must unlock them (this will erase the bootloader):

```bash
$ sudo stm32loader -c <your_sbc> -u -W
```

To upload new firmware run:
```bash
$ sudo stm32loader -c <your_sbc> -e -v -w firmware.bin
```

### Debug

To debug:
* make sure you have stlink from texane installed on your system: https://github.com/texane/stlink/blob/master/README.md
* install extension: https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug
* compile and flash DEBUG firmware
* `CTRL + SHIFT + D` and click on `start debug` button

## mbed development

Documentation:
* [MBED OS Documentation](https://os.mbed.com/docs/v5.12/)
* [MBED OS API Doxygen](https://os.mbed.com/docs/v5.12/mbed-os-api-doxy/modules.html)
