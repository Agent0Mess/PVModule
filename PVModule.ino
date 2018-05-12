#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "orientation_measure.h"
#include "rtcadapter.h"
#include "motordriver.h"
/**
Adafruit_BNO055 bno = Adafruit_BNO055(55);
imu::Vector<3> orient;
uint8_t syscal;
uint8_t gyrocal;
uint8_t accelcal;
uint8_t magcal;
adafruit_bno055_offsets_t sensor_offsets;
OrientationMeasurement sensor_mes = OrientationMeasurement();
*/

RtcAdapter MyRTC;
datetime_t current_time;

void setup(void)
{
    Serial.begin(9600);

    if (! MyRTC.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }

/**    Serial.println("Orientation Sensor Test"); Serial.println("");

    /* Initialise the sensor
    if(!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }

    delay(1000);

    bno.setExtCrystalUse(true);
    bno.setMode(bno.OPERATION_MODE_NDOF);
    sensor_mes.load_calibration_data();
    */
}

void loop(void)
{
    current_time=MyRTC.read_time();
    Serial.print("Hour: ");
    Serial.println(current_time.hour);
    Serial.print("Minute: ");
    Serial.println(current_time.min);
    Serial.print("Day of Year: ");
    Serial.println(current_time.doy);

/**
    // Get a new sensor event
    sensors_event_t event;
    bno.getEvent(&event);
    orient=sensor_mes.get_eulerAngles();

    // Display the floating point data
    Serial.print("Direct Euler X: ");
    Serial.print(event.orientation.x, 4);
    Serial.print("\tQuaternion to Euler X: ");
    Serial.print(orient.x(), 4);
    Serial.print("\nDirect Euler Y: ");
    Serial.print(event.orientation.y, 4);
    Serial.print("\tQuaternion to Euler Y: ");
    Serial.print(orient.y(), 4);
    Serial.print("\nDirect Euler Z: ");
    Serial.print(event.orientation.z, 4);
    Serial.print("\tQuaternion to Euler Z: ");
    Serial.print(orient.z(), 4);
    Serial.println("");

    delay(100);

    if (bno.isFullyCalibrated()) {
        Serial.println("Calibration complete");
    }
    else {
        bno.getCalibration(&syscal, &gyrocal, &accelcal, &magcal);
        Serial.println("Sensor is not fully calibrated");
        Serial.print("Calibration Status is: ");
        Serial.print(syscal);
        Serial.print(gyrocal);
        Serial.print(accelcal);
        Serial.println(magcal);
    }
    sensor_mes.getSensorOffsets(sensor_offsets);
    Serial.println(sensor_offsets.accel_offset_x);
    Serial.println(sensor_offsets.accel_offset_y);
    Serial.println(sensor_offsets.accel_offset_z);
    Serial.println(sensor_offsets.mag_offset_x);
    Serial.println(sensor_offsets.mag_offset_y);
    Serial.println(sensor_offsets.mag_offset_z);
    Serial.println(sensor_offsets.gyro_offset_x);
    Serial.println(sensor_offsets.gyro_offset_y);
    Serial.println(sensor_offsets.gyro_offset_z);
    Serial.println(sensor_offsets.accel_radius);
    Serial.println(sensor_offsets.mag_radius);
*/

}
