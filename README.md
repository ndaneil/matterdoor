# <img src="./logo.png" height="26" /> MatterDoor - Smart, Matter-capable do not disturb sign

A problem I encountered during the pandemic attending onlinre lectures was that my family did not know when I had the lectures. They would enter the room during lectures or tests. The problem is also present when working from home. People walking in when attending meetings was something people had to get used to. **But there is a solution!** If you have visited a radio station or saw one in a move, you must have seen the large ON AIR signs which light up signaling you to be cautios. I have made one for myself during the pandemic to signal when not to enter. This project is a better version of that prototype. It has cool animations on the five-row RGB display in the front of the device. The brain of MatterDoor is the nRF7002DK, Nordic Semiconductor's latest Matter-capable development kit. MatterDoor can be commissioned to  and controlled via a custom Matter fabric using the Android app in this repository, or any existing Matter-compatible ecosystem. Say goodbye to interrupted meeting and lectures.

This project is an entry to the [Make it Matter!](https://www.hackster.io/contests/makeitmatter) contest.

## Matter
<p align="center" style="background-color:white;"><img src="./imgs/matter-logo.png" width="40%"></p>

[Matter](https://csa-iot.org/all-solutions/matter/) is a new application-layer protocol developed by members of the Connectivity Stands Alliance aiming to be the industry standard protocol for all IoT devices. Both Google and Apple pledged to support it for their smart home ecosystems. An important feature is the ability of Matter-compatible devices to be commissioned into multiple ecosystems (called Matter fabrics), allowing the end user the freedom to choose their ecosystem without having to worry whether the device will support it. Additionally, simultaneous control from separate ecosystems is also possible. 

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

### Assembly

For the electronics-side, the components need to be soldered together based on the following schematic:

<p align="center"><img src="./imgs/schematic.png" width="50%"></p>

The 8 pins grouped together on the left need to be connected to a header in the nRF7002DK and the `P1.13` label marks the wire that will be connected to that pin on the board:

<p align="center"><img src="./imgs/nrf7002dk-connect.jpg" width="80%"></p>

Keeping the ording of the pins on the header is important, it has to be connected in that exact order. The wiring is shown below:

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


## Demo



