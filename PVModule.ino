#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "orientation_measure.h"
#include "rtcadapter.h"
#include "motordriver.h"
#include "orientation_controller.h"
#include "sunpos.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>

#define DEBUG
#define LCD
#define RESET_CALIBRATION


Adafruit_BNO055 bno = Adafruit_BNO055(55);
imu::Vector<3> orient;

offset_data_t sensor_offsets, sensor_offsets_load, current_calib;

OrientationMeasurement sensor_mes = OrientationMeasurement();
RtcAdapter MyRTC;
OrientationController PanelControl;

#ifdef LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#endif

int eeAddress = 0; /**< Address for calibration data in EEPROM*/

void setup(void)
{
    Serial.begin(9600);

    #ifdef LCD
    lcd.begin(16, 2);
    #endif

    if (! MyRTC.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }

    Serial.println("Orientation Sensor Test"); Serial.println("");

    /* Initialise the sensor */
    if(!bno.begin()){
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }

    delay(500);

    #ifdef RESET_CALIBRATION
    sensor_offsets = sensor_mes.load_calibration_data();
    EEPROM.put(eeAddress, sensor_offsets);
    delay(500);
    #endif

    EEPROM.get(eeAddress, sensor_offsets_load);

    sensor_mes.setMode(sensor_mes.OPERATION_MODE_CONFIG);
    delay(500);
    sensor_mes.setExtCrystalUse(true);
    Serial.println("Loading Calibartion Data");
    sensor_mes.setSensorOffsets(sensor_offsets_load);
    delay(500);
    sensor_mes.setMode(sensor_mes.OPERATION_MODE_NDOF_FMC_OFF);
}

void loop(void)
{
    #ifdef DEBUG
    orient=sensor_mes.get_eulerAngles();
    Serial.print("\t Euler X: ");
    Serial.print(orient.x(), 4);
    Serial.print("\t Euler Y: ");
    Serial.print(orient.y(), 4);
    Serial.print("\t Euler Z: ");
    Serial.print(orient.z(), 4);
    Serial.println("");
    #endif

    #ifdef LCD
    lcd.setCursor(0, 0);
    lcd.print("Tilt");
    lcd.setCursor(11, 0);
    lcd.print(orient.z());
    lcd.setCursor(0, 1);
    lcd.print("Comp");
    lcd.setCursor(11, 1);
    lcd.print(orient.x());
    #endif

    PanelControl.orient_panel();
    sensor_mes.getSensorOffsets(current_calib);

    #ifdef DEBUG
    displaySensorOffsets(current_calib);
    displayCalStatus();
    #endif

    updateCalibration();

}


/**************************************************************************/
/*
    Display sensor calibration status
    */
/**************************************************************************/
void displayCalStatus(void)
{
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    sensor_mes.getCalibration(&system, &gyro, &accel, &mag);

    /* The data should be ignored until the system calibration is > 0 */
    Serial.print("\t");
    if (!system)
    {
        Serial.print("! ");
    }

    /* Display the individual values */
    Serial.print("Sys:");
    Serial.print(system, DEC);
    Serial.print(" G:");
    Serial.print(gyro, DEC);
    Serial.print(" A:");
    Serial.print(accel, DEC);
    Serial.print(" M:");
    Serial.print(mag, DEC);
}

/**************************************************************************/
/*
    Display the raw calibration offset and radius data
    */
/**************************************************************************/
void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial.println("Accelerometer: ");
    Serial.print(calibData.accel_offset_x); Serial.print(" ");
    Serial.print(calibData.accel_offset_y); Serial.print(" ");
    Serial.print(calibData.accel_offset_z); Serial.print(" ");

    Serial.println("\nGyro: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(" ");
    Serial.print(calibData.gyro_offset_y); Serial.print(" ");
    Serial.print(calibData.gyro_offset_z); Serial.print(" ");

    Serial.println("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(" ");
    Serial.print(calibData.mag_offset_y); Serial.print(" ");
    Serial.print(calibData.mag_offset_z); Serial.print(" ");

    Serial.println("\nAccel Radius: ");
    Serial.print(calibData.accel_radius);

    Serial.println("\nMag Radius: ");
    Serial.print(calibData.mag_radius);
}

/**************************************************************************/
/*
    Update Calibration
    */
/**************************************************************************/

void updateCalibration (void) {
    /** Turn off panel for savety. It should not be moving when
            loading a new calibration */
    PanelControl.stop_panel();

    /** Perform a basic check if calibration data has changed */
    if ((sensor_offsets_load.accel_offset_x != current_calib.accel_offset_x) || \
            (sensor_offsets_load.mag_offset_x != current_calib.mag_offset_x) || \
            (sensor_offsets_load.gyro_offset_x != current_calib.gyro_offset_x) && \
            (sensor_offsets_load.mag_offset_x>0)){
        /** New calibration data is filtered through a simple IIR filter */
        sensor_offsets= (sensor_offsets_load*19 + current_calib)/20;
        EEPROM.put(eeAddress, sensor_offsets);
        delay(300);
        EEPROM.get(eeAddress, sensor_offsets_load);
        delay(300);
        sensor_mes.setMode(sensor_mes.OPERATION_MODE_CONFIG);
        delay(300);
        sensor_mes.setSensorOffsets(sensor_offsets_load);
        delay(300);
        sensor_mes.setMode(sensor_mes.OPERATION_MODE_NDOF_FMC_OFF);
    }
}
