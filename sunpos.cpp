#include "sunpos.h"
#include <stdlib.h>
#include <math.h>


//time

datetime_t datetime_t::update_format( datetime_t& falseform){
    datetime_t formattedt;
    ldiv_t hourres;
    ldiv_t minres;
    ldiv_t secres;
    int neg_sec=0;
    int neg_min=0;
    int neg_hour=0;

    secres=ldiv(falseform.sec,60L);                            // calculate seconds
    formattedt.sec=secres.rem;
    if (formattedt.sec < 0) {
        neg_sec=1;
        formattedt.sec=60+formattedt.sec;
    }

    formattedt.min=falseform.min+secres.quot-neg_sec;         // calculate minutes
    minres=ldiv(formattedt.min,60);
    formattedt.min=minres.rem;
    if (formattedt.min < 0) {
        neg_min=1;
        formattedt.min=60+formattedt.min;
    }

    formattedt.hour=falseform.hour+minres.quot-neg_min;       // calculate hours
    hourres=ldiv(formattedt.hour,24);
    formattedt.hour=hourres.rem;
    if (formattedt.hour < 0) {
        neg_hour=1;
        formattedt.hour=24+formattedt.hour;
    }
    //    formattedt.day=oclock.day+hourres.quot-neg_hour;

    return formattedt;
}

//Sunpos
//Definition der Konstruktoren
Sunpos::Sunpos(float lat, float lon) {
    set_latitude(lat);
    set_longitude(lon);
    //set_geoh(-50/60/57.29578);
    init();
}
Sunpos::Sunpos() {
    set_latitude(49.96);
    set_longitude(5.93);
    //set_geoh(-50/60/57.29578);
    init();
}

