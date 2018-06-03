#ifndef ORIENTATIONCONTROLLER_H
#define ORIENTATIONCONTROLLER_H

#include "rtcadapter.h"
#include "sunpos.h"
#include "orientation_measure.h"
#include <math.h>
#include "motordriver.h"
#include <Arduino.h>

/**
 * @brief The OrientationController class provides the functionalities to
 * control the motors and adjust the panel's orientatiion.
 */
class OrientationController
{
private:
    RtcAdapter current_time;
    Sunpos sun_position;
    datetime_t mez_now_;
    OrientationMeasurement orient_sensor;
    MotorDriver panel_driver;

    float desired_tilt_;
    float desired_azimuth_;
    float deviation_tilt_;
    float deviation_azimuth_;

    float tolerance_band_tilt_;
    float tolerance_band_azimuth_;

    bool sun_is_up_;

    bool is_tilt_running_;
    bool is_azi_running_;



public:
    OrientationController();


    /**
     * @brief is_daytime - Checks if the sun has risen
     * @return Returns true, if it is between sunrise and sunset
     */
    bool is_daytime();

    /**
     * @brief Calculates the desired tilt angle for the current time and day
     * @return Desired tilt angle in degrees
     */
    float desired_value_tilt();

    /**
     * @brief Calculates the desired azimut angle for the current time and day
     * @return Desired azimut angle in degrees
     */
    float desired_value_azimuth();

    float deviation_tilt();
    float deviation_azimuth();

    void set_tolerance_band_tilt(float tilt_tol);
    float get_tolerance_band_tilt() const;

    void set_tolerance_band_azimuth(float azi_tol);
    float get_tolerance_band_azimuth() const;

    void orient_panel();
    void stop_panel();

    datetime_t getMez_now() const;
};

#endif // ORIENTATION_CONTROLLER_H