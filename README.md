# Arduino Core support for Pettee UWB Board
## Installing using Boards Manager
This is the preferred and easiest way to install Arduino Core support for Pettee UWB Board

To start the installation process using the Boards Managaer, follow these steps:

- Install the current upstream Arduino IDE at the 1.8 level or later. The current version is at the [arduino.cc](https://www.arduino.cc/en/software "arduino.cc") website.

- Start Arduino and open the Preferences window and enter the release link above into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.

	![Arduino Preferences Window](https://docs.espressif.com/projects/arduino-esp32/en/latest/_images/install_guide_preferences.png)

- Open Boards Manager from Tools > Board menu and install PetteeMFG Boards platform (and do not forget to select your PetteeMFG board from Tools > Board menu after installation).

- Restart Arduino IDE.
