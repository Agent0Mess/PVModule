#include "sunpos.h"
#include <math.h>

#define PI 3.14159265

//Definition der Konstruktoren
Sunpos::Sunpos(float lat, float lon) {
    set_latitude(lat);
    set_longitude(lon);
    set_woz_sec(0);
    set_woz_min(0);
    set_woz_hour(0);
}
Sunpos::Sunpos() {
    set_latitude(49.96);
    set_longitude(5.93);
    set_woz_sec(0);
    set_woz_min(0);
    set_woz_hour(0);
}

//Definition Elementfunktionen

void Sunpos::set_latitude(float lat) {
    latitude=lat;
}
float Sunpos::get_latitude() {
    return latitude;
}
void Sunpos::set_longitude(float lon) {
    longitude=lon;
}
float Sunpos::get_longitude() {
    return longitude;
}

void Sunpos::set_woz_sec(int secnow) {
    woz.sec=secnow;
}
int Sunpos::get_woz_sec() {
    return woz.sec;
}
void Sunpos::set_woz_min(int minnow) {
    woz.min=minnow;
}
int Sunpos::get_woz_min() {
    return woz.min;
}
void Sunpos::set_woz_hour(int hournow) {
    woz.hour=hournow;
}
void Sunpos::set_mez_sec(int secnow) {
    woz.sec=secnow;
}
int Sunpos::get_mez_sec() {
    return woz.sec;
}
void Sunpos::set_mez_min(int minnow) {
    woz.min=minnow;
}
int Sunpos::get_mez_min() {
    return woz.min;
}
void Sunpos::set_mez_hour(int hournow) {
    woz.hour=hournow;
}
void Sunpos::set_doy(int doynow) {
    doy=doynow;
}
int Sunpos::get_doy() {
    return doy;
}

float Sunpos::get_declang() {
    declang=eclipticang*sin(365/(2*PI))*get_doy();
    return declang;
}
