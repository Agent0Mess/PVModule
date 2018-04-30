#include "orientation_measure.h"

OrientationMeasurement::OrientationMeasurement ():
    Adafruit_BNO055(-1, BNO055_ADDRESS_A), Quaternion(){}

OrientationMeasurement::OrientationMeasurement ( int32_t sensorID = -1, uint8_t address = BNO055_ADDRESS_A ):
    Adafruit_BNO055(sensorID, address), Quaternion(){}

imu::Vector<3> OrientationMeasurement::get_eulerAngles() const{
    imu::Vector<3> orientation;
    imu::Quaternion quat=getQuat();
    orientation=quat.toEuler()*(180/3.1415);
    return orientation;
}

void OrientationMeasurement::load_calibration_data() {
    saved_offsets.accel_offset_x =-13;
    saved_offsets.accel_offset_y =-29;
    saved_offsets.accel_offset_z =-31;
    saved_offsets.gyro_offset_x  = 49;
    saved_offsets.gyro_offset_y  =732;
    saved_offsets.gyro_offset_z  =-491;
    saved_offsets.mag_offset_x   =-8;
    saved_offsets.mag_offset_y   =-1;
    saved_offsets.mag_offset_z   =36;
    saved_offsets.accel_radius   =1000;
    saved_offsets.mag_radius     =799;

    setMode(OPERATION_MODE_CONFIG);
    setSensorOffsets(saved_offsets);
    setMode(OPERATION_MODE_NDOF);
}
