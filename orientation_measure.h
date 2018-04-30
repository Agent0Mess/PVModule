
#include <Adafruit_BNO055.h>
#include <utility/quaternion.h>
#include <utility/vector.h>

class OrientationMeasurement : public Adafruit_BNO055, public imu::Quaternion
{
private:
    adafruit_bno055_offsets_t saved_offsets;
public:
    OrientationMeasurement ();
    OrientationMeasurement (int32_t sensorID, uint8_t address);

    imu::Vector<3> get_eulerAngles() const;

    void load_calibration_data ();

};
