#ifndef SUNPOS_H
#define SUNPOS_H

#define PI 3.14159265

struct time {
    long int sec;
    long int min;
    long int hour;

    time update_format( time& falseform);
};


class Sunpos {
private:
    const float deg=180/PI;
    const float rad=PI/180;

    float latitude;
    float longitude;

    time woz;                           // True local time, solar time
    time mez;                           // local time
    time hourangtime;                   // Hour Angle Time
    int doy;                            // Day of Year; first January = 1

    const float eclipticang=23.45;      // Ecliptic Angle of the Earth
    float declang;                      // Earth Declination Angle in degrees
    time  hourangref;                   // Referenzzeit für hourang
    float hourang;                      // HA - Hour Angle
    float elevang;                      // Elevation Angle Beta @ Solar Time
    float azang;                        // Solar Angle Azimut, Angle @ Solar time
    float pvtiltang;                    // PV Panel tilt Angle, lokal, horizontal gegenüber Boden
    float hsr;                          // Azimuth @ Sunrise in degrees
    time sunrisewoz;                    // time of sunrise in solar time
    float b;                            // used for calculation solar day
    float e;                            // equation of time (woz-moz) in minutes
    time moz;                           // mean local time -> woz-e

    time ct;                            // local clocktime, mez @ local longitude
    time st;                            // local summer time

    float geoh;                         // geometric horizon height
    time timediff;

    time srwoz;                         // sunrise in WOZ
    time srmoz;                         // sunrise in MOZ
    time srmez;                         // sunrise in MEZ
    time sswoz;                         // sunset in WOZ
    time ssmoz;                         // sunset in MOZ
    time ssmez;                         // sunset in MEZ
    time twelveoclock;                  //
    time oneoclock;                     //

public:
    //Deklaration der Konstruktoren
    Sunpos(float, float);               // (latitude, longitude)
    Sunpos();                           // default is Wiltz, Luxembourg
    //Init
    void init();
    //Deklaration der Elementfunktionen
    void set_latitude(float);
    float get_latitude();
    void set_longitude(float);
    float get_longitude();

    void set_woz( time& woznow);
    time get_woz();
    void set_mez( time& meznow);
    time get_mez();
    void set_doy(int);
    int get_doy();

    float get_eclipticang();
    float get_declang();
    void set_hourangref ( time& harefnew);
    float get_hourang();
    float get_elevang();
    float get_azang();
    float get_pvtiltang();
    float get_hsr();
    time get_hourangtime();
    time get_sunrisewoz();
    float get_b();
    float get_e();
    time get_moz();
    time get_ct();
    time get_st();
    void set_geoh(float);
    float get_geoh();
    void set_timediff( time& diffnew );
    time get_timediff();

    void set_twelveoclock( time& twelvenew);
    void set_oneoclock ( time& onenew);
    time get_twelveoclock();
    time get_oneoclock ();

    time get_srwoz();
    time get_srmoz();
    time get_srmez();
    time get_sswoz();
    time get_ssmoz();
    time get_ssmez();
};

#endif // SUNPOS_H
