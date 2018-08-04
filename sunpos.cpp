#include "sunpos.h"
#include <stdlib.h>
#include <math.h>

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

    update_woz();
    set_mez(inittime);
    set_hourangref(haref);
    set_timediff(inittime);
    set_geoh(-0.01454);                     /**< It might be necessary to adapt this constant */

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

void Sunpos::update_woz() {
    datetime_t woz_temp;
    datetime_t oneoclock;
    oneoclock.hour=1L;
    oneoclock.min=0L;
    oneoclock.sec=0L;
    woz_temp=mez_;
    woz_temp.min=woz_temp.min+get_e()+4*longitude_;
    woz_temp=woz_temp.update_format(woz_temp);
    woz_temp=woz_temp-oneoclock;
    woz_=woz_temp.update_format(woz_temp);
}

datetime_t Sunpos::get_woz() {
    update_woz();
    return woz_;
}
void Sunpos::set_mez( datetime_t& meznow) {
    mez_=meznow;
}
datetime_t Sunpos::get_mez() {
    return mez_.update_format(mez_);
}
void Sunpos::set_doy(int doynow) {
    mez_.doy=doynow;
}
int Sunpos::get_doy() {
    return mez_.doy;
}

float Sunpos::get_declang() {
    declang_=23.45*sin(((2*PI)/365.0)*(get_doy()-81.0));
    return declang_;
}
void Sunpos::set_hourangref( datetime_t& harefnew) {
    hourangref_=harefnew;
}
float Sunpos::get_hourang() {
    get_woz();                                         // update woz
    hourang_=(((-hourangref_.hour)+get_woz().hour)+(1.0/60.0)*((-hourangref_.min)+get_woz().min)+(1.0/360.0)*((-hourangref_.sec)+get_woz().sec))*15;
    return hourang_;
}
float Sunpos::get_elevang() {
    elevang_=deg_*asin(sin(rad_*latitude_)*sin(rad_*get_declang())\
                     +cos(rad_*latitude_)*cos(rad_*get_declang())*cos(rad_*get_hourang()));
    return elevang_;
}
float Sunpos::get_azang() {
    float tan_alpha;
    float sin_hourang;
    float beta;
    float hourang=get_hourang();
    float azang_old;
    float declang= get_declang();

    sin_hourang=sin(rad_*hourang);
    beta=cos(rad_*hourang)*sin(rad_*latitude_)-tan(rad_*declang)*cos(rad_*latitude_);
    tan_alpha=sin_hourang/beta;

    azang_old=deg_*asin(cos(rad_*get_declang())*sin(rad_*get_hourang())/cos(rad_*get_elevang()));

    if (tan_alpha>0 && hourang<0) {
        azang_=azang_old;
    }
    else if (tan_alpha<0 && hourang>0) {
        azang_=360-azang_old;
    }
    else {
        azang_=180+azang_old;
    }
    return azang_;
}
float Sunpos::get_pvtiltang() {
    pvtiltang_=90.0-get_elevang();
    return pvtiltang_;
}
float Sunpos::get_hsr() {
    hsr_=deg_*(acos(-tan(rad_*latitude_)*tan(rad_*get_declang())));
    return hsr_;
}
datetime_t Sunpos::get_hourangtime() {
    float anghours;                                 // auxiliary variable
    anghours=get_hsr()/15;                          // hours as float
    hourangtime_.hour=(long) floor(anghours);
    hourangtime_.min= (long) floor((anghours-hourangtime_.hour)*60);
    hourangtime_.sec= (long) floor((anghours-hourangtime_.min)*60);
    return hourangtime_.update_format(hourangtime_);
}
datetime_t Sunpos::get_sunrisewoz(){
    get_hourangtime();                              // update hourangtime
    sunrisewoz_=hourangref_-hourangtime_;
    return sunrisewoz_;
}
float Sunpos::get_b(){
    b_=(360.0/364.0)*(get_doy()-81);
    return b_;
}
float Sunpos::get_e() {
    float d=get_b();                                      //auxiliary variable
    e_=9.87*sin(rad_*2*d)-7.53*cos(rad_*d)-1.5*sin(rad_*d);
    return e_;
}

datetime_t Sunpos::get_moz() {
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
datetime_t Sunpos::get_ct() {
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
datetime_t Sunpos::get_st() {
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

void Sunpos::set_timediff(datetime_t& diffnew){
    timediff_=diffnew;
}
datetime_t Sunpos::get_timediff(){
    double diffhours=12*acos((sin(geoh_)-sin(rad_*get_latitude())*sin(rad_*get_declang()))/ \
                             (cos(rad_*get_latitude())*cos(rad_*get_declang())))/PI;
    timediff_.min= long((diffhours*60L));
    return timediff_.update_format(timediff_);
}


datetime_t Sunpos::get_srwoz(){
    datetime_t twelve;
    twelve.hour=12L;
    twelve.min=0L;
    twelve.sec=0L;
    datetime_t timed=get_timediff();
    srwoz_=twelve-timed;
    return srwoz_.update_format(srwoz_);
}
datetime_t Sunpos::get_srmoz(){
    datetime_t auxmoz=get_srwoz();
    srmoz_.hour=auxmoz.hour;
    srmoz_.min=auxmoz.min-get_e();
    srmoz_.sec=auxmoz.sec;
    return srmoz_.update_format(srmoz_);
}
datetime_t Sunpos::get_srmez(){
    get_srmoz();
    long ctsecs=long(((srmoz_.min-4*(get_longitude()))*60));
    srmez_.hour=srmoz_.hour+1;
    srmez_.sec=srmoz_.sec+ctsecs;
    return srmez_.update_format(srmez_);
}
datetime_t Sunpos::get_sswoz(){
    datetime_t twelve;
    twelve.hour=12L;
    twelve.min=0L;
    twelve.sec=0L;
    datetime_t timed=get_timediff();
    srwoz_=twelve+timed;
    return srwoz_.update_format(srwoz_);
}
datetime_t Sunpos::get_ssmoz(){
    datetime_t auxmoz=get_sswoz();
    ssmoz_.hour=auxmoz.hour;
    ssmoz_.min=auxmoz.min-get_e();
    ssmoz_.sec=auxmoz.sec;
    return ssmoz_.update_format(ssmoz_);
}
datetime_t Sunpos::get_ssmez(){
    get_ssmoz();
    long ctsecs=long(((ssmoz_.min-4*(get_longitude()))*60));
    ssmez_.hour=ssmoz_.hour+1;
    ssmez_.sec=ssmoz_.sec+ctsecs;
    return ssmez_.update_format(ssmez_);
}


