#include "orientation_measure.h"

Orient_Measure::Orient_Measure ( int32_t sensorID = -1, uint8_t address = BNO055_ADDRESS_A ):
    Adafruit_BNO055(sensorID, address), Quaternion(){}

imu::Vector<3> Orient_Measure::get_eulerAngles() const{

}