//Definition von init()
void Sunpos::init() {
    datetime_t inittime;                      // ggf Initialisierung über RTC hinzufügen
    inittime.hour=0L;
    inittime.min=0L;
    inittime.sec=0L;
    datetime_t haref;
    haref.hour=12L;
    haref.min=0L;
    haref.sec=0L;
    datetime_t onetime;
    onetime.hour=1L;
    onetime.min=0L;
    onetime.sec=0L;

    set_woz(inittime);
    set_mez(inittime);
    set_hourangref(haref);
    set_timediff(inittime);
    set_twelveoclock(haref);
    set_oneoclock(onetime);
    set_geoh(-0.01454);

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

void Sunpos::set_woz( datetime_t& woznow) {
    woz=woznow;
}
datetime_t Sunpos::get_woz() {
    return woz.update_format(woz);
}
void Sunpos::set_mez( datetime_t& meznow) {
    mez=meznow;
}
datetime_t Sunpos::get_mez() {
    return mez.update_format(mez);
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
    declang=get_eclipticang()*sin(((2*PI)/365)*(get_doy()-81));
    return declang;
}
void Sunpos::set_hourangref( datetime_t& harefnew) {
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
datetime_t Sunpos::get_hourangtime() {
    float anghours;                                 // auxiliary variable
    anghours=get_hsr()/15;                          // hours as float
    hourangtime.hour=(long) floor(anghours);
    hourangtime.min= (long) floor((anghours-hourangtime.hour)*60);
    hourangtime.sec= (long) floor((anghours-hourangtime.min)*60);
    return hourangtime.update_format(hourangtime);
}
datetime_t Sunpos::get_sunrisewoz(){
    get_hourangtime();                              // update hourangtime
    sunrisewoz.hour=hourangref.hour-hourangtime.hour;
    sunrisewoz.min=hourangref.min-hourangtime.min;
    sunrisewoz.min=hourangref.sec-hourangtime.sec;
    return sunrisewoz.update_format(sunrisewoz);
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

datetime_t Sunpos::get_moz() {
    get_woz();
    div_t divres;
    divres=div((long)floor(get_e()),60);
    long ehours= divres.quot;
    long emins= (long) floor(get_e()-ehours);
    long esecs= (long) floor((get_e()-emins)*60);
    moz.hour=woz.hour-ehours;
    moz.min=woz.min-emins;
    moz.sec=woz.sec-esecs;
    return moz.update_format(moz);
}
datetime_t Sunpos::get_ct() {
    get_moz();
    long ctsecs=(long) ((moz.min-4*(get_longitude()))*60);
    ct.hour=moz.hour+1;
    div_t minres;
    div_t secres;
    secres=div(ctsecs,60);                            // calculate minutes
    ct.sec=moz.sec+secres.rem;
    ct.min=moz.min+secres.quot;
    minres=div(ct.min,60);                            // calculate hours
    ct.min=minres.rem;
    ct.hour=ct.hour+minres.quot;
    return ct.update_format(ct);
}
datetime_t Sunpos::get_st() {
    st=get_ct();
    st.hour=st.hour+1;
    return st.update_format(st);
}

void Sunpos::set_geoh(float newgeoh){
    geoh=newgeoh;
}

float Sunpos::get_geoh(){
    return geoh;
}

void Sunpos::set_timediff(datetime_t& diffnew){
    timediff=diffnew;
}

datetime_t Sunpos::get_timediff(){
    double diffhours=12*acos((sin(geoh)-sin(rad*get_latitude())*sin(rad*get_declang()))/ \
                             (cos(rad*get_latitude())*cos(rad*get_declang())))/PI;
    timediff.min= long((diffhours*60L));
    return timediff.update_format(timediff);
}

void Sunpos::set_twelveoclock( datetime_t& twelvenew) {
    twelveoclock=twelvenew;
}

void Sunpos::set_oneoclock( datetime_t& onenew) {
    oneoclock=onenew;
}

datetime_t Sunpos::get_twelveoclock(void) {
    return twelveoclock;
}

datetime_t Sunpos::get_oneoclock(void) {
    return oneoclock;
}

datetime_t Sunpos::get_srwoz(){
    datetime_t twelve=get_twelveoclock();
    datetime_t timed=get_timediff();
    srwoz.hour=twelve.hour-timed.hour;
    srwoz.min=twelve.min-timed.min;
    srwoz.sec=twelve.sec-timed.sec;
    return srwoz.update_format(srwoz);
}

datetime_t Sunpos::get_srmoz(){
    datetime_t auxmoz=get_srwoz();
    srmoz.hour=auxmoz.hour;
    srmoz.min=auxmoz.min-get_e();
    srmoz.sec=auxmoz.sec;
    return srmoz.update_format(srmoz);
}

datetime_t Sunpos::get_srmez(){
    get_srmoz();
    long ctsecs=long(((srmoz.min-4*(get_longitude()))*60));
    srmez.hour=srmoz.hour+1;
    srmez.sec=srmoz.sec+ctsecs;
    return srmez.update_format(srmez);
}

datetime_t Sunpos::get_sswoz(){
    datetime_t twelve=get_twelveoclock();
    datetime_t timed=get_timediff();
    srwoz.hour=twelve.hour+timed.hour;
    srwoz.min=twelve.min+timed.min;
    srwoz.sec=twelve.sec+timed.sec;
    return srwoz.update_format(srwoz);
}

datetime_t Sunpos::get_ssmoz(){
    datetime_t auxmoz=get_sswoz();
    ssmoz.hour=auxmoz.hour;
    ssmoz.min=auxmoz.min-get_e();
    ssmoz.sec=auxmoz.sec;
    return ssmoz.update_format(ssmoz);
}

datetime_t Sunpos::get_ssmez(){
    get_ssmoz();
    long ctsecs=long(((ssmoz.min-4*(get_longitude()))*60));
    ssmez.hour=ssmoz.hour+1;
    ssmez.sec=ssmoz.sec+ctsecs;
    return ssmez.update_format(ssmez);
}
