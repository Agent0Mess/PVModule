#include <math.h>
#include "orientation_controller.h"

/** Depending on the mecanical construction of the panel
 * and the position of the sensor */
#define OFFSET_TILT 61.0
#define OFFSET_AZIMUTH 82.0

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

bool OrientationController::getSunIsUp() const
{
    return sun_is_up_;
}

bool OrientationController::getEmergencyStopped() const
{
    return emergency_stopped;
}

bool OrientationController::checkIsPanelTilting()
{
    new_tilt_=currentTilt();
    tilt_check_time_=millis();
    if(abs(last_tilt_-new_tilt_)<SENSORPRECISION && abs(second_tilt_-new_tilt_<SENSORPRECISION))
    {
        last_tilt_=second_tilt_;
        second_tilt_=new_tilt_;
        return false;
    }
    else
    {
        last_tilt_=second_tilt_;
        second_tilt_=new_tilt_;
        return true;
    }
}

bool OrientationController::checkIsPanelTurning()
{
    new_azi_=currentAzimuth();
    turn_check_time_=millis();
    if(abs(last_azi_-new_azi_)<SENSORPRECISION && abs(second_azi_-new_azi_)<SENSORPRECISION)
    {
        last_azi_=second_azi_;
        second_azi_=new_azi_;
        return false;
    }
    else
    {
        last_azi_=second_azi_;
        second_azi_=new_azi_;
        return true;
    }
}

bool OrientationController::getPanelIsBlocked() const
{
    return panel_is_blocked_;
}

OrientationController::OrientationController()
{
    set_tolerance_band_azimuth(5.5);
    set_adjust_precision_azimuth(3.5);
    set_tolerance_band_tilt(3.0);
    set_adjust_precision_tilt(2.0);
    emergency_stopped=false;
    panel_is_blocked_=false;
    turn_check_time_=0;


}


