# arduino-mini-filesystem
Minimal filesystem for EEPROM arduino library


## Installation
* [Download](https://github.com/413j0/arduino-mini-filesystem/archive/master.zip) or clone this repository to `Arduino/libraries/MiniFileSystem`.
* Restart Arduino IDE.
* Check the example.

### Notes:

#### Limitations
needs to have at least one file all the time (it is enforced)

there is a limit of 255 files

filesize is limited to 255 bytes

file name is limited to 255 bytes

it doesn't have folder support but you can emulate it via filenames, because there is no limitiation on it the beside size

it can only access files by number, so you must iterate yourself if you need to acces by name
