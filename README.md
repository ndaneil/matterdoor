# <img src="./logo.png" height="26" /> MatterDoor - Smart do not disturb sign

<div align="center"><img src="./imgs/cover.png" width="80%"></div>

This project is also an entry to the [Make it Matter!](https://www.hackster.io/contests/makeitmatter) contest organized by Hackster.io and Nordic Semiconductor. My entry is available here: [https://www.hackster.io/ndaniel/smart-do-not-disturb-sign-1a102f](https://www.hackster.io/ndaniel/smart-do-not-disturb-sign-1a102f). The interactive version of this document, which includes the source code and 3D design files too, is available here: [https://github.com/ndaneil/matterdoor/](https://github.com/ndaneil/matterdoor/)

A problem I encountered during the pandemic attending online lectures was that my family did not know when I had my online classes. They would enter the room during lectures or tests. The problem is also present when working from home. People walking in when attending meetings was something everyone had to get used to. **But there is a solution!** If you have visited a radio station or saw one in a movie, you must have seen the large ON AIR signs which light up signaling you to be cautious. I have made one for myself during the pandemic to signal when not to enter. This project is a better version of that prototype. It has cool animations on the five-row RGB display in the front of the device. The brain of MatterDoor is the nRF7002DK, Nordic Semiconductor's latest Matter-capable development kit. MatterDoor can be commissioned to  and controlled via a custom Matter fabric using the Android app in this repository, or any existing Matter-compatible ecosystem. Say goodbye to interrupted meeting and lectures.

## Matter and SDK

[Matter](https://csa-iot.org/all-solutions/matter/) is a new application-layer protocol developed by members of the Connectivity Stands Alliance aiming to be the industry standard protocol for all IoT devices. Both Google and Apple pledged to support it for their smart home ecosystems. An important feature is the ability of Matter-compatible devices to be commissioned into multiple ecosystems (called Matter fabrics), allowing the end user the freedom to choose their ecosystem without having to worry whether the device will support it. Additionally, simultaneous control from separate ecosystems is also possible. 

This project was the first time I used the nRF SDK. I was happy to see that Nordic offers an [nRF Connect SDK Fundamentals](https://academy.nordicsemi.com/courses/nrf-connect-sdk-fundamentals/) course, which I have completed, learning about the SDK, the principles of low-level parallelism, multithreading and Zephyr RTOS. After completing the course, I have also earned a certificate.

 I highly recommend this course. Additionally, I have watched the following webinars about Matter:
 - [Introduction to Matter](https://youtu.be/v_285vCHifw)
 - [Developing Matter 1.0 products with nRF Connect SDK](https://youtu.be/9Ar13rMxGIk)
 - [How to go to market with Matter](https://youtu.be/ZV6fjTLAqdA)

To get familiar with the concepts discussed here, I recommend watching at least the first of these webinars.

## Hardware

I applied for free hardware with my idea and got the nRF7002DK from Nordic Semiconductor. I decided to use the form factor of the development kit for the sign. I have considered several display technologies:
 - E-ink has the advantages of low power consumption and vibrant colors in bright light. But for a do not disturb sign, we need a way to get the attention of the person approaching the door, so display technologies emitting light are more suitable for that purpose.
 - LCD displays offer high resolution while still being visible in the dark.
 - OLED displays have a great viewing angle, but are more expensive and the form factors are smaller.
 - Programmable LEDs offer a great way for custom displays, but the light needs to be diffused.

In the end, I decided to use WS2812B programmable LEDs for the display part since I could design the display to match the dimensions of the nRF7002DK. It also meant that I could design some cool animations to be run on the display. To diffuse the light, I used tubes designed for LED strips.

### BOM

Apart from the 3D printed housing, the most important components are the WS2812B LED strip, the tube and the logic level shifter needed since the nRF3540 runs on 1.8V logic level in the nRF7002Dk:

<div align="center"><img src="./imgs/components.jpg" width="50%"></div>

The following list contains all the components needed:
 - Wires
 - One jumper wire
 - 500R resistor
 - 3K3 resistor
 - TXS0108E logic level shifter (or equivalent)
 - Tube for LED strip, 1m
 - WS2812B LED strip 96 pixels/leds/m, 1m
 - Perfboard
 - 1x8 pin header

#### 3D-printed parts

The housing I designed consists of four major parts and an optional switch cover. **The STL files of the designed casing are available [here](./3dfiles).** GitHub has a nice 3D viewer, but here are some screenshots from Shapr3D, the software I used to design the casing:

<div align="center"><img src="./imgs/3d-side.PNG" width="50%"><img src="./imgs/3d-fromtop.PNG" width="50%"></div>
<div align="center"><img src="./imgs/3d-side2.PNG" width="50%"><img src="./imgs/3d-fromabove.PNG" width="50%"></div>

The bottom part of the casing will house the nRF7002DK, while the top part will hold the LED tubes. **[The assembled 3D model can be viewed here.](./3dfiles/matterdoor-combined-do-not-use.stl)**

### Assembly

For the electronics-side, the components need to be soldered together based on the following schematic:

<div align="center"><img src="./imgs/schematic.png" width="50%"></div>

The 8 pins grouped together on the left need to be connected to a header in the nRF7002DK and the `P1.13` label marks the wire that needs to be connected to that pin on the board:

<div align="center"><img src="./imgs/nrf7002dk-connect.jpg" width="80%"></div>

**Keep in mind that VDD is 1.8V.** So using the correct ordering of the pins on the header is important, it has to be connected in that exact order. The wiring is shown below:

<div align="center"><img src="./imgs/wiring1.jpg" height="200"><img src="./imgs/wiring2.jpg" height="200"></div>

The three wires leaving the perfboard are to be connected to the LED strip. 

#### Creating the display

Cut five, 12.5cm-long sections from the tube and cut five 12-pixel-long sections from the LED strip. The next step is placing the cut strip parts in alternate orientation parallel to each other. Soldering wires to the `GND` and `5V` pins on one side of the strips is the next step, then connecting these and joining it to the `5V` and `GND` wires leaving the perfboard. The `DIN` wire from the perfboard needs to be connected to the input of the first strip section. Placing the LEDs in the tubes is the next step, followed by soldering the data wires between the strips the following way:

<div align="center"><img src="./imgs/rows-wiring.jpg" width="80%"></div>

The next step is putting double-sided tape on the top cover. It may not be strong enough, so some other glue can be used as well:

<div align="center"><img src="./imgs/top-tape.jpg" width="80%"></div>

After securing the tubes on the top plate, the two side pieces can be slid over from the sides. The three wires leaving the top panel need to be routed through one of the two holes in the top panel.

<div align="center"><img src="./imgs/pillars.jpg" width="80%"></div>

<div align="center"><img src="./imgs/halfassembled.jpg" width="80%"></div>

<div align="center"><img src="./imgs/top-assembled.jpg" width="80%"></div>

The assembly of the top part is then complete. Two additional wires can be soldered to the `5V` and `GND` pins so that power can be supplied directly instead of through the Micro USB port. The perfboard can be covered up with hot glue. An important part is to cover up the side close to the nRF7002DK board. Putting a piece of duct tape there is needed to prevent potential short circuits.

<div align="center"><img src="./imgs/connected.jpg" width="80%"></div>

The last step is assembling the top and bottom parts and the hardware-part is complete:

<div align="center"><img src="./imgs/assembled-side.jpg" width="80%"></div>

<div align="center"><img src="./imgs/assembled-top.jpg" width="80%"></div>

## Software

There were three main challenges with the software:
1. Get the LED strip to work with the nRF7002DK
2. Modifying the Matter template
    - Using ZAP was challenging
    - Long compile times before linking fails
3. Controlling the device from my phone
    - In the end, I chose to modify Google's [Google Home Mobile SDK Sample Applications for Matter](https://github.com/google-home/sample-apps-for-matter-android/tree/main) to create my own Matter fabric

### Controlling the LEDs

As someone familiar with the Arduino framework, I got used to having lots of libraries and examples for peripherals. Controlling WS2812B LEDs just worked with every microcontoller I have tried before. Even with 3.3V logic level microcontrollers like the ESP32, running a [FastLED](https://github.com/FastLED/FastLED) or a [NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) example code worked out of the box. Since the nRF7002DK is a relatively new board, so far the [official Zephyr repo](https://github.com/zephyrproject-rtos/zephyr/tree/d5a1a8bf04c9a4a1f7b9c73f2426368e93844143/samples/drivers/led_ws2812/boards) did not support it. As a result, I looked for examples elsewhere. The only example I could find was a [hackster.io project](https://www.hackster.io/mahmood-ul-hassan/how-to-interface-nordic-thingy-53-with-neopixels-ws2812b-c79533). As shown in the hardware section, the board has 1.8V logic level, which makes a logic level shifter necessary. Even though I added one, I could not get the example to work. I tried modifying the official example from the Zephyr repo to work with the nRF53 and I managed to make it work, but it was unstable. I could control the first couple of LEDs, but afterwards, the LEDs just showed random colors. I adjusted several config options, but the display was still unstable. It was at that point that I decided to [raise a ticket in Nordic DevZone](https://devzone.nordicsemi.com/f/nordic-q-a/105200/ws2812-driver-for-nrf5340) to ask if there was an official example. Thanks to Kenneth from Nordic, I started investigating if the level shifting was working correctly. I switched to using the `TXS0108E` shown in the hardware section, but I still could not get a stable output. The final solution was to add the two resistors, one to pull high the Output Enable pin of the level shifter, the other to add resistance between the first LED's input and the bi-directional level shifter's high side. With these, controlling the display works flawlessly. In the spirit of open source, I created a pull request in the Zephyr repository to add support for nRF53-based boards (including nRF7002DK) in the WS2812 example: [https://github.com/zephyrproject-rtos/zephyr/pull/64823](https://github.com/zephyrproject-rtos/zephyr/pull/64823)

#### The solution

The LEDs are controlled over the I2S interface of the nRF5340. The overlay file for the board is the following:
``` c
#include <zephyr/dt-bindings/led/led.h>

&pinctrl {
	i2s0_default_alt: i2s0_default_alt {
		group1 {
			/* Default I2S config for the nRF5340, P1.13 is the output */
			psels = <NRF_PSEL(I2S_SCK_M, 1, 15)>,
				<NRF_PSEL(I2S_LRCK_M, 1, 12)>,
				<NRF_PSEL(I2S_SDOUT, 1, 13)>,
				<NRF_PSEL(I2S_SDIN, 1, 14)>;
		};
	};
};

i2s_led: &i2s0 {
	status = "okay";
	pinctrl-0 = <&i2s0_default_alt>;
	pinctrl-names = "default";
};

/ {
	led_strip: ws2812 {
		compatible = "worldsemi,ws2812-i2s";

		i2s-dev = < &i2s_led >;
		chain-length = <60>; /* arbitrary; change at will */
		color-mapping = <LED_COLOR_ID_GREEN
					LED_COLOR_ID_RED
					LED_COLOR_ID_BLUE>;
		reset-delay = <500>;
	};

	aliases {
		led-strip = &led_strip;
	};
};
```

The I2S device is configured with the default pinout. From the four pins, `I2S_SDOUT` is the important one, which is the data output. Controlling WS2812B LEDs is achieved over a single wire, which means that the timing of the signals is crucial. With the I2S interface, a fixed clock frequency is provided and then the output bits are set in a way to 'emulate' as if the pin was set for the required interval to the correct logic level by the bits output to the `I2S_SDOUT` pin. 

The `led_strip` needs to be added to the devicetree. Here, the important parameters are the `reset-delay` and the `chain-length`. A major difference between the WS2812 and WS2812B(more common) LEDs is that the latter require at least a 500us reset time. Though it may work with less time, it is better to follow the specification. The `chain-length` should be set to the length of the LED strip. In our case, that is $5 \cdot 12 = 60$. A WS2812B LED at full brightness can consume 60mA, which would result in a 3.6A current draw if all LEDs would be turned to full brightness. I could not find what is the maximal current output the nRF7002DK can handle through 5V pin, but since the USB 2.0 specification has a 500mA limit (though it is usually possible to draw up to 2A), I soldered in an external connector to be able to power the device from a separate 5V source. Additionally, when designing the animations on the display, an important consideration was this limit not to overload the voltage supply of the board. 

In addition to configuring the drivers, the following config parameters are also needed:

```
CONFIG_SPI=n

CONFIG_I2S=y
CONFIG_WS2812_STRIP=y
CONFIG_WS2812_STRIP_I2S=y
```
Since in the official WS2812 Zephyr example, SPI is the default device, that needs to be turned off and I2S on. The LED driver also needs to be enabled and the I2S mode needs to be set. With these parameters, it is possible to control the LEDs from the nRF7002DK.

### Modifying the Matter template

In the [Developing Matter 1.0 products with nRF Connect SDK](https://youtu.be/9Ar13rMxGIk?t=2053) webinar, there was an example on how to use the Matter-template project and modify it to be a simple Matter light switch. I tried to follow that tutorial, but I encountered a strange issue at the ZAP step. ZAP is the program that would need to be used to create the Matter endpoint configuration. But there was a problem: whenever I would edit the configuration, it would show the zigbee configuration:
``` bash
zap ./src/template.zap
```

<div align="center"><img src="./imgs/zap-zigbee.png" width="80%"></div>

And there were no options to add Matter-specific endpoints. Then after saving the file, lots of lines were removed from the `template.zap` file. (I could see this from the diff). And the example that I could previously compile no longer worked. After tedious debugging, I found what was going on. In the [source code of the ZAP generate script](https://github.com/project-chip/connectedhomeip/blob/master/scripts/tools/zap/generate.py#L87-L108) we have the following function for detecting the ZCL config files:

```python
def detectZclFile(zapFile):
    print(f"Searching for zcl file from {zapFile}")

    prefix_chip_root_dir = True
    path = 'src/app/zap-templates/zcl/zcl.json'

    data = json.load(open(zapFile))
    for package in data["package"]:
        if package["type"] != "zcl-properties":
            continue

        prefix_chip_root_dir = (package["pathRelativity"] != "resolveEnvVars")
        # found the right path, try to figure out the actual path
        if package["pathRelativity"] == "relativeToZap":
            path = os.path.abspath(os.path.join(
                os.path.dirname(zapFile), package["path"]))
        elif package["pathRelativity"] == "resolveEnvVars":
            path = os.path.expandvars(package["path"])
        else:
            path = package["path"]

    return getFilePath(path, prefix_chip_root_dir)
```
Note that in the case of using `relativeToZap`, the path is joined to the path of the `zapFile`. In the [matter template](https://github.com/nrfconnect/sdk-nrf/tree/main/samples/matter/template) the [template.zap](https://github.com/nrfconnect/sdk-nrf/blob/main/samples/matter/template/src/template.zap) file contains the following section:

``` json
 "package": [
    {
      "pathRelativity": "relativeToZap",
      "path": "../../../../../modules/lib/matter/src/app/zap-templates/app-templates.json",
      "type": "gen-templates-json",
      "version": "chip-v1"
    },
    {
      "pathRelativity": "relativeToZap",
      "path": "../../../../../modules/lib/matter/src/app/zap-templates/zcl/zcl.json",
      "type": "zcl-properties",
      "category": "matter",
      "version": 1,
      "description": "Matter SDK ZCL data"
    }
  ],
  ```
We can see that both the `zcl.json` and `app-templates.json` files are referenced relative to the location of the `template.zap` file. This was an issue for me since on my computer, I installed the nRF tools to `/opt/nordic/` and the project was located in a folder in my home directory. Due to this, neither the `generate.py` script, nor the ZAP tool could find these files. The paths needed to be modified in `template.zap` to the correct values:
``` json
  "package": [
    {
      "pathRelativity": "absolute",
      "path": "/opt/nordic/ncs/v2.5.0/modules/lib/matter/src/app/zap-templates/zcl/zcl.json",
      "type": "zcl-properties",
      "category": "matter",
      "version": 1,
      "description": "Matter SDK ZCL data"
    },
    {
      "pathRelativity": "absolute",
      "path": "/opt/nordic/ncs/v2.5.0/modules/lib/matter/src/app/zap-templates/app-templates.json",
      "type": "gen-templates-json",
      "version": "chip-v1"
    }
  ],
```
Note that `pathRelativity` is changed to `absolute` which is not one of options in the `generate.py` source code, meaning that the path will be used as is, resulting in an absolute path. After opening the zap tool from the terminal, the Matter-related configurations were correctly loaded and I was able to add a new endpoint:

<div align="center"><img src="./imgs/zap-matter.png" width="80%"></div>

One thing to keep in mind is that after saving the file, the ZAP tool overwrites the paths to be relative again! 

I added a `Matter extended color light` endpoint, but later turned off the color-related clusters. Apart from the `Identify` and `On/Off` clusters, the `Level Control` cluster was also used. This cluster will be the one controlling the state of the display. For the do not disturb sign, there are three states that need to be distinguished:
 - `OFF ` - Turning off the display to save power
 - `FREE` - Actively signal that it is okay to enter
 - `BUSY` - Signal not to enter

After editing and saving the `template.zap` file, the next step is to generate the files. In the code below, I replaced the absolute path to the folder of my Matter template project with `<absolute-path-to-project-folder>`.

 ``` bash
 python3 /opt/nordic/ncs/v2.5.0/modules/lib/matter/scripts/tools/zap/generate.py  <absolute-path-to-project-folder>/src/template.zap -t /opt/nordic/ncs/v2.5.0/modules/lib/matter/src/app/zap-templates/app-templates.json -o <absolute-path-to-project-folder>/src/zap-generated
 Searching for zcl file from <absolute-path-to-project-folder>/src/template.zap
🔧 Using temporary state directory: /var/folders/17/r70j3wcn3l3cyl1bpnsstz600000gn/T/zap.VwzxeJ
🤖 ZAP generation started: 
    🔍 input files: <absolute-path-to-project-folder>/src/template.zap
    🔍 output pattern: <absolute-path-to-project-folder>/src/zap-generated
    🔍 using templates: /opt/nordic/ncs/v2.5.0/modules/lib/matter/src/app/zap-templates/app-templates.json
    🔍 using zcl data: /opt/nordic/ncs/v2.5.0/modules/lib/matter/src/app/zap-templates/zcl/zcl.json
    🔍 zap version: ver. 2023.9.28, featureLevel 98, commit: cca76eb7c7e3eebff0056bff2da907a582b0848b from 2023-09-28T16:17:00.000Z, mode: binary, exe: zap-cli
🕐 Setup time: 794ms 
👉 using input file: <absolute-path-to-project-folder>/src/template.zap
👉 using output destination: <absolute-path-to-project-folder>/src/zap-generated
🕐 File loading time: 119ms
🤖 Generating files:
    ✍  <absolute-path-to-project-folder>/src/zap-generated/callback-stub.cpp
    ✍  <absolute-path-to-project-folder>/src/zap-generated/CHIPClusters.h
    ✍  <absolute-path-to-project-folder>/src/zap-generated/gen_config.h
    ✍  <absolute-path-to-project-folder>/src/zap-generated/IMClusterCommandHandler.cpp
    ✍  <absolute-path-to-project-folder>/src/zap-generated/PluginApplicationCallbacks.h
    ✍  <absolute-path-to-project-folder>/src/zap-generated/CHIPClientCallbacks.h
    ✍  <absolute-path-to-project-folder>/src/zap-generated/access.h
    ✍  <absolute-path-to-project-folder>/src/zap-generated/endpoint_config.h
🕐 Generation time: 864ms 
clang-format error: [Errno 2] No such file or directory: 'clang-format'
Files generated in: <absolute-path-to-project-folder>/src/zap-generated
 ```
Afterwards, we can see that in the `src/zap-generated/` folder, the files were updated. Now the next step is adding the LED controller and animation logic, and configuring the handlers for the clusters that were added.

### Adding WS2812B drivers
The first step is to modify the `Kconfig` file as shown below. The `mainmenu` string can be freely modified. The important change is to modify the `I2S` device to be enabled by default:

```diff
  #
  # Copyright (c) 2022 Nordic Semiconductor
  #
  # SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
  #
- mainmenu "Matter nRF Connect Template Example Application"
+ mainmenu "MatterDoor"

  # Sample configuration used for Thread networking
  if NET_L2_OPENTHREAD

  choice OPENTHREAD_NORDIC_LIBRARY_CONFIGURATION
    default OPENTHREAD_NORDIC_LIBRARY_MTD
  endchoice

  choice OPENTHREAD_DEVICE_TYPE
    default OPENTHREAD_MTD
  endchoice

  endif # NET_L2_OPENTHREAD

  source "${ZEPHYR_BASE}/../modules/lib/matter/config/nrfconnect/chip-module/Kconfig.features"
  source "${ZEPHYR_BASE}/../modules/lib/matter/config/nrfconnect/chip-module/Kconfig.defaults"
  source "Kconfig.zephyr"
+
+ config I2S
+   default y
```

Then the next step is adding the configuration in the `nrf7002dk_nrf5340_cpuapp.overlay` file:

```diff
  /*
  * Copyright (c) 2021 Nordic Semiconductor ASA
  *
  * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
  */

  #include <zephyr/dt-bindings/ipc_service/static_vrings.h>
+ #include <zephyr/dt-bindings/led/led.h>


  / {
    chosen {
      nordic,pm-ext-flash = &mx25r64;
    };
+     led_strip: ws2812 {
+     compatible = "worldsemi,ws2812-i2s";
+
+     i2s-dev = < &i2s_led >;
+     chain-length = <60>;
+     color-mapping = <LED_COLOR_ID_GREEN
+           LED_COLOR_ID_RED
+           LED_COLOR_ID_BLUE>;
+     reset-delay = <1000>;
+   };
+   aliases {
+     led-strip = &led_strip;
+   };
  };

  /* Set IPC thread priority to the highest value to not collide with other threads. */
  &ipc0 {
      zephyr,priority = <0 PRIO_COOP>;
  };

+ i2s_led: &i2s0 {
+   status = "okay";
+   pinctrl-0 = <&i2s0_default_alt>;
+   pinctrl-names = "default";
+ };
+
+ i2s_pinconf: &pinctrl {
+   i2s0_default_alt: i2s0_default_alt {
+     group1 {
+       psels = <NRF_PSEL(I2S_SCK_M, 1, 15)>,
+         <NRF_PSEL(I2S_LRCK_M, 1, 12)>,
+         <NRF_PSEL(I2S_SDOUT, 1, 13)>,
+         <NRF_PSEL(I2S_SDIN, 1, 14)>;
+     };
+   };
+ };
+ &gpio1 {
+   status = "okay";
+ };
```

The changes to the overlay file are the same as the one introduced in the section about controlling the LEDs. And similarly as there, the `prj.conf` file also needs some modifications:

```diff
  #
  # Copyright (c) 2021 Nordic Semiconductor ASA
  #
  # SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
  #

  # This sample uses Kconfig.defaults to set options common for all
  # samples. This file should contain only options specific for this sample
  # or overrides of default values.

  # Enable CHIP
  CONFIG_CHIP=y
  CONFIG_CHIP_ENABLE_PAIRING_AUTOSTART=y
  CONFIG_CHIP_PROJECT_CONFIG="src/chip_project_config.h"
  # 32768 == 0x8000 (example Product ID added temporaly,
  # but it must be changed with proper PID from the list:
  # https://github.com/project-chip/connectedhomeip/blob/482e6fd03196a6de45465a90003947ef4b86e0b1/docs/examples/discussion/PID_allocation_for_example_apps.md)
- CONFIG_CHIP_DEVICE_PRODUCT_ID=32768
+ # Lighting 0x8005
+ CONFIG_CHIP_DEVICE_PRODUCT_ID=32773
  CONFIG_STD_CPP14=y

  # Add support for LEDs and buttons on Nordic development kits
  CONFIG_DK_LIBRARY=y

  # Bluetooth Low Energy configuration
- CONFIG_BT_DEVICE_NAME="MatterTemplate"
+ CONFIG_BT_DEVICE_NAME="MatterDoor"

  # Other settings
  CONFIG_THREAD_NAME=y
  CONFIG_MPU_STACK_GUARD=y
  CONFIG_RESET_ON_FATAL_ERROR=n
  CONFIG_CHIP_LIB_SHELL=y

  # Disable NFC commissioning
  CONFIG_CHIP_NFC_COMMISSIONING=n

  # Reduce application size
  CONFIG_USE_SEGGER_RTT=n
+
+ CONFIG_SPI=y
+ CONFIG_I2S=y
+ CONFIG_LED_STRIP=y
+ CONFIG_WS2812_STRIP=y
+ CONFIG_WS2812_STRIP_I2S=y
```

The product ID (PID) is also modified, set to `0x8005`. This config is also used to select the correct certificate belonging to the vendor and product IDs (VID and PID). A built-in certificate is used in this case since the PID falls within the range `0x8000`-`0x801F`. For a final product, a proper certificate should be used. More information is available about this in the [How to go to market with Matter](https://youtu.be/ZV6fjTLAqdA) webinar. For this demo project, it is fine to use the builtin certificate. 

A less important, optional change is the modification of the `sample.yaml` file, which is used for tests, checking the `devicetree`:
```diff
  sample:
-   description: Matter Template sample
+   description: Matter Door Status
    name: Matter Template
  tests:
    # Excluded in quarantine.yaml to limit resources usage in integration builds
        - nrf5340dk_nrf5340_cpuapp
        - nrf7002dk_nrf5340_cpuapp
      platform_allow: nrf52840dk_nrf52840 nrf5340dk_nrf5340_cpuapp nrf7002dk_nrf5340_cpuapp
+   sample.drivers.led.ws2812:
+     tags: LED
+     filter: dt_compat_enabled("worldsemi,ws2812-i2s")
+     harness_config:
+       fixture: fixture_led_ws2812
```

### Adding the logic

Now we turn our attention to the `src` folder's contents. Here is where the main logic will be. We have added the `On/Off`, the `Level Control` and the `Identify` endpoints to the `Endpoint - 1` cluster. (A `Descriptor` cluster is also added, but that will not be implemented for now.) The first changes relating to the `Identify` cluster are the same as shown in the [Developing Matter 1.0 products with nRF Connect SDK](https://youtu.be/9Ar13rMxGIk) webinar. In `app_event.h`, two new values need to be added to the `AppEventType` enum:

```diff
  class LEDWidget;

- enum class AppEventType : uint8_t { None = 0, Button, ButtonPushed, ButtonReleased, Timer, UpdateLedState };
+ enum class AppEventType : uint8_t { None = 0, Button, ButtonPushed, ButtonReleased, Timer, UpdateLedState, IdenfityStart, IdentifyStop,};

  enum class FunctionEvent : uint8_t { NoneSelected = 0, FactoryReset };
```

Next, in the `app_task.h`, there are multiple changes needed:

```diff
  /*
  * Copyright (c) 2021 Nordic Semiconductor ASA
  *
  * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
  */

  #pragma once

  #include "app_event.h"
  #include "led_widget.h"

  #include <platform/CHIPDeviceLayer.h>

  #if CONFIG_CHIP_FACTORY_DATA
  #include <platform/nrfconnect/FactoryDataProvider.h>
  #else
  #include <platform/nrfconnect/DeviceInstanceInfoProviderImpl.h>
  #endif

+ #include <app/clusters/identify-server/identify-server.h>
+
+ #include <app/clusters/on-off-server/on-off-server.h>
+ #include <app/clusters/level-control/level-control.h>
+
  struct k_timer;

  class AppTask {
  public:
    static AppTask &Instance()
    {
      static AppTask sAppTask;
      return sAppTask;
    };

+    
+   enum DispState {
+     OFF = 0,
+     FREE = 1,
+     BUSY = 2
+   };
+
    CHIP_ERROR StartApp();

    static void PostEvent(const AppEvent &event);
+   void setState(DispState newState);
+   static void dispThread(void);
+   DispState currentState = OFF;
+

  private:
    CHIP_ERROR Init();

    void CancelTimer();
    void StartTimer(uint32_t timeoutInMs);

    static void DispatchEvent(const AppEvent &event);
    static void UpdateLedStateEventHandler(const AppEvent &event);
    static void FunctionHandler(const AppEvent &event);
    static void FunctionTimerEventHandler(const AppEvent &event);

    static void ChipEventHandler(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg);
    static void ButtonEventHandler(uint32_t buttonState, uint32_t hasChanged);
    static void LEDStateUpdateHandler(LEDWidget &ledWidget);
    static void FunctionTimerTimeoutCallback(k_timer *timer);
    static void UpdateStatusLED();

+   static void IdentifyStartHandler(Identify *identify);
+   static void IdentifyStopHandler(Identify *identify);
+
+   static void OnOffEffectHandler(OnOffEffect * effect);
+   static void clearAll();
+   static void set(int x, int y, int r, int g, int b);
+   static void drawX(int offset100, int brightness);
+   static void drawFree(int offset100, int brightness);

    FunctionEvent mFunction = FunctionEvent::NoneSelected;
    bool mFunctionTimerActive = false;

+   static Identify sIdentify;
+   static OnOffEffect sOnOffEffect;

  #if CONFIG_CHIP_FACTORY_DATA
    chip::DeviceLayer::FactoryDataProvider<chip::DeviceLayer::InternalFlashFactoryData> mFactoryDataProvider;
  #endif

```

Some include directives are added, relating to the clusters added earlier to the zap file. A new enum is introduced (`DispState`), which will be used to sync the state of the sign between the Matter fabric and the MatterDoor device. Several other functions are added and two static variables for the handlers. The implementations of all of these functions are shown and explained below.

The `app_task.cpp` file starts with the following macros and declarations:

```diff
  #include <zephyr/kernel.h>
  #include <zephyr/logging/log.h>

+ #include <app-common/zap-generated/attributes/Accessors.h>
+ #include <errno.h>
+ #include <string.h>
+ #include <zephyr/drivers/led_strip.h>
+ #include <zephyr/device.h>
+ #include <zephyr/drivers/i2s.h>
+ #include <zephyr/sys/util.h>
+ #include <zephyr/sys/printk.h>
+ #define STRIP_NODE		DT_ALIAS(led_strip)
+ #define STRIP_NUM_PIXELS	DT_PROP(DT_ALIAS(led_strip), chain_length)
+ #define DELAY_TIME K_MSEC(6)
+ #define RGB(_r, _g, _b) { .r = (_r), .g = (_g), .b = (_b) }
+ #define DISPLAY_TASK_PRIORITY 1
+ static struct led_rgb pixels[STRIP_NUM_PIXELS];
+ static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);
+ K_THREAD_DEFINE(disp_thread, 2048, AppTask::dispThread, NULL, NULL, NULL, DISPLAY_TASK_PRIORITY, 0, 0);
```

After the includes, we get the `STRIP_NODE` from the `devicetree` and the number of pixels of the LED strip from that node. A 6 millisecond delay time is defined, which will be used for the delay between two frames of the `BUSY` and `FREE` state animations. We also define a more convenient way to use RGB colors. The `DISPLAY_TASK_PRIORITY` is used for the new thread we are defining in the last line of the above code. This thread will be responsible for displaying the animations on the five-row WS2812B display. We also define the `pixels` variable which will be used to set the colors of the strip. The `strip` variable will be used to control the the LED strip. 

Let's check the newly created `dispThread()` function next:

```c
void AppTask::dispThread(void){
	int rc;
	printk("Display thread started\n");
	if (device_is_ready(strip)) {
		LOG_INF("Found LED strip device %s", strip->name);
	} else {
		LOG_ERR("LED strip device %s is not ready", strip->name);
		return;
	}
	LOG_INF("Displaying pattern on strip");
	memset(&pixels, 0x00, sizeof(pixels));
	DispState prevState = Instance().currentState;
	DispState nextState = Instance().currentState;
	int maxbrightness = 50;
	int cntr = 0;
	int currbrightness = 50;
	while (1) {
		if(nextState != prevState){
			if(currbrightness <= 0){
				prevState = nextState;
				currbrightness++;
			}else{
				currbrightness--;
			}
		}else if(currbrightness < maxbrightness){
			currbrightness++;
		}
		cntr = (cntr+1)%(100*12);
		switch (prevState){
		case OFF:
			clearAll();
			break;
		case FREE:
			drawFree(cntr,currbrightness);
			break;
		case BUSY:
			drawX(cntr,currbrightness);
			break;
		default:
			break;
		}
		rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
		if (rc) {
			LOG_ERR("couldn't update strip: %d", rc);
		}
		k_sleep(DELAY_TIME);
		if(nextState != Instance().currentState){
			nextState = Instance().currentState;
		}
	}
}
```
After some initialization steps, all pixels of the strip are off. We store the current and the next state in separate variables. This enables transitioning between these animations in a smooth way. The `maxbrightness` variable sets the maximum brightness any color channel will have. It can be increased to 255 if the device is to be used in bright areas, but note that this will result in increased power consumption. The `currbrightness` variable stores the current brightness level, which is used during transitions between different states. If `Instance().currentState` does not match the state within the thread, the `nextState` gets updated and the following events will occur:
1. The brightness of the current animation is decremented at each iteration.
2. When the brightness reaches 0, the state is updated to the new one, meaning that starting this iteration, the new animation is displayed.
3. The brightness is incremented at each iteration until `maxbrightness` is reached.

 `cntr` is a variable which is used as a time parameter of the `drawFree(...)` and `drawX(...)` animations to signal which frame to draw. `led_strip_update_rgb` is called in each iteration to set the LED pixel states on the display based on the contents of the `pixels` array. 
 
 The next functions we examine are `set(...)`, `clearAll()` and `flicker(...)`, which are used by the animation functions (`drawX(...)`, `drawFree(...)`):

```c
void AppTask::set(int x, int y, int r, int g, int b){
	int idx = 0;
	if(y%2 == 0){
		idx = y*12+(11-x);
	}else{
		idx = y*12+x;
	}
	pixels[idx].r = r;
	pixels[idx].g = g;
	pixels[idx].b = b;
}
void AppTask::clearAll(){
	memset(&pixels, 0x00, sizeof(pixels));
}
int inline flicker(int value, int offset){
	return (value*(100-abs(50-offset%100)))/100;
}
```

The `set(...)` function is a simple mapping of the strip to the coordinates shown in the following figure, enabling the use of 2D coordinates in the animations: 

<div align="center"><img src="./imgs/led-order.jpg" width="80%"></div>

`clearAll()` is used to clear the strip to black. `flicker(...)` is a simple mapper function which can be used to have a continuous linear change in the output between `value` and `value/2` depending on the offset. We will use it to get a pulsing effect. 

Now that the helper functions are defined, the next step is visualizing the `FREE` and `BUSY` states:

```c
void AppTask::drawX(int offset100, int brightness){
	int factor = offset100%100;
	int offset = offset100/100;
	clearAll();

	set(4,1,flicker(brightness,factor),0,0);
	set(5,1,flicker(brightness,factor),0,0);
	set(6,1,flicker(brightness,factor),0,0);
	set(7,1,flicker(brightness,factor),0,0);

	set(4,2,flicker(brightness,factor+20),0,0);
	set(7,2,flicker(brightness,factor+20),0,0);

	set(4,3,flicker(brightness,factor+40),0,0);
	set(7,3,flicker(brightness,factor+40),0,0);

	set(4,3,flicker(brightness,factor+60),0,0);
	set(5,3,flicker(brightness,factor+60),0,0);
	set(6,3,flicker(brightness,factor+60),0,0);
	set(7,3,flicker(brightness,factor+60),0,0);
	
	

	int level = flicker(brightness,offset*9+factor/12)-brightness/3;
	for(int i = 0; i < 5; i++){
		set(0,i,level,brightness/10,0);
		set(11,i,level,brightness/10,0);
	}	
}

void AppTask::drawFree(int offset100, int brightness){
	int factor = offset100%100;
	int offset = (offset100/240)%5;
	clearAll();

	int fromzero = 120-abs(120-offset100%240);
	int rcolor = (brightness*fromzero)/120;

	set(0,0,0,flicker(brightness,factor),0);
	set(11,0,0,flicker(brightness,factor),0);
	
	set(0,1,0,flicker(brightness,factor+20),0);
	set(11,1,0,flicker(brightness,factor+20),0);
	
	set(0,2,0,flicker(brightness,factor+40),0);
	set(11,2,0,flicker(brightness,factor+40),0);
	
	set(0,3,0,flicker(brightness,factor+60),0);
	set(11,3,0,flicker(brightness,factor+60),0);
	
	set(0,4,0,flicker(brightness,factor+80),0);
	set(11,4,0,flicker(brightness,factor+80),0);
	
	for(int i = 3; i < 9; i++){
		set(i,offset,0,rcolor,0);
	}
}
```
Here is what these animations look like on the display:

<div align="center"><img src="./imgs/state-free.gif" width="80%"></div>

[https://github.com/ndaneil/matterdoor/blob/main/imgs/state-free.gif](https://github.com/ndaneil/matterdoor/blob/main/imgs/state-free.gif)

<div align="center"><img src="./imgs/state-busy.gif" width="80%"></div>

[https://github.com/ndaneil/matterdoor/blob/main/imgs/state-busy.gif](https://github.com/ndaneil/matterdoor/blob/main/imgs/state-busy.gif)

The handler functions also need to be defined:

```c
void AppTask::IdentifyStartHandler(Identify *) {
	AppEvent event;
	event.Type = AppEventType::IdenfityStart;
	event.Handler = [](const AppEvent &event) {
		sFactoryResetLEDs.Blink(LedConsts::kIndentifyBlinkRate_ms);
	};
	PostEvent(event);
}
void AppTask::IdentifyStopHandler(Identify *) {
	AppEvent event;
	event.Type = AppEventType::IdentifyStop;
	event.Handler = [](const AppEvent &event) {
		sFactoryResetLEDs.Set(false);
	};
	PostEvent(event);
}
void AppTask::OnOffEffectHandler(OnOffEffect *effect) {

}

void AppTask::setState(DispState newState){
	currentState = newState;
}
```
The `Identify` cluster's functions are the same as in [Developing Matter 1.0 products with nRF Connect SDK](https://youtu.be/9Ar13rMxGIk). The callback handlers are needed to initialise the `sIdentify` and `sOnOffEffect` variables. Additionally, the blink rate and the light endpoint id needs to be defined. The location for these insertions can be checked in the [app_task.cpp](./matter-door/src/app_task.cpp) file.
```c
constexpr uint32_t kIndentifyBlinkRate_ms{ 500 };
constexpr uint8_t kLightEndpointId = 1;
Identify AppTask::sIdentify = { kLightEndpointId, AppTask::IdentifyStartHandler, AppTask::IdentifyStopHandler, EMBER_ZCL_IDENTIFY_IDENTIFY_TYPE_VISIBLE_LED};
OnOffEffect AppTask::sOnOffEffect = { kLightEndpointId, AppTask::OnOffEffectHandler };
```

So far, the `Identify` cluster is working and the animations for the states are defined, but how can we control this remotely? We need a way to set the state within our `AppTask` instance. To do that, let's create a new file, `callbacks.cpp`:
```c
#include "app_task.h"
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <app/data-model/Nullable.h>
#include <lib/core/DataModelTypes.h>
#include <lib/support/CodeUtils.h>
#include <app/clusters/on-off-server/on-off-server.h>
#include <app/clusters/level-control/level-control.h>

using namespace ::chip;
using namespace ::chip::app::Clusters;
using namespace ::chip::app::Clusters::ColorControl;
using namespace ::chip::app::Clusters::LevelControl;
using namespace ::chip::app::Clusters::OnOff;
using ::chip::app::DataModel::Nullable;

LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & attributePath, uint8_t type, uint16_t size,
                                       uint8_t * value)
{

    LOG_INF("-----------------> Post attribute change callback! %d %d %d", type, size, *value);

    if(type == 32){
        if(size == 1){
            LOG_INF("Setting display state to %d", *value);
            AppTask::Instance().setState((AppTask::DispState) (*value));
        }
    }
}
```

In the callback, everything other than type `0x20` is filtered out. `0x20` corresponds to an unsigned 8-bit attribute, which is the type of the `CurrentLevel` attribute within the Level Control cluster:

<div align="center"><img src="./imgs/zap-level-control.png" width="80%"></div>

Checking the attribute this way is not ideal since other `uint8` attributes can also alter the display state, but for now, there seemed to be no issues. Ideally, the attribute path would need to be checked instead. After filtering out the unrelevant messages, the state within the `AppTask` instance needs to be set and the rest is handled by the display shown earlier. Finally, we need to add this file to the sources in `CMakeLists.txt`:
```diff
...
  target_sources(app PRIVATE
      src/app_task.cpp
      src/main.cpp
      src/zap-generated/IMClusterCommandHandler.cpp
      src/zap-generated/callback-stub.cpp
+     src/callbacks.cpp
      ${COMMON_ROOT}/src/led_widget.cpp
  )
...
```

The firmware in now complete, the final step is compiling it and flashing it to the nRF7002DK. The file structure of the final Matter project should look something like this (some extra unused config files are not shown). 

```
.
├── CMakeLists.txt
├── Kconfig
├── README.rst
├── boards
│   └── nrf7002dk_nrf5340_cpuapp.overlay
├── configuration
│   └── nrf7002dk_nrf5340_cpuapp
│       └── pm_static_dfu.yml
├── prj.conf
├── sample.yaml
└── src
    ├── app_config.h
    ├── app_event.h
    ├── app_task.cpp
    ├── app_task.h
    ├── callbacks.cpp
    ├── chip_project_config.h
    ├── main.cpp
    ├── template.matter
    ├── template.zap
    └── zap-generated
        ├── CHIPClientCallbacks.h
        ├── CHIPClusters.h
        ├── IMClusterCommandHandler.cpp
        ├── PluginApplicationCallbacks.h
        ├── access.h
        ├── af-gen-event.h
        ├── callback-stub.cpp
        ├── endpoint_config.h
        ├── gen_config.h
        └── gen_tokens.h
```

## Matter fabric and Android app

Now that the MatterDoor device is ready, we need to connect it to a Matter fabric. I wanted to understand how the process would work if I had my own Fabric, so I chose to modify Google's [Google Home Mobile SDK Sample Applications for Matter](https://github.com/google-home/sample-apps-for-matter-android/tree/main). The final, modified code is available in the repository's [android-app](./android-app/) folder. The changes done can be categorised the following way:
 - New icon:
   <img src="./logo.png" height="36" />
 - Changing UI so that instead of a simple On/Off option, we have the option to select between the `FREE` and `BUSY` modes in addition to the `OFF` mode: <div align="center"><img src="./imgs/app-control.png" width="50%"></div>

 - Replacing the On/Off cluster controller logic to Level cluster controlling logic. 
   - A level setting of 0 means the device is `OFF`, 1 means `FREE`, 2 means `BUSY`.
   - Though the On/Off cluster is also enabled on the nRF7002DK, that is not in use. For consistency, I used `setLevelDeviceStateOnOffCluster(...)` to set the level value. This function also controls the On/Off cluster automatically based on the level value. 

### Commissioning MatterDoor to the custom fabric

After building and launching the app on the phone, we can start adding a new device:
1. Open the app and click on the plus icon on the bottom right corner to add a new device.
2. Scan the QR code, which is available on the url output to the console of the nRF7002DK, or enter the setup code available on the console output.
3. Wait for the commissioning process to complete. If the connection is unsuccessful, turn off any VPN connections on the phone, long press button 1 on the nRF7002DK to factory reset it, and try the process again.
4. Name the device.
5. Control the display state of MatterDoor from the app.
<div align="center"><img src="./imgs/setup-1.jpg" width="30%">&nbsp<img src="./imgs/setup-2.jpg" width="30%">&nbsp<img src="./imgs/setup-3.jpg" width="30%"></div>

<div align="center"><img src="./imgs/setup-4.jpg" width="30%">&nbsp<img src="./imgs/setup-5.jpg" width="30%"></div>

## Demo
The demo video showing the setup process and MatterDoor in use is available here: [https://github.com/ndaneil/matterdoor/#demo](https://github.com/ndaneil/matterdoor/#demo)

## A note on licenses
 - All third-party brands (including brand names, logos, trademarks and icons) remain the property of their respective owners. Matter is developed by the Connectivity Standards Alliance<sup>TM</sup>.
 - The original Google Home Sample App for Matter is licensed under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0). 
 - The Matter template code used ([https://github.com/nrfconnect/sdk-nrf/tree/main/samples/matter/template](https://github.com/nrfconnect/sdk-nrf/tree/main/samples/matter/template)) is available under the following license: [https://github.com/nrfconnect/sdk-nrf/blob/main/LICENSE](https://github.com/nrfconnect/sdk-nrf/blob/main/LICENSE)