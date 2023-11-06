# <img src="./logo.png" height="26" /> MatterDoor - Smart, Matter-capable do not disturb sign

A problem I encountered during the pandemic attending onlinre lectures was that my family did not know when I had the lectures. They would enter the room during lectures or tests. The problem is also present when working from home. People walking in when attending meetings was something people had to get used to. **But there is a solution!** If you have visited a radio station or saw one in a move, you must have seen the large ON AIR signs which light up signaling you to be cautios. I have made one for myself during the pandemic to signal when not to enter. This project is a better version of that prototype. It has cool animations on the five-row RGB display in the front of the device. The brain of MatterDoor is the nRF7002DK, Nordic Semiconductor's latest Matter-capable development kit. MatterDoor can be commissioned to  and controlled via a custom Matter fabric using the Android app in this repository, or any existing Matter-compatible ecosystem. Say goodbye to interrupted meeting and lectures.

This project is an entry to the [Make it Matter!](https://www.hackster.io/contests/makeitmatter) contest.

## Matter
<p align="center" style="background-color:white;"><img src="./imgs/matter-logo.png" width="40%"></p>

[Matter](https://csa-iot.org/all-solutions/matter/) is a new application-layer protocol developed by members of the Connectivity Stands Alliance aiming to be the industry standard protocol for all IoT devices. Both Google and Apple pledged to support it for their smart home ecosystems. An important feature is the ability of Matter-compatible devices to be commissioned into multiple ecosystems (called Matter fabrics), allowing the end user the freedom to choose their ecosystem without having to worry whether the device will support it. Additionally, simultaneous control from separate ecosystems is also possible. 

This project was the first time I used the nRF SDK. I was happy to see that Nordic offers an [nRF Connect SDK Fundamentals](https://academy.nordicsemi.com/courses/nrf-connect-sdk-fundamentals/) course, which I have completed, earning a certificate 🎉

<p align="center"><img src="./imgs/my-certificate.png" width="50%"></p>

 I highly recommend this course. Additionally, I have watched the following webinars on Matter:
 - [Introduction to Matter](https://youtu.be/v_285vCHifw)
 - [Developing Matter 1.0 products with nRF Connect SDK](https://youtu.be/9Ar13rMxGIk)
 - [How to go to market with Matter](https://youtu.be/ZV6fjTLAqdA)


## Hardware

I applied for free hardware with my idea and got the nRF7002DK from Nordic Semiconductor. I decided to use the form factor of the development kit for the sign. I contemplated several display technologies:
 - E-ink has the advantages of low power consumption and vibrant colors in bright light. But for a do not disturb sign, we need a way to get the attention of the person approachin the door, so display technologies emitting light are more suitable for that purpose.
 - LCD displays offer high resolution while still being visible in the dark.
 - OLED displays have a great viewing angle, but are more expensive and the form factors are smaller.
 - Programmable LEDs offer a great way for custom displays, but the light needs to be deflected.

In the end, I decided to use WS2812b programmable LEDs for the display part since I could design the display to match the dimensions of the nRF7002DK. It also meant that I could design some cool animations to be run on the display. To diffuse the light, I used tubes designed for LED strips.

### BOM

Apart from the 3D printed housing, the most important components are the WS2812b LED strip, the tube and the logic level shifter needed since the nRF3540 has 1.8V logic level pins:

<p align="center"><img src="./imgs/components.jpg" width="50%"></p>

All the components needed:
 - Wires
 - One jumper wire
 - 500R resistor
 - 3K3 resistor
 - TXS0108E logic level shifter
 - Tube for LED strip, 1m
 - WS2812B LED strip 96 pixels/leds/m, 1m
 - Perfboard
 - 1x8 pin header

#### 3D-printed parts

The housing I designed consists of four major parts and an optional switch cover. **The STL files of the degined casing are available [here](./3dfiles).** GitHub has a nice 3D viewer, but here are some screenshots from Shapr3D, the software I used to design the casing:

<p align="center"><img src="./imgs/3d-side.PNG" width="50%"><img src="./imgs/3d-fromtop.PNG" width="50%"></p>
<p align="center"><img src="./imgs/3d-side2.PNG" width="50%"><img src="./imgs/3d-fromabove.PNG" width="50%"></p>

**[The assembled 3D model can be viewed here.](./3dfiles/matterdoor-combined-do-not-use.stl)**

### Assembly

For the electronics-side, the components need to be soldered together based on the following schematic:

<p align="center"><img src="./imgs/schematic.png" width="50%"></p>

The 8 pins grouped together on the left need to be connected to a header in the nRF7002DK and the `P1.13` label marks the wire that will be connected to that pin on the board:

<p align="center"><img src="./imgs/nrf7002dk-connect.jpg" width="80%"></p>

**VDD is 1.8V.** Keeping the ording of the pins on the header is important, it has to be connected in that exact order. The wiring is shown below:

<p align="center"><img src="./imgs/wiring1.jpg" height="200"><img src="./imgs/wiring2.jpg" height="200"></p>

The three wires leaving the perfboard are to be connected to the LED strip. Cut five 12.5cm-long parts from the tube and cut five 12-pixel-long sections from the LED strip. The next step is placing the cut strip parts in alternate orientation parallel to each other. Soldering wires to the `GND` and `5V` pins is the next step, then connecting these and joining it to the `5V` and `GND` wires leaving the perfboard. The `DIN` wire from the perfboard needs to be connected to the input of the first strip section. Placing the LEDs in the tubes is the next step, followed by soldering the data wires between the strips the following way:

<p align="center"><img src="./imgs/rows-wiring.jpg" width="80%"></p>

The next step is putting double-sided tape on the top cover. It may not be strong enough, so some other glue can be used as well:

<p align="center"><img src="./imgs/top-tape.jpg" width="80%"></p>

After securing the tubes on the top plate, the two side pieces can be slided over from the sides. The three wires leaving the top panel need to be routed through one of the two holes in the top panel.

<p align="center"><img src="./imgs/pillars.jpg" width="80%"></p>

<p align="center"><img src="./imgs/halfassembled.jpg" width="80%"></p>

<p align="center"><img src="./imgs/top-assembled.jpg" width="80%"></p>

The assembly of the top part is then complete. An additional wires can be soldered to the `5V` and `GND` pins so that power can be supplied over that instead of the Micro USB port. The perfboard can be covered up with hot glue. An important part is the side close to the nRF7002DK board. Putting a piece of duct tape there is needed to prevent potential short circuits.

<p align="center"><img src="./imgs/connected.jpg" width="80%"></p>

The last step is assembling the top and bottom parts and the hardware-part is complete:

<p align="center"><img src="./imgs/assembled-side.jpg" width="80%"></p>

<p align="center"><img src="./imgs/assembled-top.jpg" width="80%"></p>

## Software

There were three main challenges with the software:
1. Get the LED strip to work with the nRF7002DK
2. Modifying the Matter template
    - Using ZAP was challenging
    - Long compile times before linking fails
3. Controlling the device from my phone
    - In the end, I chose to modify Google's [Google Home Mobile SDK Sample Applications for Matter](https://github.com/google-home/sample-apps-for-matter-android/tree/main) to create my own Matter fabric

### Controlling the LEDs

As someone familiar with the Arduino framework, I got used to having lots of libraries and examples for peripherials. Controlling WS2812b LEDs just worked with every microcontoller I have tried before. Even with 3.3V logic level microcontrollers like the ESP32, running a [FastLED](https://github.com/FastLED/FastLED) or a [NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) example code worked out of the box. Since the nRF7002DK is a relatively new board, so far the [official Zephyr repo](https://github.com/zephyrproject-rtos/zephyr/tree/d5a1a8bf04c9a4a1f7b9c73f2426368e93844143/samples/drivers/led_ws2812/boards) did not support that board. As a result, I looked for example elsewhere. The only example I could find was a [hackster.io project](https://www.hackster.io/mahmood-ul-hassan/how-to-interface-nordic-thingy-53-with-neopixels-ws2812b-c79533). As shown in the hardware section, the board has a 1.8V logic level, which makes a logic level shifter necessary. Even though I added one, I could not get the example to work. I tried modifying the official example from the Zephyr repo to work with the nRF53 and I managed to make it work. But it was unstable. I could control the first couple of LEDs, but afterwards, the LEDs just showed random colors. I adjusted several config options, but the display was still unstable. It was at that point that I decided to [raise a ticket in Nordic DevZone](https://devzone.nordicsemi.com/f/nordic-q-a/105200/ws2812-driver-for-nrf5340) to ask if there was an official example. Thanks to Kenneth from Nordic, I started investigating if the level shifting was not working correctly. I switched to using the `TXS0108E` shown in the hardware section, but I still could not get a stable output. The final solution was to add the two resistors, one to pull high the Output Enable pin of the level shifter, the other to add resistance between the first LED's input and the bi-directional level shifter's high side. With these, controlling the display works flawlessly. In the spirit of open soure, I created a pull request in the Zephyr repository to add nRF7002DK support to the WS2812 example: https://github.com/zephyrproject-rtos/zephyr/pull/64823

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

&gpio1 {
	status = "okay";
};

/ {
	led_strip: ws2812 {
		compatible = "worldsemi,ws2812-i2s";

		i2s-dev = < &i2s_led >;
		chain-length = <60>; /* arbitrary; change at will */
		color-mapping = <LED_COLOR_ID_GREEN
					LED_COLOR_ID_RED
					LED_COLOR_ID_BLUE>;
		// out-active-low;
		reset-delay = <500>;
	};

	aliases {
		led-strip = &led_strip;
	};
};
```

The I2S device is configured with the default pinout. From the four poins, `I2S_SDOUT` is the important one which is the data output. Controlling WS2812b LEDs is done over a single wire, which means that the timing of the signals is crucial. With the I2S interface, a fixed clock frequency is provided and then the output bits are set in a way to 'emulate' as if the pin was set at the required interval to the correct logic level by the bits output at the pin. Since the I2S pins are located at the GPIO bank 1, we need to enable that along with the I2S device. Finally, we need to add the `led_strip` to the devicetree. Here, the important parameters are the `reset-delay` and the `chain-length`. A major difference between the WS2812 and WS2812b(more common) LEDs is that the latter require at least a 500us reset time. Though it may work with less time, it is better to follow the specification. The `chain-length` should be set to the length of the LED chain. In our case, that is $5 \cdot 12 = 60$. A WS2812b LED at full brightness can consume 60mA, which would result in a 3.6A current draw if all LEDs would be turned to full brighness. I could not find what is the maximal current output the nRF7002DK can output at the 5V pin, but since the USB 2.0 specification has a 500mA limit (though it is usually possible to draw up to 2A), I soldered an external connector to be able to power the device from a separate 5V source. Additionally, when designing the animations on the display, an important consideration was this limit not to overload the voltage supply of the board. 

In addition to configuring the drivers, the following config parameters are also needed:

```
CONFIG_SPI=n

CONFIG_I2S=y
CONFIG_WS2812_STRIP=y
CONFIG_WS2812_STRIP_I2S=y
```
Since in the official WS2812 Zephyr example, SPI is the default device, that needs to be turned off and I2S on. The LED driver also needs to be enabled and the I2S mode needs to be set. With these parameters, it is possible to control the LEDs. 

### Modifying the matter template

In the [Developing Matter 1.0 products with nRF Connect SDK](https://youtu.be/9Ar13rMxGIk?t=2053) webinar, there was an example on how to use the Matter-template project and modify it to be a simple Matter light switch. I tried to follow that tutorial, but I encountered a strange issue at the ZAP step. ZAP is the program that would need to be used to create the Matter endpoint configuration. But there was a problem: whenever I would edit the configuration, it would show the zigbee configuration:
``` bash
zap ./src/template.zap
```

<p align="center"><img src="./imgs/zap-zigbee.png" width="80%"></p>

And there were no options to add Matter-specific endpoints. Then after saving the file, lots of options were removed from the `template.zap` file. (I could see this from the diff). And the example that I could previously comple no longer worked. After tedious debugging, I found what was going on. In the [source code of the ZAP generate script](https://github.com/project-chip/connectedhomeip/blob/master/scripts/tools/zap/generate.py#L87-L108) we have the following function for detecting the ZCL config files:

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
In the [matter template](https://github.com/nrfconnect/sdk-nrf/tree/main/samples/matter/template) the [template.zap](https://github.com/nrfconnect/sdk-nrf/blob/main/samples/matter/template/src/template.zap) file contains the following values:

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
We can see that both the `zcl.json` and `app-templates.json` files are referenced relative to the location of the `template.zap` file. This was an issue for me since on my computer, I installed the nRF tools to `/opt/nordic/` and the project was locaded in a folder in my home directory. Due to this, neither the `generate.py` script, nor the ZAP tool could find these files. The paths needed to be modified in `template.zap` to the correct values:
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
Note that `pathRelativity` was set to `absolute` which is not one of options in the `generate.py` source code, meaning that the path will be used as is, resulting in an absolute path. After opening the zap tool from the command line, the `Matter`-related configuration were correcly loaded and I was able to add a new endpoint:

<p align="center"><img src="./imgs/zap-matter.png" width="80%"></p>

One thing to keep in mind is that after saving the file, the ZAP tool re-fills the paths to be relative! I added a `Matter extended color light`, but later turned on the color-related clusters. Apart from the `Identify` and `On/Off` clusters, the `Level Control` cluster was also used. This cluster will be the one controlling the state of the display. For the do not disturb sign, there are three states that need to be distinguished:
 - OFF - Turning off the display to save power
 - FREE - Actively signal that it is free to enter
 - BUSY - Signal not to enter

After editing and saving the `template.zap` file, the next step is to generate the files. In the code below, I replaced the absolute path to the folder of my Matter template project with `<absolute-path-to-project-folder>`.

 ``` bash
 python3 /opt/nordic/ncs/v2.5.0/modules/lib/matter/scripts/tools/zap/generate.py  ~<absolute-path-to-project-folder>/src/template.zap -t /opt/nordic/ncs/v2.5.0/modules/lib/matter/src/app/zap-templates/app-templates.json -o <absolute-path-to-project-folder>/src/zap-generated
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
Afterwards, we can see that in the `src/zap-generated/` folder, the files were updated.

## Demo



