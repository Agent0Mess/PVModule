#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <Arduino.h>

class MotorDriver
{
private:
    int rel_1_=7;     /**< Standard relay 1 pin is 7 */
    int rel_2_=6;     /**< Standard relay 2 pin is 6 */
    int brake_A_=9;   /**< Standard brake pin for linear actuator is 9 */
    int direct_A_=12; /**< Standard direction pin for linear actuator is 12 */
    int speed_A_=3;   /**< Standard speed pin for linear actuator is 3; */

    int rel_;           /**< Contains current relay status */
    int direct_;        /**< Contains current linear actor status */

public:
    /**
     * @brief Standard constructor; Uses predefined pin configuration.
     */
    MotorDriver();

    /**
     * @brief Constructer where the pin configuration is passed as an argument
     * @param rel_1 - Pin for relay 1
     * @param rel_2 - Pin for relay 2
     * @param brake_A - Pin for brake for linear actuator
     * @param direct_A - Pin for direction for linear actuator
     * @param speed_A - Pin for speed for linear actuator
     */
    MotorDriver(int rel_1, int rel_2, int brake_A, int direct_A, int speed_A );

    /**
     * @brief Declares the pins in question as OUTPUTs
     */
    void setup_pins();

    /**
     * @brief Extends linear actuator, so that the panel tilts. Tilt angle decreases.
     *
     * The panel must not be tilting already when calling this function.
     * If the panel is still tilting in any direction when calling this function,
     * something must have gone wrong.
     * The panel will then simply stop tilting for safety reasons.
     */
    void run_tilt_panel_horizontal();

    /**
     * @brief Retracts linear actuator, so that the panel tilts. Tilt angle increases.
     *
     * The panel must not be tilting already when calling this function.
     * If the panel is still tilting in any direction when calling this function,
     * something must have gone wrong.
     * The panel will then simply stop tilting for safety reasons.S
     */
    void run_tilt_panel_vertical();

    /**
     * @brief Stops panel's tilt movement
     */
    void stop_tilt_panel();

    /**
     * @brief Turns panel clockwise (i.e. from east to west). Azimuth angle decreases.
     *
     * The panel must not be turning already when calling this function.
     * If the panel is still turning in any direction when calling this function,
     * something must have gone wrong.
     * The panel will then simply stop turning for safety reasons.
     */
    void run_turn_panel_cw();

    /**
     * @brief Turns panel counter-clockwise (i.e. from west to east).
     * Azimuth angle decreases.
     *
     * The panel must not be turning already when calling this function.
     * If the panel is still turning in any direction when calling this function,
     * something must have gone wrong.
     * The panel will then simply stop turning for safety reasons.
     */
    void run_turn_panel_ccw();

    /**
     * @brief Stop panel's turn movement
     */
    void stop_turn_panel();

};

#endif // MOTORDRIVER_H
