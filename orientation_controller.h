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
#define MORNING_POS_AZIMUTH 70 /**< Morning position azimuth angle */
#define MORNING_POS_TILT 80 /**< Morning position tilt angle */
#define SENSOR_GARBAGE_VALUES 5 /**< Number of unreliable sensor values after reset */

/**
 * @brief The OrientationController class provides the functionalities to
 * control the motors and adjust the panel's orientatiion.
 */

#define LOWER_TURN_ANGLE 65     /**< Panel will not turn to a smaller angle */
#define UPPER_TURN_ANGLE 275   /**< Panel will not turn to a bigger angle */

class OrientationController
{
private:
    RtcAdapter current_time;    /**< Object for time reading */
    Sunpos sun_position;        /**< Object for sun positioin determination*/
    datetime_t mez_now_;        /**< Current MEZ */
    OrientationMeasurement orient_sensor;   /**< Object for sensor reading */
    MotorDriver panel_driver;       /**< Object for panel motion control */

    float desired_tilt_;        /**< Desired tilt in degrees  */
    float desired_azimuth_;     /**< Desired azimuth in degrees */
    float deviation_tilt_;      /**< Deviation tilt from desired tilt in degrees */
    float deviation_azimuth_;   /**< Deviation azimuth from desired azimuth */

    int direction_tilt_;        /**< Current tilting direction */
    int direction_turn_;        /**< Current turning direction */

    float tolerance_band_tilt_;     /**< Tolerance band for tilt */
    float tolerance_band_azimuth_;  /**< Tolerance band for azimuth */
    float adjust_precision_tilt_;       /**< Precision band for tilt */
    float adjust_precision_azimuth_;    /**< Precision band for azimuth */

    bool sun_is_up_;    /**< Is the sun up */

    bool is_tilt_running_;  /**< Is the panel tilting */
    bool is_azi_running_;   /**< Is the panel turning */

    float last_azi_,new_azi_,second_azi_;   /**< Values for azimuth blocking detection */
    float last_tilt_,new_tilt_,second_tilt_;    /**< Values for tilt blocking detection */

    bool morning_position_tilt_;    /**< Tilt in morning position in degrees */
    bool morning_position_turn_;    /**< Azimuth in morning position in degrees */

    bool emergency_stopped_;    /**< Is emergency stop active */

    unsigned long turn_check_time_; /**< time for azimuth blocking detection */
    unsigned long tilt_check_time_; /**< time for tilt blocking detection */

    bool panel_is_blocked_; /**< Is blocking stop active */

    imu::Vector<3> orient_; /**< Current orientation */

    int sensor_read_counter_; /**< Contains the number of sensor reads sonce restart */

public:
    /**
     * @brief Default constructor sets tolerance band and
     * adjust precision to predefined values.
     *
     * Tolerance band should always be greater or equal to the
     * corresponding adjust precision.
     */
    OrientationController();



    /**
     * @brief is_daytime - Checks if the sun has risen
     * @return Returns true, if it is between sunrise and sunset
     */
    bool isDaytime();

    /**
     * @brief Begin RTC and sensor
     */
    void begin();

    /**
     * @brief Get the current offset corrected tilt angle of the panel
     * @return current tilt angle of the panel in degrees
     */
    float currentTilt();

    /**
     * @brief Get the current offset corrected azimuth angle of the panel
     * @return current azimuth angle in degrees
     */
    float currentAzimuth();

    /**
     * @brief Calculate the desired tilt angle for the current time and day
     * @return Desired tilt angle in degrees
     */
    float desiredValueTilt();

    /**
     * @brief Calculate the desired azimut angle for the current time and day
     * @return Desired azimut angle in degrees
     */
    float desiredValueAzimuth();

    /**
     * @brief Get the deviation of the tilt angle (desired tilt - current tilt)
     * @return deviation of the tilt angle in degrees
     */
    float deviationTilt();

    /**
     * @brief Get the deviation of the azimuth angle (desired azimuth - current azimuth)
     * @return deviation of the azimuth angle in degrees
     */
    float deviationAzimuth();

    /**
     * @brief Set the tilt tolerance band
     * @param Tilt tolerance band in degrees
     */
    void setToleranceBandTilt(float tilt_tol);

    /**
     * @brief Get the tilt tolerance band
     * @return Tilt tolerance band in degrees
     */
    float getToleranceBandTilt() const;

    /**
     * @brief Set the azimuth tolerance band
     * @param Azimuth tolerance band in degrees
     */
    void setToleranceBandAzimuth(float azi_tol);

    /**
     * @brief Get the azimuth tolerance band
     * @return Azimuth tolerance band in degrees
     */
    float getToleranceBandAzimuth() const;

    /**
     * @brief Set the adjust precision band for tilt
     * @param tilt_tol Tilt precision in degrees
     */
    void setAdjustPrecisionTilt(float tilt_tol);

    /**
     * @brief Get the adjust pprecision band for tilt
     * @return Tilt adjustment precision in degrees
     */
    float getAdjustPrecisionTilt() const;

    /**
     * @brief Set the adjust precision band for azimuth
     * @param azi_tol Azimuth precision in degrees
     */
    void setAdjustPrecisionAzimuth(float azi_tol);

    /**
     * @brief Get the adjust pprecision band for azimuth
     * @return Azimuth adjustment precision in degrees
     */
    float getAdjustPrecisionAzimuth() const;

    /**
     * @brief Main state machine for the panel orientation
     */
    void orientPanel();

    /**
     * @brief Stop the panel's motion
     */
    void stopPanel();

    /**
     * @brief getMezNow
     * @return
     */
    datetime_t getMezNow() const;

    /**
     * @brief readSensorData
     */
    void readSensorData();

    /**
     * @brief getIsAziRunning
     * @return
     */
    bool getIsAziRunning() const;

    /**
     * @brief getIsTiltRunning
     * @return
     */
    bool getIsTiltRunning() const;

    /**
     * @brief getSunIsUp
     * @return
     */
    bool getSunIsUp() const;

    /**
     * @brief deviationTiltMorningPos
     * @return
     */
    float deviationTiltMorningPos();

    /**
     * @brief deviationAzimuthMorningPos
     * @return
     */
    float deviationAzimuthMorningPos();

    /**
     * @brief getEmergencyStopped
     * @return
     */
    bool getEmergencyStopped() const;

    /**
     * @brief checkIsPanelTilting
     * @return
     */
    bool checkIsPanelTilting();

    /**
     * @brief checkIsPanelTurning
     * @return
     */
    bool checkIsPanelTurning();

    /**
     * @brief getPanelIsBlocked
     * @return
     */
    bool getPanelIsBlocked() const;
};

#endif // ORIENTATION_CONTROLLER_H
