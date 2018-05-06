#ifndef ORIENTATIONMEASURE_H
#define ORIENTATIONMEASURE_H

#include <Adafruit_BNO055.h>
#include <utility/quaternion.h>
#include <utility/vector.h>

class OrientationMeasurement : public Adafruit_BNO055, public imu::Quaternion
{
private:
    adafruit_bno055_offsets_t saved_offsets_;
public:
    /**
     * @brief Standard Constructor for class
     *
     * Standard ID and address are taken. This should work in
     * most cases.
     */
    OrientationMeasurement ();

    /**
     * @brief Constructor if your sensor's id an address are are different
     * @param sensorID - Standard is -1
     * @param address - Standard is BNO055_ADDRESS_A
     */
    OrientationMeasurement (int32_t sensorID, uint8_t address);

    /**
     * @brief Calculates the euler angles of the sensor's orientation
     * @return angles around x,y anz z axis in degrees
     */
    imu::Vector<3> get_eulerAngles() const;

    /**
     * @brief Loads saved calibration data, for the specific sensor
     * in use.
     *
     * If you want to use a different sensor, calibration should be
     * adapted in the function.
     */
    void load_calibration_data ();

};

#endif
