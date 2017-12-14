This code is designed for replacing the cloud-based firmware on the Sonoff Basic switch with a LAN-only, simplified version: only supporting on/off/status requests.  The expectation is that additional alternative resources are available on the LAN to handle e.g. scheduling or a GUI.  The code is intended to be compiled by the non-os version of the esp8266 SDK (I used the open-source version; but the proprietary one should work).  There is presently no OTA custom-firmware functionality of which I'm aware, which means this code needs to be uploaded via esptool.py over wires connected to pins soldered onto the Sonoff PCB.

As a further implementation simplification, device hostname & wifi credentials are intended to be compiled into the binary artifact to be uploaded to the device: there is no onboard interface (GUI or API) for connecting to first to the device's hosted access point and then configuring the settings for your personal wifi network.  Use the following command line (assuming  you've already got the cross-compiler in your path) to build/upload (via a FT232RL USB to TTL Adapter):
`env ESPPORT=/dev/ttyUSB0 HOSTNAME=sonoff-den-lamp SSID=your_clever_ssid PASSWORD='Y0ur$ecurePa$$word' make flash`

The API has 3 endpoints, under the `/power/` path:
* on
* off
* state
