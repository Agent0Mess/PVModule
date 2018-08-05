#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "orientation_measure.h"
#include "rtcadapter.h"
#include "motordriver.h"
#include "orientation_controller.h"
#include "sunpos.h"
//#include <MemoryFree.h>
#include <EEPROM.h>


#define LEDPIN 13

OrientationMeasurement sensor_mes = OrientationMeasurement();
OrientationController PanelControl;

int addr = 0;

void setup(void)
{
    Serial.begin(115200);
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN,HIGH);

    PanelControl.begin();

    Serial.println("Orient Sensor Test"); Serial.println("");
    PanelControl.stopPanel();
    digitalWrite(LEDPIN,LOW);
}

void loop(void)
{
        Serial.print(" Is day? ");
        Serial.println(PanelControl.isDaytime());
        Serial.print("\t  doy: ");
        Serial.print(PanelControl.getMezNow().doy);
        Serial.print("\t Hour: ");
        Serial.print(PanelControl.getMezNow().hour);
        Serial.print("\t Min: ");
        Serial.print(PanelControl.getMezNow().min);
        Serial.println("");


        Serial.print("\t Dev Tilt: ");
        Serial.print(PanelControl.deviationTilt(), 4);
        Serial.print("\t Dev Azi: ");
        Serial.print(PanelControl.deviationAzimuth(), 4);
        Serial.println("");

        Serial.print("\t Dev Tilt MP: ");
        Serial.print(PanelControl.deviationTiltMorningPos(), 4);
        Serial.print("\t Dev Azimuth MP: ");
        Serial.print(PanelControl.deviationAzimuthMorningPos(), 4);
        Serial.println("");

        Serial.print("\t Des Tilt: ");
        Serial.print(PanelControl.desiredValueTilt(), 4);
        Serial.print("\t Des Azi: ");
        Serial.print(PanelControl.desiredValueAzimuth(), 4);
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

    //        Serial.print("freeMemory()=");
    //        Serial.println(freeMemory());

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

    PanelControl.orientPanel();

    if(millis()+1000<millis())
    {
        EEPROM.write(addr, 99);
        addr+=1;
    }

    if(PanelControl.getEmergencyStopped())
    {
        EEPROM.write(addr, 88);
        addr+=1;
    }

    if (addr == EEPROM.length())
    {
      addr = 0;
    }
}
