#include "orientation_measure.h"


offset_data_t offset_data_t::operator + (const offset_data_t offset2){
    offset_data_t result;
    result.accel_offset_x= this->accel_offset_x + offset2.accel_offset_x;
    result.accel_offset_y= this->accel_offset_y + offset2.accel_offset_y;
    result.accel_offset_z= this->accel_offset_z + offset2.accel_offset_z;

    result.gyro_offset_x= this->gyro_offset_x + offset2.gyro_offset_x;
    result.gyro_offset_y= this->gyro_offset_y + offset2.gyro_offset_y;
    result.gyro_offset_z= this->gyro_offset_z + offset2.gyro_offset_z;

    result.mag_offset_x= this->mag_offset_x + offset2.mag_offset_x;
    result.mag_offset_y= this->mag_offset_y + offset2.mag_offset_y;
    result.mag_offset_z= this->mag_offset_z + offset2.mag_offset_z;

    result.accel_radius= this->accel_radius + offset2.accel_radius;
    result.mag_radius= this->mag_radius + offset2.mag_radius;

    return result;
}

offset_data_t offset_data_t::operator * (const int multiplicator){
    offset_data_t result;
    result.accel_offset_x= this->accel_offset_x * multiplicator;
    result.accel_offset_y= this->accel_offset_y * multiplicator;
    result.accel_offset_z= this->accel_offset_z * multiplicator;

    result.gyro_offset_x= this->gyro_offset_x * multiplicator;
    result.gyro_offset_y= this->gyro_offset_y * multiplicator;
    result.gyro_offset_z= this->gyro_offset_z * multiplicator;

    result.mag_offset_x= this->mag_offset_x * multiplicator;
    result.mag_offset_y= this->mag_offset_y * multiplicator;
    result.mag_offset_z= this->mag_offset_z * multiplicator;

    result.accel_radius= this->accel_radius * multiplicator;
    result.mag_radius= this->mag_radius * multiplicator;

    return result;
}

offset_data_t offset_data_t::operator / (const double divisor){
    offset_data_t result;
    result.accel_offset_x= static_cast<int>(round(this->accel_offset_x / divisor));
    result.accel_offset_y= static_cast<int>(round(this->accel_offset_y / divisor));
    result.accel_offset_z= static_cast<int>(round(this->accel_offset_z / divisor));

    result.gyro_offset_x= static_cast<int>(round(this->gyro_offset_x / divisor));
    result.gyro_offset_y= static_cast<int>(round(this->gyro_offset_y / divisor));
    result.gyro_offset_z= static_cast<int>(round(this->gyro_offset_z / divisor));

    result.mag_offset_x= static_cast<int>(round(this->mag_offset_x / divisor));
    result.mag_offset_y= static_cast<int>(round(this->mag_offset_y / divisor));
    result.mag_offset_z= static_cast<int>(round(this->mag_offset_z / divisor));

    result.accel_radius= static_cast<int>(round(this->accel_radius / divisor));
    result.mag_radius= static_cast<int>(round(this->mag_radius / divisor));

    return result;
}

void offset_data_t::operator = (adafruit_bno055_offsets_t bno_offsets){
    offset_data_t result;
    this->accel_offset_x = bno_offsets.accel_offset_x;
    this->accel_offset_y = bno_offsets.accel_offset_y;
    this->accel_offset_z = bno_offsets.accel_offset_z;

    this->gyro_offset_x = bno_offsets.gyro_offset_x;
    this->gyro_offset_y = bno_offsets.gyro_offset_y;
    this->gyro_offset_z = bno_offsets.gyro_offset_z;

    this->mag_offset_x = bno_offsets.mag_offset_x;
    this->mag_offset_y = bno_offsets.mag_offset_y;
    this->mag_offset_z = bno_offsets.mag_offset_z;

    this->accel_radius = bno_offsets.accel_radius;
    this->mag_radius = bno_offsets.mag_radius;

    return result;
}


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


adafruit_bno055_offsets_t OrientationMeasurement::load_calibration_data() {
    saved_offsets_.accel_offset_x =-9;
    saved_offsets_.accel_offset_y =17;
    saved_offsets_.accel_offset_z =-3;
    saved_offsets_.gyro_offset_x  =0;
    saved_offsets_.gyro_offset_y  =52;
    saved_offsets_.gyro_offset_z  =1;
    saved_offsets_.mag_offset_x   =210;
    saved_offsets_.mag_offset_y   =54;
    saved_offsets_.mag_offset_z   =26;
    saved_offsets_.accel_radius   =1000;
    saved_offsets_.mag_radius     =620;

    //setSensorOffsets(saved_offsets_);

    return saved_offsets_;
}
