#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "orientation_measure.h"
#include "rtcadapter.h"
#include "motordriver.h"
#include "orientation_controller.h"
#include "sunpos.h"

#define LEDPIN 13

OrientationMeasurement sensor_mes = OrientationMeasurement();
OrientationController PanelControl;

void setup(void)
{
    Serial.begin(115200);
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN,HIGH);

    PanelControl.begin();

    Serial.println("Orient Sensor Test"); Serial.println("");
    PanelControl.stop_panel();
    digitalWrite(LEDPIN,LOW);
}

void loop(void)
{
        Serial.print(" Is day? ");
        Serial.println(PanelControl.is_daytime());
        Serial.print("\t  doy: ");
        Serial.print(PanelControl.getMez_now().doy);
        Serial.print("\t Hour: ");
        Serial.print(PanelControl.getMez_now().hour);
        Serial.print("\t Min: ");
        Serial.print(PanelControl.getMez_now().min);
        Serial.println("");


        Serial.print("\t Dev Tilt: ");
        Serial.print(PanelControl.deviation_tilt(), 4);
        Serial.print("\t Dev Azi: ");
        Serial.print(PanelControl.deviation_azimuth(), 4);
        Serial.println("");

        Serial.print("\t Dev Tilt MP: ");
        Serial.print(PanelControl.deviation_tilt_morning_pos(), 4);
        Serial.print("\t Dev Azimuth MP: ");
        Serial.print(PanelControl.deviation_azimuth_morning_pos(), 4);
        Serial.println("");

        Serial.print("\t Des Tilt: ");
        Serial.print(PanelControl.desired_value_tilt(), 4);
        Serial.print("\t Des Azi: ");
        Serial.print(PanelControl.desired_value_azimuth(), 4);
        Serial.println("");

        Serial.print("\t C. Tilt: ");
        Serial.print(PanelControl.currentTilt(), 4);
        Serial.print("\t C. Azi: ");
        Serial.print(PanelControl.currentAzimuth(), 4);
        Serial.println("");

        Serial.print("Azi runs: ");
        Serial.println(PanelControl.getIsAziRunning());

        Serial.print("Tilt runs:  ");
        Serial.println(PanelControl.getIsTiltRunning());

        if(PanelControl.getEmergencyStopped())
        {
            Serial.println("---Stop---");
        }
        if(PanelControl.getPanelIsBlocked())
        {
            while (1)
            {
                Serial.println("---Blocked---");
                digitalWrite(LEDPIN,HIGH);
                delay(5000);
            }
        }

        PanelControl.orient_panel();

}
