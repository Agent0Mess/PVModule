#include "motordriver.h"

MotorDriver::MotorDriver()
{ 
    setup_pins();
}

MotorDriver::MotorDriver(int rel_1, int rel_2, int brake_A, int direct_A, int speed_A )
{
    rel_1_=rel_1;
    rel_2_=rel_2;
    brake_A_=brake_A;
    direct_A_=direct_A;
    speed_A_=speed_A;

    setup_pins();
}

void MotorDriver::setup_pins()
{
    pinMode(rel_1_, OUTPUT);
    pinMode(rel_2_, OUTPUT);
    pinMode(brake_A_, OUTPUT);
    pinMode(direct_A_, OUTPUT);
}

bool MotorDriver::run_tilt_panel_horizontal()
{
    if (millicounter_tilt_+1000<=millis())
    {
        if (direct_==0) {
            digitalWrite(direct_A_, HIGH);
            digitalWrite(brake_A_, LOW);
            analogWrite(speed_A_, 255);
            direct_ = 1;                //direction is forward
            return true;

        }
        else {
            stop_tilt_panel();
            return false;
        }
    }
}


bool MotorDriver::run_tilt_panel_vertical()
{
    if (millicounter_tilt_+1000<=millis())
    {
        if (direct_==0)
        {
            digitalWrite(direct_A_, LOW);
            digitalWrite(brake_A_, LOW);
            analogWrite(speed_A_, 255);
            direct_ = 2;                //direction is backward
            return true;
        }
        else {
            stop_tilt_panel();
            return false;
        }
    }
}

void MotorDriver::stop_tilt_panel()
{
    digitalWrite(speed_A_, 0);      // Channel A, speed = zero
    analogWrite(brake_A_, HIGH);    // Channel A, engage brake
    millicounter_tilt_=millis();
}

bool MotorDriver::run_turn_panel_cw()
{
    if (millicounter_azi_+1000<=millis())
    {
        if (rel_==0) {
            digitalWrite(rel_1_, HIGH);
            rel_ = 1;
            return true;
        }
        else
            stop_turn_panel();
            return false;
    }
}

bool MotorDriver::run_turn_panel_ccw()
{
    if (millicounter_azi_+1000<=millis())
    {
        if (rel_==0) {
            digitalWrite(rel_2_, HIGH);
            rel_ = 2;
            return true;
        }
        else
            stop_turn_panel();
            return false;
    }
}

void MotorDriver::stop_turn_panel()
{
    digitalWrite(rel_1_, LOW);
    digitalWrite(rel_2_, LOW);
    rel_ = 0;
    millicounter_azi_=millis();
}
