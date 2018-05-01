#ifndef SUNPOS_H
#define SUNPOS_H

#define PI 3.14159265

struct datetime_t {
    long int sec;
    long int min;
    long int hour;

    datetime_t update_format( datetime_t& falseform);
};


class Sunpos {
private:
    const float deg=180/PI;
    const float rad=PI/180;

    float latitude;
    float longitude;

    datetime_t woz;                           // True local time, solar time
    datetime_t mez;                           // local time
    datetime_t hourangtime;                   // Hour Angle Time
    int doy;                            // Day of Year; first January = 1

    const float eclipticang=23.45;      // Ecliptic Angle of the Earth
    float declang;                      // Earth Declination Angle in degrees
    datetime_t  hourangref;                   // Referenzzeit für hourang
    float hourang;                      // HA - Hour Angle
    float elevang;                      // Elevation Angle Beta @ Solar Time
    float azang;                        // Solar Angle Azimut, Angle @ Solar time
    float pvtiltang;                    // PV Panel tilt Angle, lokal, horizontal gegenüber Boden
    float hsr;                          // Azimuth @ Sunrise in degrees
    datetime_t sunrisewoz;                    // time of sunrise in solar time
    float b;                            // used for calculation solar day
    float e;                            // equation of time (woz-moz) in minutes
    datetime_t moz;                           // mean local time -> woz-e

    datetime_t ct;                            // local clocktime, mez @ local longitude
    datetime_t st;                            // local summer time

    float geoh;                         // geometric horizon height
    datetime_t timediff;

    datetime_t srwoz;                         // sunrise in WOZ
    datetime_t srmoz;                         // sunrise in MOZ
    datetime_t srmez;                         // sunrise in MEZ
    datetime_t sswoz;                         // sunset in WOZ
    datetime_t ssmoz;                         // sunset in MOZ
    datetime_t ssmez;                         // sunset in MEZ
    datetime_t twelveoclock;                  //
    datetime_t oneoclock;                     //

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

    void set_woz( datetime_t& woznow);
    datetime_t get_woz();
    void set_mez( datetime_t& meznow);
    datetime_t get_mez();
    void set_doy(int);
    int get_doy();

    float get_eclipticang();
    float get_declang();
    void set_hourangref ( datetime_t& harefnew);
    float get_hourang();
    float get_elevang();
    float get_azang();
    float get_pvtiltang();
    float get_hsr();
    datetime_t get_hourangtime();
    datetime_t get_sunrisewoz();
    float get_b();
    float get_e();
    datetime_t get_moz();
    datetime_t get_ct();
    datetime_t get_st();
    void set_geoh(float);
    float get_geoh();
    void set_timediff( datetime_t& diffnew );
    datetime_t get_timediff();

    void set_twelveoclock( datetime_t& twelvenew);
    void set_oneoclock ( datetime_t& onenew);
    datetime_t get_twelveoclock();
    datetime_t get_oneoclock ();

    datetime_t get_srwoz();
    datetime_t get_srmoz();
    datetime_t get_srmez();
    datetime_t get_sswoz();
    datetime_t get_ssmoz();
    datetime_t get_ssmez();
};

#endif // SUNPOS_H
