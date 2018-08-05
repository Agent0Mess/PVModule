# PVModule [![Build Status](https://travis-ci.org/Agent0Mess/PVModule.svg?branch=master)](https://travis-ci.org/Agent0Mess/PVModule)
<b>Important note:</b> This is a work in progress. The code compiles and seems to work with my setup. However, a lot
of cases remain untested. If you find issues with the code, I would be glad to hear from you.

PVModule is a project written in C++ for Arduino, which should provide all the functionality to align a Solar Panel with the sun.

<h3> Sun Position - Azimuth And Elevation </h3>
Sunpos provides functions to calculate the position of the sun at a given time and in a given location,
in order to align a solar panel respectively. 

<h3> Determining The Orientation of The Panel </h3>
Orientation_measure extends the functionalities of the Sparkfun BNO080 IMU library to adapt them for our purposes.
This means that the respective library must be installed: https://github.com/sparkfun/SparkFun_BNO080_Arduino_Library
More information about the sensor can be found here: https://www.sparkfun.com/products/14686
In the course of this project, the BNO055 has also been integrated and tested. However it turned out, that this sensor
is not suitable for the project and has been discarded.

<h3> Real Time Clock </h3>
In order for the RTC to work properly the following library is used: https://github.com/adafruit/RTClib
More information about the RTC module used in this project can be found here: https://www.adafruit.com/product/255

<h3> Motor Drivers and Movement </h3>
There are no special libraries required for the integraion of the motors in our case. However, a Relay Shield as well
as a Motor Shield for Arduino are used. The secific functionality concerning the motors, is contained in the MotorDriver class. Since the code depends strongly on the hardware used, it is very likely, that the code needs to be adapted to suit
your setup.

<h3> Movement Control </h3>
The functionaliity concerning the control of the motors is contained in the OrientationController class.

<h3> Further Notes </h3>
The code is designed to run on an Arduino Uno, but it should run on most other boards of the Arduino family as well.
It has been sucessfully tested with an Arduino Uno, Arduino Mega 2560 and Arduino Due (w/o shields).

Further information will be provided along with the progression of the project.

Related to this project are the following pages:
<ul>
  <li>https://www.youmagine.com/designs/spacer-m3</li>
</ul>
  

