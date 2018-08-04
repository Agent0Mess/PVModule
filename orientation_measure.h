#ifndef ORIENTATIONMEASURE_H
#define ORIENTATIONMEASURE_H


#include "SparkFun_BNO080_Arduino_Library.h"
#include <utility/vector.h>
#include <utility/quaternion.h>
#include <math.h>



class OrientationMeasurement : public BNO080
{
private:

public:
    /**
     * @brief Standard Constructor for class
     *
     * Standard ID and address are taken. This should work in
     * most cases.
     */
    OrientationMeasurement ();

    /**
     * @brief Calculates the euler angles of the sensor's orientation
     * @return angles around x,y anz z axis in degrees
     */
    imu::Vector<3> get_eulerAngles();

};

#endif
