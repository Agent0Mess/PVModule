#include "orientation_measure.h"

OrientationMeasurement::OrientationMeasurement ()
{
}


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