void OrientationController::begin()
{
    orient_sensor.begin();
    while(!current_time.begin()){}
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
    int wait_counter=0;

    while((millis()<=millicounter_sensor_read+1000) && wait_counter<=20)
    {
        if (orient_sensor.dataAvailable() == true)
        {
            orient_=orient_sensor.get_eulerAngles();
            is_sensor_read=true;
            break;
        }
        else
        {
            delay(50);
            wait_counter++;
        }
    }
    /** If the sensor hangs and does not send new data,
     * stop the motors and try to restart the sensor */
    if (is_sensor_read==false || wait_counter>=20)
    {
        stop_panel();
        emergency_stopped=true;
        orient_sensor.softReset();
        begin();
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


float OrientationController::currentTilt()
{
    return (orient_.y()+OFFSET_TILT);
}
float OrientationController::currentAzimuth()
{
    return fmod((-orient_.x())+OFFSET_AZIMUTH+360.0,360);
}

float OrientationController::deviation_tilt()
{
    float des_tilt;
    float result;

    des_tilt=this->desired_value_tilt();
    result= des_tilt-currentTilt();
    return result;
}

float OrientationController::deviation_azimuth()
{
    float des_azimuth;
    float result;

    des_azimuth=this->desired_value_azimuth();
    result=des_azimuth-currentAzimuth();
    return result;
}

float OrientationController::deviation_tilt_morning_pos()
{
    float des_tilt;
    float result;
    datetime_t srmez;

    srmez= sun_position.get_srmez();
    sun_position.set_mez(srmez);

    des_tilt = MORNING_POS_TILT;
    result = des_tilt-currentTilt();
    return result;
}

float OrientationController::deviation_azimuth_morning_pos()
{
    float des_azimuth;
    float result;
    datetime_t srmez;

    srmez= sun_position.get_srmez();
    sun_position.set_mez(srmez);

    des_azimuth = MORNING_POS_AZIMUTH;//sun_position.get_azang();
    result = des_azimuth-currentAzimuth();
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

    /** Check if panel has turned out of range, if so, stop motors */
    if (currentAzimuth()<LOWER_TURN_ANGLE || currentAzimuth()>UPPER_TURN_ANGLE || emergency_stopped)
    {
        panel_driver.stop_turn_panel();
        is_azi_running_=false;
        direction_turn_=0;
    }
    else
    {
        /** If the panel's motors are runnning, but the panel is not moving,
         * stop the panel */
        if(turn_check_time_+BLOCKINGTIME<millis() && !panel_is_blocked_)
        {
            if (is_azi_running_ && !checkIsPanelTurning())
            {
                stop_panel();
                panel_is_blocked_=true;
            }
            else
                panel_is_blocked_=false;
        }
        if(tilt_check_time_+BLOCKINGTIME<millis() && !panel_is_blocked_)
        {
            if (is_tilt_running_ && !checkIsPanelTilting())
            {
                stop_panel();
                panel_is_blocked_=true;
            }
            else
                panel_is_blocked_=false;
        }

        if (is_daytime() && (!panel_is_blocked_))
        {
            morning_position_tilt_=false;
            morning_position_turn_=false;

            if(this->desired_value_azimuth()>=LOWER_TURN_ANGLE && this->desired_value_azimuth()<=UPPER_TURN_ANGLE)
            {
                /** If the panel's tilt is not good, and the panel is not already
             * in a tilt motion, start tilting the panel in the right direction */
                if ((abs(deviation_tilt())>tolerance_band_tilt_)&& !is_tilt_running_)
                {
                    if ((deviation_tilt()<0))
                    {
                        is_tilt_running_=panel_driver.run_tilt_panel_vertical();
                        direction_tilt_=1;
                    }
                    else if(deviation_tilt()>0)
                    {
                        is_tilt_running_=panel_driver.run_tilt_panel_horizontal();
                        direction_tilt_=2;
                    }
                }
                /** If the panel has reached a good tilt, stop */
                else if (abs(deviation_tilt())<=adjust_precision_tilt_)
                {
                    panel_driver.stop_tilt_panel();
                    is_tilt_running_=false;
                    direction_tilt_=0;
                }
                /** If the panel has tilted too far, stop */
                else if ((deviation_tilt()>0 && direction_tilt_==1) ||
                         deviation_tilt()<0 && direction_tilt_==2)
                {
                    panel_driver.stop_tilt_panel();
                    is_tilt_running_=false;
                    direction_tilt_=0;
                }
                /** If the panel's turn (azimuth) is not good, and the panel is not already
             * in a turning motion, start turning the panel in the right direction */
                if ((abs(deviation_azimuth())>tolerance_band_azimuth_)&& !is_azi_running_)
                {
                    if ((deviation_azimuth()<0))
                    {
                        is_azi_running_=panel_driver.run_turn_panel_cw();
                        direction_turn_=1;
                    }
                    else if(deviation_azimuth()>0)
                    {
                        is_azi_running_=panel_driver.run_turn_panel_ccw();
                        direction_turn_=2;
                    }
                }
                /** If the panel has reached a good turn position, stop */
                else if (abs(deviation_azimuth())<=adjust_precision_azimuth_)
                {
                    panel_driver.stop_turn_panel();
                    is_azi_running_=false;
                    direction_turn_=0;
                }
                /** If the panel has turned too far, stop */
                else if ((deviation_azimuth()>0 && direction_turn_==1) ||
                         deviation_azimuth()<0 && direction_turn_==2)
                {
                    panel_driver.stop_turn_panel();
                    is_azi_running_=false;
                    direction_turn_=0;
                }

            }
        }
    }
    if (currentAzimuth()+5<LOWER_TURN_ANGLE || currentAzimuth()-5>UPPER_TURN_ANGLE)
    {
        panel_driver.stop_turn_panel();
        is_azi_running_=false;
        direction_turn_=0;
    }
    else
    {
        if (!is_daytime())
        {
            if (morning_position_tilt_==false || morning_position_turn_==false)
            {
                /** If the panel's tilt is not good, and the panel is not already
             * in a tilt motion, start tilting the panel in the right direction */
                if ((abs(deviation_tilt_morning_pos())>tolerance_band_tilt_)&& !is_tilt_running_)
                {
                    if ((deviation_tilt_morning_pos()<0))
                    {
                        is_tilt_running_=panel_driver.run_tilt_panel_vertical();
                        direction_tilt_=1;
                    }
                    else if(deviation_tilt_morning_pos()>0)
                    {
                        is_tilt_running_=panel_driver.run_tilt_panel_horizontal();
                        direction_tilt_=2;
                    }
                }
                /** If the panel has reached a good tilt, stop */
                else if (abs(deviation_tilt_morning_pos())<=adjust_precision_tilt_)
                {
                    panel_driver.stop_tilt_panel();
                    is_tilt_running_=false;
                    direction_tilt_=0;
                    morning_position_tilt_=true;
                }
                /** If the panel has tilted too far, stop */
                else if ((deviation_tilt_morning_pos()>0 && direction_tilt_==1) ||
                         deviation_tilt_morning_pos()<0 && direction_tilt_==2)
                {
                    panel_driver.stop_tilt_panel();
                    is_tilt_running_=false;
                    direction_tilt_=0;
                }
                /** If the panel's turn (azimuth) is not good, and the panel is not already
             * in a turning motion, start turning the panel in the cw direction */
                if ((abs(deviation_azimuth_morning_pos())>tolerance_band_azimuth_)&& !is_azi_running_)
                {
                    is_azi_running_=panel_driver.run_turn_panel_cw();
                    direction_turn_=1;
                }
                /** If the panel has reached a good turn position, stop */
                else if (abs(deviation_azimuth_morning_pos())<=tolerance_band_azimuth_+5 ||
                         currentAzimuth()<=LOWER_TURN_ANGLE)
                {
                    panel_driver.stop_turn_panel();
                    is_azi_running_=false;
                    direction_turn_=0;
                    morning_position_turn_=true;
                }
            }
        }

    }
}


void OrientationController::stop_panel()
{
    panel_driver.stop_turn_panel();
    panel_driver.stop_tilt_panel();
    is_azi_running_=false;
    direction_turn_=0;
    is_tilt_running_=false;
    direction_tilt_=0;
}
