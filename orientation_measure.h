
#include <Adafruit_BNO055.h>
#include <utility/quaternion.h>
#include <utility/vector.h>

class Orient_Measure : public Adafruit_BNO055, public imu::Quaternion
{
  public:
    Orient_Measure ( int32_t sensorID, uint8_t address);

    imu::Vector<3> Orient_Measure::get_eulerAngles() const;
};
