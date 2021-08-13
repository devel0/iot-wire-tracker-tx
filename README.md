# iot-wire-tracker-tx

<!-- TOC -->
* [Description](#description)
  + [Usage](#usage)
* [How this project was built](#how-this-project-was-built)
<!-- TOCEND -->

<hr/>

## Description

Crude replacement for a damaged wire tracker tx module.

Generates a 2.5khz ( 50% duty ) square signal suitable for red wire.

![](doc/schematic.png)

<img src="doc/tx-signal.jpg" height=300/>

### Usage

example:
| **tx side** | **other side** |
|---|---|
| cable_txred | cable_unk1 |
| cable_black | cable_unk2 |
|  | ... |
|  | cable_unkX |

:warning:
- ensure no voltage present on cables either for the tx side and other side cables

- connect cable_txred to the pin 9
- connect cable_black to the 10k resistor that in turn connects to GND ( to avoid double verify the best is to attach cable_black to a known cable, such an earth gnd or chassis )
- go to the other side with the wire tracker rx tool ( not provided ) that amplify into a speaker detected signal moving the point on wires
- two wires ( corresponding to the counterpart cable_txred and cable_black ) cause high volume on speaker
- counterverify if cable_unk1 e and cable_unk2 are those searched by connecting them together and if the speaker stop this mean that they are those matching source tx.
- place a mark on two wires matched
- **important** : go to the tx side and disconnect cable_txred, cable_black before to reapply any voltage on cables or tx module could get damaged

## Notes
- 10k resistor allow to safely short circuit between red, black wire to cancel the signal in the counterverify step without destroy the pin9 port because of high level output of the same while connects to gnd from black cable

## How this project was built

- from platformio / projects / new selecting arduino nano (new bootloader)
- from platformio / libraries / added TimerInterrupt
