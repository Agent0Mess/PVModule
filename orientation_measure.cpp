#include "orientation_measure.h"

OrientationMeasurement::OrientationMeasurement ()
{
}

offset_data_t offset_data_t::operator * (const int multiplicator){
    offset_data_t result;
    result.accel_offset_x= this->accel_offset_x * multiplicator;
    result.accel_offset_y= this->accel_offset_y * multiplicator;
    result.accel_offset_z= this->accel_offset_z * multiplicator;

imu::Vector<3> OrientationMeasurement::get_eulerAngles(){
    imu::Vector<3> orientation;
    imu::Quaternion quat;
    quat.x()=this->getQuatI();
    quat.y()=this->getQuatJ();
    quat.z()=this->getQuatK();
    quat.w()=this->getQuatReal();
    orientation=quat.toEuler()*(180/3.1415);
    return orientation;
}

