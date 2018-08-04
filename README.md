# PVModule [![Build Status](https://travis-ci.org/Agent0Mess/PVModule.svg?branch=master)](https://travis-ci.org/Agent0Mess/PVModule)
<b>Important note:</b> This is a work in progress. The code compiles and seems to work with my setup. However, a lot
of cases remain untested. If you find issues with the code, I would be glad to hear from you.

PVModule is a project written in C++ for Arduino, which should provide all the functionality to align a Solar Panel with the sun.

Sunpos provides functions to calculate the position of the sun at a given time and in a given location,
in order to align a solar panel respectively. 

Orientation_measure extends the functionalities of the Sparkfun BNO080 IMU library to adapt them for our purposes.
This means that the respective library must be installed: https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library
More information about the sensor can be found here: https://www.sparkfun.com/products/14686
In the course of this project, the BNO055 has also been integrated and tested. However it turned out, that this sensor
is not suitable for the project and has been discarded.

In order for the RTC to work properly the following library is used: https://github.com/adafruit/RTClib
More information about the RTC module used in this project can be found here: https://www.adafruit.com/product/255

The code is designed to run on an Arduino Uno, but it should run on most other boards of the Arduino family as well.
It has been sucessfully tested with an Arduino Uno, Arduino Mega 2560 and Arduino Due.

Further information will be provided along with the progression of the project.

