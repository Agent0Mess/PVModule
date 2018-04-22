#include "sunpos.h"

time woznow;
Sunpos panel1;
float tiltangle;
float earthdecl;
float aziangle;
float elevang;
float hourang;
time srmez;
int srmezmin;
int srmezhour;
time timediff;
time ssmez;

void setup() {
  Serial.begin(9600);
  Serial.println("test");
  woznow.hour=12;
  woznow.min=0;
  woznow.sec=0;
  panel1.set_woz(woznow);
  panel1.set_doy(101);           // 21/06/2018
  tiltangle=panel1.get_pvtiltang();
  earthdecl=panel1.get_declang();
  aziangle=panel1.get_azang();
  elevang=panel1.get_elevang();
  hourang=panel1.get_hourang();
  srmez=panel1.get_srmez();
  ssmez=panel1.get_ssmez();
  srmezmin=srmez.min;
  srmezhour=srmez.hour;
  timediff=panel1.get_timediff();
}

void loop() {
  Serial.print("Tilt Angle is:  ");
  Serial.println(tiltangle);
  Serial.print("Earth declination angle is:  ");
  Serial.println(earthdecl);
  Serial.print("Solar Elevation angle is:  ");
  Serial.println(elevang);
  Serial.print("Azimuth angle is:  ");
  Serial.println(aziangle);
  Serial.print("Hour angle is:  ");
  Serial.println(hourang);
  Serial.print("WOZ is:  ");
  woznow=woznow.update_format(woznow);
  Serial.print(woznow.hour);
  Serial.print(":");
  Serial.println(woznow.min);  
  Serial.print("Timediff is:  ");
  Serial.print(timediff.hour);
  Serial.print(":");
  Serial.println(timediff.min);
  
  Serial.print("Sunrise MEZ is:  ");
  Serial.print(srmez.hour);
  Serial.print(":");
  Serial.println(srmez.min);    

  Serial.print("Sunset MEZ is:  ");
  Serial.print(ssmez.hour);
  Serial.print(":");
  Serial.println(ssmez.min);  

}
