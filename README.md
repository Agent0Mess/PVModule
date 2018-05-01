# PVModule
Important note: This is a work in progress. Even though most of the code compiles properly, important functionalities and features are still missing. Those will be provided during the next weeks.

PVModule is a project written in C++ which should provide all the functionality to align a Solar Panel with the sun.

Sunpos provides functions to calculate the position of the sun at a given time and in a given location,
in order to align a solar panel respectively.

Orientation_measure extends the functionalities of the Adafruit libraries for the BNO055 IMU to adept them for our purposes.
This means that the respective library must be installed: https://github.com/adafruit/Adafruit_BNO055
As well as the Adafruit Sensor library: https://github.com/adafruit/Adafruit_Sensor

In order for the RTC to work properly the following library is used: https://github.com/adafruit/RTClib

The code is designed to run on an Arduino Uno, but it shuld run on most other boards of the Arduino family as well.

Further information will be provided along with the progression of the project.

