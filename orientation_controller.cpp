#include <math.h>
#include "orientation_controller.h"

/** Depending on the mecanical construction of the panel
 * and the position of the sensor */
#define OFFSET_TILT 61.0
#define OFFSET_AZIMUTH 82.0

datetime_t OrientationController::getMezNow() const
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
    return emergency_stopped_;
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
    setToleranceBandAzimuth(5.5);
    setAdjustPrecisionAzimuth(3.5);
    setToleranceBandTilt(3.0);
    setAdjustPrecisionTilt(2.0);
    emergency_stopped_=false;
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

bool OrientationController::isDaytime()
{
    mez_now_=current_time.readTime();
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
            emergency_stopped_=false;
            break;
        }
        else
        {
            /** If millis() overflows, this prevents the loop from running too long */
            delay(50);
            wait_counter++;
        }
    }
    /** If the sensor hangs and does not send new data,
     * stop the motors and try to restart the sensor */
    if (is_sensor_read==false || wait_counter>=20)
    {
        stopPanel();
        emergency_stopped_=true;
        orient_sensor.softReset();
        begin();
    }
}

float OrientationController::desiredValueTilt()
{
    mez_now_=current_time.readTime();
    sun_position.set_mez(mez_now_);
    return sun_position.get_pvtiltang();
}

