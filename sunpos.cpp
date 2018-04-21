#include "sunpos.h"
#include <stdlib.h>
#include <math.h>


//time
time time::update_format(){
    time formattedt;
    div_t hourres;
    div_t minres;
    div_t secres;

    secres=div(sec,60);                            // calculate minutes
    formattedt.sec=sec+secres.rem;
    formattedt.min=min+secres.quot;

    minres=div(formattedt.min,60);                        // calculate hours
    formattedt.min=minres.rem;
    formattedt.hour=hour+minres.quot;

    hourres=div(formattedt.hour,24);                      // calculate days
    formattedt.hour=minres.rem;
//    formattedt.day=oclock.day+hourres.quot;
    return formattedt;
}


//Sunpos
//Definition der Konstruktoren
Sunpos::Sunpos(float lat, float lon) {
    set_latitude(lat);
    set_longitude(lon);
    set_geoh(-50/60/57.29578);
    init();
}
Sunpos::Sunpos() {
    set_latitude(49.96);
    set_longitude(5.93);
    set_geoh(-50/60/57.29578);
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
    set_timediff(inittime);

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
    return woz.update_format();
}
void Sunpos::set_mez( time& meznow) {
    mez=meznow;
}
time Sunpos::get_mez() {
    return mez.update_format();
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
    get_woz();                                         // update woz
    hourang=((hourangref.hour-woz.hour)+(1/60)*(hourangref.min-woz.min))*15;
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
float Sunpos::get_hsr() {
    hsr=deg*(acos(-tan(rad*latitude)*tan(rad*get_declang())));
    return hsr;
}
time Sunpos::get_hourangtime() {
    float anghours;                                 // auxiliary variable
    anghours=get_hsr()/15;                          // hours as float
    hourangtime.hour=(int) floor(anghours);
    hourangtime.min= (int) floor((anghours-hourangtime.hour)*60);
    hourangtime.sec= (int) floor((anghours-hourangtime.min)*60);
    return hourangtime.update_format();
}
time Sunpos::get_sunrisewoz(){
    get_hourangtime();                              // update hourangtime
    sunrisewoz.hour=hourangref.hour-hourangtime.hour;
    sunrisewoz.min=hourangref.min-hourangtime.min;
    sunrisewoz.min=hourangref.sec-hourangtime.sec;
    return sunrisewoz.update_format();
}
float Sunpos::get_b(){
    b=(360/364)*(get_doy()-81);
    return b;
}
float Sunpos::get_e() {
    float d=get_b();                                      //auxiliary variable
    e=9.87*sin(rad*2*d)-7.53*cos(rad*d)-1.5*sin(rad*d);
    return e;
}

time Sunpos::get_moz() {
    get_woz();
    div_t divres;
    divres=div((int)floor(get_e()),60);
    int ehours= divres.quot;
    int emins= (int) floor(get_e()-ehours);
    int esecs= (int) floor((get_e()-emins)*60);
    moz.hour=woz.hour-ehours;
    moz.min=woz.min-emins;
    moz.sec=woz.sec-esecs;
    return moz.update_format();
}
time Sunpos::get_ct() {
    get_moz();
    int ctsecs=(int) ((moz.min-4*(get_longitude()))*60);
    ct.hour=moz.hour+1;
    div_t minres;
    div_t secres;
    secres=div(ctsecs,60);                            // calculate minutes
    ct.sec=moz.sec+secres.rem;
    ct.min=moz.min+secres.quot;
    minres=div(ct.min,60);                            // calculate hours
    ct.min=minres.rem;
    ct.hour=ct.hour+minres.quot;
    return ct.update_format();
}
time Sunpos::get_st() {
    st=get_ct();
    st.hour=st.hour+1;
    return st.update_format();
}

void Sunpos::set_geoh(float newgeoh){
     geoh=newgeoh;
}

float Sunpos::get_geoh(){
    return geoh;
}

void Sunpos::set_timediff(time& diffnew){
    timediff=diffnew;
}

time Sunpos::get_timediff(){
    float diffhours=12*acos((sin(geoh)-sin(rad*get_latitude()))/ \
            (cos(rad*get_latitude()))*cos(rad*get_declang()))/PI;
    timediff.sec= (int) floor(diffhours*360);
    return timediff.update_format();
}



