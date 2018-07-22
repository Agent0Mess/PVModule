#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "orientation_measure.h"
#include "rtcadapter.h"
#include "motordriver.h"
#include "orientation_controller.h"
#include "sunpos.h"
//#include <EEPROM.h>
#include <LiquidCrystal.h>

#define DEBUG
#define LCD
#define RESET_CALIBRATION

imu::Vector<3> orient;

OrientationMeasurement sensor_mes = OrientationMeasurement();
RtcAdapter MyRTC;
OrientationController PanelControl;


int eeAddress = 0; /**< Address for calibration data in EEPROM*/

void setup(void)
{
    Serial.begin(9600);

    PanelControl.begin();

//        if (! MyRTC.begin()) {
//          Serial.println("Couldn't find RTC");
//          while (1);
//        }

    Serial.println("Orientation Sensor Test"); Serial.println("");
    PanelControl.stop_panel();
}

void loop(void)
{
        Serial.print(" Is it daytime? ");
        Serial.println(PanelControl.is_daytime());
        Serial.print("\t Euler X: ");
        Serial.print(PanelControl.orient_.x(), 4);
        Serial.print("\t Euler Y: ");
        Serial.print(PanelControl.orient_.y(), 4);
        Serial.print("\t Euler Z: ");
        Serial.print(PanelControl.orient_.z(), 4);
        Serial.println("");

        Serial.print("\t Deviation Tilt: ");
        Serial.print(PanelControl.deviation_tilt(), 4);
        Serial.print("\t Deviation Azimuth: ");
        Serial.print(PanelControl.deviation_azimuth(), 4);
        Serial.println("");

        Serial.print("\t Deviation Tilt Morning Pos: ");
        Serial.print(PanelControl.deviation_tilt_morning_pos(), 4);
        Serial.print("\t Deviation Azimuth Morning Pos: ");
        Serial.print(PanelControl.deviation_azimuth_morning_pos(), 4);
        Serial.println("");

        Serial.print("\t Desited Tilt: ");
        Serial.print(PanelControl.desired_value_tilt(), 4);
        Serial.print("\t Desired Azimuth: ");
        Serial.print(PanelControl.desired_value_azimuth(), 4);
        Serial.println("");


        Serial.print("Azi is running:  ");
        Serial.println(PanelControl.getIsAziRunning());

        Serial.print("Tilt is running:  ");
        Serial.println(PanelControl.getIsTiltRunning());

        if(PanelControl.getEmergencyStopped())
        {
            Serial.println("----------------Emergency stop------------------------");
        }
        if(PanelControl.getPanelIsBlocked())
        {
            Serial.println("----------------Panel is blocked------------------------");
        }

        PanelControl.orient_panel();

}


/**************************************************************************/
/*
    Display sensor calibration status
    */
/**************************************************************************/
//void displayCalStatus(void)
//{
//    /* Get the four calibration values (0..3) */
//    /* Any sensor data reporting 0 should be ignored, */
//    /* 3 means 'fully calibrated" */
//    uint8_t system, gyro, accel, mag;
//    system = gyro = accel = mag = 0;
//    sensor_mes.getCalibration(&system, &gyro, &accel, &mag);

//    /* The data should be ignored until the system calibration is > 0 */
//    Serial.print("\t");
//    if (!system)
//    {
//        Serial.print("! ");
//    }

//    /* Display the individual values */
//    Serial.print("Sys:");
//    Serial.print(system, DEC);
//    Serial.print(" G:");
//    Serial.print(gyro, DEC);
//    Serial.print(" A:");
//    Serial.print(accel, DEC);
//    Serial.print(" M:");
//    Serial.print(mag, DEC);
//}

///**************************************************************************/
///*
//    Display the raw calibration offset and radius data
//    */
///**************************************************************************/
//void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
//{
//    Serial.println("Accelerometer: ");
//    Serial.print(calibData.accel_offset_x); Serial.print(" ");
//    Serial.print(calibData.accel_offset_y); Serial.print(" ");
//    Serial.print(calibData.accel_offset_z); Serial.print(" ");

//    Serial.println("\nGyro: ");
//    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
//    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
//    Serial.print(calibData.gyro_offset_z); Serial.print(" ");

//    Serial.println("\nMag: ");
//    Serial.print(calibData.mag_offset_x); Serial.print(" ");
//    Serial.print(calibData.mag_offset_y); Serial.print(" ");
//    Serial.print(calibData.mag_offset_z); Serial.print(" ");

//    Serial.println("\nAccel Radius: ");
//    Serial.print(calibData.accel_radius);

//    Serial.println("\nMag Radius: ");
//    Serial.print(calibData.mag_radius);
//}

///**************************************************************************/
///*
//    Update Calibration
//    */
///**************************************************************************/

//void updateCalibration (void) {
//    /** Turn off panel for savety. It should not be moving when
//            loading a new calibration */
//    PanelControl.stop_panel();

//    /** Perform a basic check if calibration data has changed */
//    if ((sensor_offsets_load.accel_offset_x != current_calib.accel_offset_x) || \
//            (sensor_offsets_load.mag_offset_x != current_calib.mag_offset_x) || \
//            (sensor_offsets_load.gyro_offset_x != current_calib.gyro_offset_x) && \
//            (sensor_offsets_load.mag_offset_x>0)){
//        /** New calibration data is filtered through a simple IIR filter */
//        sensor_offsets= (sensor_offsets_load*19 + current_calib)/20;
//        EEPROM.put(eeAddress, sensor_offsets);
//        delay(300);
//        EEPROM.get(eeAddress, sensor_offsets_load);
//        delay(300);
//        sensor_mes.setMode(sensor_mes.OPERATION_MODE_CONFIG);
//        delay(300);
//        sensor_mes.setSensorOffsets(sensor_offsets_load);
//        delay(300);
//        sensor_mes.setMode(sensor_mes.OPERATION_MODE_NDOF_FMC_OFF);
//    }
//}