float OrientationController::desiredValueAzimuth()
{
    mez_now_=current_time.readTime();
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

float OrientationController::deviationTilt()
{
    float des_tilt;
    float result;

    des_tilt=this->desiredValueTilt();
    result= des_tilt-currentTilt();
    return result;
}

float OrientationController::deviationAzimuth()
{
    float des_azimuth;
    float result;

    des_azimuth=this->desiredValueAzimuth();
    result=des_azimuth-currentAzimuth();
    return result;
}

float OrientationController::deviationTiltMorningPos()
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

float OrientationController::deviationAzimuthMorningPos()
{
    float des_azimuth;
    float result;
    datetime_t srmez;

    srmez= sun_position.get_srmez();
    sun_position.set_mez(srmez);

    des_azimuth = MORNING_POS_AZIMUTH;
    result = des_azimuth-currentAzimuth();
    return result;
}

void OrientationController::setToleranceBandTilt(float tilt_tol)
{
    tolerance_band_tilt_=tilt_tol;
}
float OrientationController::getToleranceBandTilt() const
{
    return tolerance_band_tilt_;
}

void OrientationController::setToleranceBandAzimuth(float azi_tol)
{
    tolerance_band_azimuth_=azi_tol;
}
float OrientationController::getToleranceBandAzimuth() const
{
    return tolerance_band_azimuth_;
}

void OrientationController::setAdjustPrecisionTilt(float tilt_adj)
{
    adjust_precision_tilt_=tilt_adj;
}
float OrientationController::getAdjustPrecisionTilt() const
{
    return adjust_precision_tilt_;
}

void OrientationController::setAdjustPrecisionAzimuth(float azi_adj)
{
    adjust_precision_azimuth_=azi_adj;
}
float OrientationController::getAdjustPrecisionAzimuth() const
{
    return adjust_precision_azimuth_;
}

void OrientationController::orientPanel()
{
    this->readSensorData();

    /** Check if panel has turned out of range, if so, stop motors */
    if (currentAzimuth()<LOWER_TURN_ANGLE || currentAzimuth()>UPPER_TURN_ANGLE || emergency_stopped_)
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
                stopPanel();
                panel_is_blocked_=true;
            }
            else
                panel_is_blocked_=false;
        }
        if(tilt_check_time_+BLOCKINGTIME<millis() && !panel_is_blocked_)
        {
            if (is_tilt_running_ && !checkIsPanelTilting())
            {
                stopPanel();
                panel_is_blocked_=true;
            }
            else
                panel_is_blocked_=false;
        }

        if (isDaytime() && (!panel_is_blocked_))
        {
            morning_position_tilt_=false;
            morning_position_turn_=false;

            if(this->desiredValueAzimuth()>=MORNING_POS_AZIMUTH && this->desiredValueAzimuth()<=UPPER_TURN_ANGLE)
            {
                /** If the panel's tilt is not good, and the panel is not already
                 * in a tilt motion, start tilting the panel in the right direction */
                if ((abs(deviationTilt())>tolerance_band_tilt_)&& !is_tilt_running_)
                {
                    if ((deviationTilt()<0))
                    {
                        is_tilt_running_=panel_driver.run_tilt_panel_vertical();
                        direction_tilt_=1;
                    }
                    else if(deviationTilt()>0)
                    {
                        is_tilt_running_=panel_driver.run_tilt_panel_horizontal();
                        direction_tilt_=2;
                    }
                }
                /** If the panel has reached a good tilt, stop */
                else if (abs(deviationTilt())<=adjust_precision_tilt_)
                {
                    panel_driver.stop_tilt_panel();
                    is_tilt_running_=false;
                    direction_tilt_=0;
                }
                /** If the panel has tilted too far, stop */
                else if ((deviationTilt()>0 && direction_tilt_==1) ||
                         deviationTilt()<0 && direction_tilt_==2)
                {
                    panel_driver.stop_tilt_panel();
                    is_tilt_running_=false;
                    direction_tilt_=0;
                }
                /** If the panel's turn (azimuth) is not good, and the panel is not already
                  * in a turning motion, start turning the panel in the right direction */
                if ((abs(deviationAzimuth())>tolerance_band_azimuth_)&& !is_azi_running_)
                {
                    if ((deviationAzimuth()<0))
                    {
                        is_azi_running_=panel_driver.run_turn_panel_cw();
                        direction_turn_=1;
                    }
                    else if(deviationAzimuth()>0)
                    {
                        is_azi_running_=panel_driver.run_turn_panel_ccw();
                        direction_turn_=2;
                    }
                }
                /** If the panel has reached a good turn position, stop */
                else if (abs(deviationAzimuth())<=adjust_precision_azimuth_)
                {
                    panel_driver.stop_turn_panel();
                    is_azi_running_=false;
                    direction_turn_=0;
                }
                /** If the panel has turned too far, stop */
                else if ((deviationAzimuth()>0 && direction_turn_==1) ||
                         deviationAzimuth()<0 && direction_turn_==2)
                {
                    panel_driver.stop_turn_panel();
                    is_azi_running_=false;
                    direction_turn_=0;
                }

            }
        }
    }
    if (currentAzimuth()<LOWER_TURN_ANGLE || currentAzimuth()>UPPER_TURN_ANGLE)
    {
        panel_driver.stop_turn_panel();
        is_azi_running_=false;
        direction_turn_=0;
    }
    else
    {
        if (!isDaytime())
        {
            /** If the panel is not already in its morning position */
            if (morning_position_tilt_==false || morning_position_turn_==false)
            {
                /** If the panel's tilt is not good, and the panel is not already
                * in a tilt motion, start tilting the panel in the right direction */
                if ((abs(deviationTiltMorningPos())>tolerance_band_tilt_)&& !is_tilt_running_)
                {
                    if ((deviationTiltMorningPos()<0))
                    {
                        is_tilt_running_=panel_driver.run_tilt_panel_vertical();
                        direction_tilt_=1;
                    }
                    else if(deviationTiltMorningPos()>0)
                    {
                        is_tilt_running_=panel_driver.run_tilt_panel_horizontal();
                        direction_tilt_=2;
                    }
                }
                /** If the panel has reached a good tilt, stop */
                else if (abs(deviationTiltMorningPos())<=adjust_precision_tilt_)
                {
                    panel_driver.stop_tilt_panel();
                    is_tilt_running_=false;
                    direction_tilt_=0;
                    morning_position_tilt_=true;
                }
                /** If the panel has tilted too far, stop */
                else if ((deviationTiltMorningPos()>0 && direction_tilt_==1) ||
                         deviationTiltMorningPos()<0 && direction_tilt_==2)
                {
                    panel_driver.stop_tilt_panel();
                    is_tilt_running_=false;
                    direction_tilt_=0;
                }
                /** If the panel's turn (azimuth) is not good, and the panel is not already
                  * in a turning motion, start turning the panel in the cw direction */
                if ((abs(deviationAzimuthMorningPos())>tolerance_band_azimuth_)&& !is_azi_running_)
                {
                    is_azi_running_=panel_driver.run_turn_panel_cw();
                    direction_turn_=1;
                }
                /** If the panel has reached a good turn position, stop */
                else if (abs(deviationAzimuthMorningPos())<=tolerance_band_azimuth_)
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


void OrientationController::stopPanel()
{
    panel_driver.stop_turn_panel();
    panel_driver.stop_tilt_panel();
    is_azi_running_=false;
    direction_turn_=0;
    is_tilt_running_=false;
    direction_tilt_=0;
}
