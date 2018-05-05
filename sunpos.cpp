#include "sunpos.h"
#include <stdlib.h>
#include <math.h>


//time

datetime_t datetime_t::operator+(const datetime_t& time2) {
    datetime_t result;
    result.sec=sec+time2.sec;
    result.min=min+time2.min;
    result.hour=hour+time2.hour;
    result.doy=doy;
    return update_format(result);
}

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
    latitude_=lat;
}
float Sunpos::get_latitude() {
    return latitude_;
}
void Sunpos::set_longitude(float lon) {
    longitude_=lon;
}
float Sunpos::get_longitude() {
    return longitude_;
}

void Sunpos::set_woz( time& woznow) {
    woz_=woznow;
}
time Sunpos::get_woz() {
    return woz_.update_format(woz_);
}
void Sunpos::set_mez( time& meznow) {
    mez_=meznow;
}
time Sunpos::get_mez() {
    return mez_.update_format(mez_);
}
void Sunpos::set_doy(int doynow) {
    doy_=doynow;
}
int Sunpos::get_doy() {
    return doy_;
}

float Sunpos::get_eclipticang() {
    return eclipticang_;
}
float Sunpos::get_declang() {
    declang_=get_eclipticang()*sin(((2*PI)/365)*(get_doy()-81));
    return declang_;
}
void Sunpos::set_hourangref( time& harefnew) {
    hourangref_=harefnew;
}
float Sunpos::get_hourang() {
    get_woz();                                         // update woz
    hourang_=((hourangref_.hour-woz_.hour)+(1/60)*(hourangref_.min-woz_.min))*15;
    return hourang_;
}
float Sunpos::get_elevang() {
    elevang_=deg_*asin(sin(rad_*latitude_)*sin(rad_*get_declang())\
                     +cos(rad_*latitude_)*cos(rad_*get_declang())*cos(rad_*get_hourang()));
    return elevang_;
}
float Sunpos::get_azang() {
    azang_=deg_*asin(cos(rad_*get_declang())*sin(rad_*get_hourang()))\
            /cos(rad_*get_elevang());
    return azang_;
}
float Sunpos::get_pvtiltang() {
    pvtiltang_=90-get_elevang();
    return pvtiltang_;
}
float Sunpos::get_hsr() {
    hsr_=deg_*(acos(-tan(rad_*latitude_)*tan(rad_*get_declang())));
    return hsr_;
}
time Sunpos::get_hourangtime() {
    float anghours;                                 // auxiliary variable
    anghours=get_hsr()/15;                          // hours as float
    hourangtime_.hour=(long) floor(anghours);
    hourangtime_.min= (long) floor((anghours-hourangtime_.hour)*60);
    hourangtime_.sec= (long) floor((anghours-hourangtime_.min)*60);
    return hourangtime_.update_format(hourangtime_);
}
time Sunpos::get_sunrisewoz(){
    get_hourangtime();                              // update hourangtime
    sunrisewoz_.hour=hourangref_.hour-hourangtime_.hour;
    sunrisewoz_.min=hourangref_.min-hourangtime_.min;
    sunrisewoz_.min=hourangref_.sec-hourangtime_.sec;
    return sunrisewoz_.update_format(sunrisewoz_);
}
float Sunpos::get_b(){
    b_=(360/364)*(get_doy()-81);
    return b_;
}
float Sunpos::get_e() {
    float d=get_b();                                      //auxiliary variable
    e_=9.87*sin(rad_*2*d)-7.53*cos(rad_*d)-1.5*sin(rad_*d);
    return e_;
}

time Sunpos::get_moz() {
    get_woz();
    div_t divres;
    divres=div((long)floor(get_e()),60);
    long ehours= divres.quot;
    long emins= (long) floor(get_e()-ehours);
    long esecs= (long) floor((get_e()-emins)*60);
    moz_.hour=woz_.hour-ehours;
    moz_.min=woz_.min-emins;
    moz_.sec=woz_.sec-esecs;
    return moz_.update_format(moz_);
}
time Sunpos::get_ct() {
    get_moz();
    long ctsecs=(long) ((moz_.min-4*(get_longitude()))*60);
    ct_.hour=moz_.hour+1;
    div_t minres;
    div_t secres;
    secres=div(ctsecs,60);                            // calculate minutes
    ct_.sec=moz_.sec+secres.rem;
    ct_.min=moz_.min+secres.quot;
    minres=div(ct_.min,60);                            // calculate hours
    ct_.min=minres.rem;
    ct_.hour=ct_.hour+minres.quot;
    return ct_.update_format(ct_);
}
time Sunpos::get_st() {
    st_=get_ct();
    st_.hour=st_.hour+1;
    return st_.update_format(st_);
}

void Sunpos::set_geoh(float newgeoh){
    geoh_=newgeoh;
}

float Sunpos::get_geoh(){
    return geoh_;
}

void Sunpos::set_timediff(time& diffnew){
    timediff_=diffnew;
}

time Sunpos::get_timediff(){
    double diffhours=12*acos((sin(geoh_)-sin(rad_*get_latitude())*sin(rad_*get_declang()))/ \
                             (cos(rad_*get_latitude())*cos(rad_*get_declang())))/PI;
    timediff_.min= long((diffhours*60L));
    return timediff_.update_format(timediff_);
}

void Sunpos::set_twelveoclock( time& twelvenew) {
    twelveoclock_=twelvenew;
}

void Sunpos::set_oneoclock( time& onenew) {
    oneoclock_=onenew;
}

time Sunpos::get_twelveoclock(void) {
    return twelveoclock_;
}

time Sunpos::get_oneoclock(void) {
    return oneoclock_;
}

time Sunpos::get_srwoz(){
    time twelve=get_twelveoclock();
    time timed=get_timediff();
    srwoz_.hour=twelve.hour-timed.hour;
    srwoz_.min=twelve.min-timed.min;
    srwoz_.sec=twelve.sec-timed.sec;
    return srwoz_.update_format(srwoz_);
}

time Sunpos::get_srmoz(){
    time auxmoz=get_srwoz();
    srmoz_.hour=auxmoz.hour;
    srmoz_.min=auxmoz.min-get_e();
    srmoz_.sec=auxmoz.sec;
    return srmoz_.update_format(srmoz_);
}

time Sunpos::get_srmez(){
    get_srmoz();
    long ctsecs=long(((srmoz_.min-4*(get_longitude()))*60));
    srmez_.hour=srmoz_.hour+1;
    srmez_.sec=srmoz_.sec+ctsecs;
    return srmez_.update_format(srmez_);
}

time Sunpos::get_sswoz(){
    time twelve=get_twelveoclock();
    time timed=get_timediff();
    srwoz_.hour=twelve.hour+timed.hour;
    srwoz_.min=twelve.min+timed.min;
    srwoz_.sec=twelve.sec+timed.sec;
    return srwoz_.update_format(srwoz_);
}

time Sunpos::get_ssmoz(){
    time auxmoz=get_sswoz();
    ssmoz_.hour=auxmoz.hour;
    ssmoz_.min=auxmoz.min-get_e();
    ssmoz_.sec=auxmoz.sec;
    return ssmoz_.update_format(ssmoz_);
}

time Sunpos::get_ssmez(){
    get_ssmoz();
    long ctsecs=long(((ssmoz_.min-4*(get_longitude()))*60));
    ssmez_.hour=ssmoz_.hour+1;
    ssmez_.sec=ssmoz_.sec+ctsecs;
    return ssmez_.update_format(ssmez_);
}
