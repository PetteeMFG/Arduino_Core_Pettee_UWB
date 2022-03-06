# Arduino Core Support for Pettee UWB Board
## Installing using Boards Manager
This is the preferred and easiest way to install Arduino Core support for Pettee UWB Board

To start the installation process using the Boards Managaer, follow these steps:

- Install the current upstream Arduino IDE at the 1.8 level or later. The current version is at the [arduino.cc](https://www.arduino.cc/en/software "arduino.cc") website.

- Start Arduino and open the Preferences window and enter the release link below into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.

		https://raw.githubusercontent.com/PetteeMFG/Pettee_UWB_Package/main/package_petteeuwb_index.json

	![Arduino Preferences Window](https://docs.espressif.com/projects/arduino-esp32/en/latest/_images/install_guide_preferences.png)

- Open Boards Manager from Tools > Board menu and install PetteeMFG Boards platform (and do not forget to select your PetteeMFG board from Tools > Board menu after installation).

- Restart Arduino IDE.

## Programming Pettee UWB Board
In order to program the STM32F401RBT6 microcontroller on the Pettee UWB Board an ST-Link V2 needs to be used to connect your computer and the board. You must make the following connections between the ST-Link V2 and the Pettee UWB board in order for SWD JTAG programming or debugging to take place.

	ST-Link V2 to Pettee UWB Board

	3.3V -> 3.3V

	IO ---> SWDIO (PA13)

	CLK --> SWCLK (PA14)

	RST --> RESET (NRST)

	GND --> GND

![Pettee UWB Board Pinout Diagram](https://i.imgur.com/zdrW76N.png)
