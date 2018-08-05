#ifndef ORIENTATIONCONTROLLER_H
#define ORIENTATIONCONTROLLER_H

#include "rtcadapter.h"
#include "sunpos.h"
#include "orientation_measure.h"
#include <math.h>
#include "motordriver.h"
#include <Arduino.h>

#define LOWER_TURN_ANGLE 65     /**< Panel will not turn to a smaller angle */
#define UPPER_TURN_ANGLE 275 /**< Panel will not turn to a bigger angle */
#define SENSORPRECISION 1   /**< Precision of the sensor */
#define BLOCKINGTIME 2500 /**< Time until blocking is detected in millisec */
#define MORNING_POS_AZIMUTH 70
#define MORNING_POS_TILT 80

/**
 * @brief The OrientationController class provides the functionalities to
 * control the motors and adjust the panel's orientatiion.
 */

#define LOWER_TURN_ANGLE 65     /**< Panel will not turn to a smaller angle */
#define UPPER_TURN_ANGLE 275   /**< Panel will not turn to a bigger angle */

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

    int direction_tilt_;
    int direction_turn_;

    float tolerance_band_tilt_;
    float tolerance_band_azimuth_;
    float adjust_precision_azimuth_;
    float adjust_precision_tilt_;

    bool sun_is_up_;

    bool is_tilt_running_;
    bool is_azi_running_;

    float last_azi_,new_azi_,second_azi_;
    float last_tilt_,new_tilt_,second_tilt_;

    bool morning_position_tilt_;
    bool morning_position_turn_;

    bool emergency_stopped_;

    unsigned long turn_check_time_;
    unsigned long tilt_check_time_;

    bool panel_is_blocked_;



public:
    /**
     * @brief Default constructor sets tolerance band and
     * adjust precision to predefined values.
     *
     * Tolerance band should always be greater or equal to the
     * corresponding adjust precision.
     */
    OrientationController();

        imu::Vector<3> orient_;

    /**
     * @brief is_daytime - Checks if the sun has risen
     * @return Returns true, if it is between sunrise and sunset
     */
    bool is_daytime();

    /**
     * @brief Begins RTC and sensor
     */
    void begin();

    /**
     * @brief Gets the current offset corrected tilt angle of the panel
     * @return current tilt angle of the panel in degrees
     */
    float currentTilt();

    /**
     * @brief Gets the current offset corrected azimuth angle of the panel
     * @return current azimuth angle in degrees
     */
    float currentAzimuth();

    /**
     * @brief Calculates the desired tilt angle for the current time and day
     * @return Desired tilt angle in degrees
     */
    float desiredValueTilt();

    /**
     * @brief Calculates the desired azimut angle for the current time and day
     * @return Desired azimut angle in degrees
     */
    float desiredValueAzimuth();

    /**
     * @brief Gets the deviation of the tilt angle (desired tilt - current tilt)
     * @return deviation of the tilt angle in degrees
     */
    float deviation_tilt();

    /**
     * @brief Gets the deviation of the azimuth angle (desired azimuth - current azimuth)
     * @return deviation of the azimuth angle in degrees
     */
    float deviation_azimuth();

    /**
     * @brief Sets the tilt tolerance band
     * @param Tilt tolerance band in degrees
     */
    void set_tolerance_band_tilt(float tilt_tol);

    /**
     * @brief Gets the tilt tolerance band
     * @return Tilt tolerance band in degrees
     */
    float get_tolerance_band_tilt() const;

    void set_tolerance_band_azimuth(float azi_tol);
    float get_tolerance_band_azimuth() const;

    void set_adjust_precision_tilt(float tilt_tol);
    float get_adjust_precision_tilt() const;

    void set_adjust_precision_azimuth(float azi_tol);
    float get_adjust_precision_azimuth() const;

    void orient_panel();
    void stop_panel();

    datetime_t getMez_now() const;

    void readSensorData();
    bool getIsAziRunning() const;
    bool getIsTiltRunning() const;
    bool getSunIsUp() const;

    float deviation_tilt_morning_pos();
    float deviation_azimuth_morning_pos();

    bool getEmergencyStopped() const;

    bool checkIsPanelTilting();
    bool checkIsPanelTurning();
    bool getPanelIsBlocked() const;
};

#endif // ORIENTATION_CONTROLLER_H
