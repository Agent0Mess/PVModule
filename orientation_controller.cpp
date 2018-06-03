#include "orientation_controller.h"


datetime_t OrientationController::getMez_now() const
{
    return mez_now_;
}

OrientationController::OrientationController()
{
    set_tolerance_band_azimuth(6.0);
    set_tolerance_band_tilt(2.0);
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
    imu::Vector<3> orientation;
    float des_tilt;
    float result;

    des_tilt=this->desired_value_tilt();
    orientation = orient_sensor.get_eulerAngles();
    result= des_tilt-(180.0-orientation.z());              /** x-axis */
    return result;
}

float OrientationController::deviation_azimuth()
{
    imu::Vector<3> orientation;
    float des_azimuth;
    float result;

    des_azimuth=this->desired_value_azimuth();
    orientation = orient_sensor.get_eulerAngles();
    result= des_azimuth-orientation.x();
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

void OrientationController::orient_panel(){


    if ((abs(deviation_tilt())>tolerance_band_tilt_)&& !is_tilt_running_)
    {
        if ((deviation_tilt()<0 && deviation_tilt()>(-100)) )
        {
            is_tilt_running_=panel_driver.run_tilt_panel_vertical();
        }
        else if(deviation_tilt()>0 && deviation_tilt()<(100))
        {
            is_tilt_running_=panel_driver.run_tilt_panel_horizontal();
        }
    }
    else if (abs(deviation_tilt())<=tolerance_band_tilt_)
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
    else if (abs(deviation_azimuth())<=tolerance_band_azimuth_)
    {
        panel_driver.stop_turn_panel();
        is_azi_running_=false;
    }

}

void OrientationController::stop_panel() {
    panel_driver.stop_turn_panel();
    panel_driver.stop_tilt_panel();
    is_azi_running_=false;
    is_tilt_running_=false;
}
