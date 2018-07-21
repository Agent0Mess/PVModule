#include <math.h>
#include "orientation_controller.h"


#define OFFSET_TILT 61.0
#define OFFSET_AZIMUTH 82.0 //South is -98.25 going west is going more negativ

datetime_t OrientationController::getMez_now() const
{
    return mez_now_;
}

bool OrientationController::getIsAziRunning() const
{
    return is_azi_running_;
}

bool OrientationController::getIsTiltRunning() const
{
    return is_tilt_running_;
}

OrientationController::OrientationController()
{
    set_tolerance_band_azimuth(4.0);
    set_adjust_precision_azimuth(3.0);
    set_tolerance_band_tilt(3.0);
    set_adjust_precision_tilt(2.0);

}


void OrientationController::begin()
{
    orient_sensor.begin();
    delay(1000);
    orient_sensor.enableRotationVector(50);
}

bool OrientationController::is_daytime()
{
    mez_now_=current_time.read_time();
    sun_position.set_mez(mez_now_);
    if ((mez_now_>sun_position.get_srmez()) && (mez_now_<sun_position.get_ssmez()))
    {
        sun_is_up_=true;
        return true;
    }
    else
    {
        sun_is_up_=false;
        return false;
    }

}

void OrientationController::readSensorData()
{
    unsigned long millicounter_sensor_read=millis();
    bool is_sensor_read=false;

    while(millis()<=millicounter_sensor_read+1000)
    {
        if (orient_sensor.dataAvailable() == true)
        {
            orient_=orient_sensor.get_eulerAngles();
            is_sensor_read=true;
            break;
        }
    }
    if (is_sensor_read=false)
    {
        //this->stop_panel();
    }
}

float OrientationController::desired_value_tilt()
{
    mez_now_=current_time.read_time();
    sun_position.set_mez(mez_now_);
    return sun_position.get_pvtiltang();
}

float OrientationController::desired_value_azimuth()
{
    mez_now_=current_time.read_time();
    sun_position.set_mez(mez_now_);
    return sun_position.get_azang();
}

float OrientationController::deviation_tilt()
{
    float des_tilt;
    float result;

    des_tilt=this->desired_value_tilt();
    result= des_tilt-(orient_.y()+OFFSET_TILT);
    return result;
}

float OrientationController::deviation_azimuth()
{
    float des_azimuth;
    float result;

    des_azimuth=this->desired_value_azimuth();
    result=des_azimuth-fmod((-orient_.x())+OFFSET_AZIMUTH+360.0,360);
    return result;
}

void OrientationController::set_tolerance_band_tilt(float tilt_tol)
{
    tolerance_band_tilt_=tilt_tol;
}
float OrientationController::get_tolerance_band_tilt() const
{
    return tolerance_band_tilt_;
}

void OrientationController::set_tolerance_band_azimuth(float azi_tol)
{
    tolerance_band_azimuth_=azi_tol;
}
float OrientationController::get_tolerance_band_azimuth() const
{
    return tolerance_band_azimuth_;
}

void OrientationController::set_adjust_precision_tilt(float tilt_adj)
{
    adjust_precision_tilt_=tilt_adj;
}
float OrientationController::get_adjust_precision_tilt() const
{
    return adjust_precision_tilt_;
}

void OrientationController::set_adjust_precision_azimuth(float azi_adj)
{
    adjust_precision_azimuth_=azi_adj;
}
float OrientationController::get_adjust_precision_azimuth() const
{
    return adjust_precision_azimuth_;
}

void OrientationController::orient_panel()
{
    this->readSensorData();

    if (is_daytime())
    {

        if(this->desired_value_azimuth()>=LOWER_TURN_ANGLE && this->desired_value_azimuth()<=UPPER_TURN_ANGLE)
        {
            if ((abs(deviation_tilt())>tolerance_band_tilt_)&& !is_tilt_running_)
            {
                if ((deviation_tilt()<0))
                {
                    is_tilt_running_=panel_driver.run_tilt_panel_vertical();
                }
                else if(deviation_tilt()>0)
                {
                    is_tilt_running_=panel_driver.run_tilt_panel_horizontal();
                }
            }
            else if (abs(deviation_tilt())<=adjust_precision_tilt_)
            {
                panel_driver.stop_tilt_panel();
                is_tilt_running_=false;
            }

            if ((abs(deviation_azimuth())>tolerance_band_azimuth_)&& !is_azi_running_)
            {
                if ((deviation_azimuth()<0))
                {
                    is_azi_running_=panel_driver.run_turn_panel_cw();
                }
                else if(deviation_azimuth()>0)
                {
                    is_azi_running_=panel_driver.run_turn_panel_ccw();
                }
            }
            else if (abs(deviation_azimuth())<=adjust_precision_azimuth_)
            {
                panel_driver.stop_turn_panel();
                is_azi_running_=false;
            }

        }
    }
}


void OrientationController::stop_panel() {
    panel_driver.stop_turn_panel();
    panel_driver.stop_tilt_panel();
    is_azi_running_=false;
    is_tilt_running_=false;
}
