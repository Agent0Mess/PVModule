#include "sunpos.h"
#include <math.h>

//Definition der Konstruktoren
Sunpos::Sunpos(float lat, float lon) {
    set_latitude(lat);
    set_longitude(lon);
    init();
}
Sunpos::Sunpos() {
    set_latitude(49.96);
    set_longitude(5.93);
    init();
}

//Definition von init()
void Sunpos::init() {
    time inittime;                      // ggf Initialisierung über RTC hinzufügen
    inittime.hour=0;
    inittime.min=0;
    inittime.sec=0;
    time haref;
    haref.hour=12;
    haref.min=0;
    haref.sec=0;

    set_woz(inittime);
    set_mez(inittime);
    set_hourangref(haref);

    set_doy(1);
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

void Sunpos::set_woz( time& woznow) {
    woz=woznow;
}
time Sunpos::get_woz() {
    return woz;
}
void Sunpos::set_mez( time& meznow) {
    mez=meznow;
}
time Sunpos::get_mez() {
    return mez;
}
void Sunpos::set_doy(int doynow) {
    doy=doynow;
}
int Sunpos::get_doy() {
    return doy;
}

float Sunpos::get_eclipticang() {
    return eclipticang;
}
float Sunpos::get_declang() {
    declang=get_eclipticang()*sin(365/(2*PI))*get_doy();
    return declang;
}
void Sunpos::set_hourangref( time& harefnew) {
    hourangref=harefnew;
}
float Sunpos::get_hourang() {
    hourang=((hourangref.hour-woz.hour)+(1/60)*(hourangref.min-woz.min))*15
    return hourang;
}
float Sunpos::get_elevang() {
    elevang=deg*asin(sin(rad*latitude)*sin(rad*get_declang())\
            +cos(rad*latitude)*cos(rad*get_declang())*cos(rad*get_hourang()));
    return elevang;
}
float Sunpos::get_azang() {
    azang=deg*asin(cos(rad*get_declang())*sin(rad*get_hourang()))\
          /cos(rad*get_elevang());
    return azang;
}
float Sunpos::get_pvtiltang() {
    pvtiltang=90-get_elevang();
    return pvtiltang;
}
