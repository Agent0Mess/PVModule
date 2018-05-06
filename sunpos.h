#ifndef SUNPOS_H
#define SUNPOS_H

#include "rtcadapter.h"

#define PI 3.14159265


class Sunpos {
private:
    const float deg_=180/PI;
    const float rad_=PI/180;

    float latitude_;
    float longitude_;

    datetime_t woz_;                           // True local time, solar time
    datetime_t mez_;                           /**< local time - time displayed on your clock */
    datetime_t hourangtime_;                   // Hour Angle Time


    const float eclipticang_=23.45;      // Ecliptic Angle of the Earth
    float declang_;                      // Earth Declination Angle in degrees
    datetime_t  hourangref_;                   // Referenzzeit für hourang
    float hourang_;                      // HA - Hour Angle
    float elevang_;                      // Elevation Angle Beta @ Solar Time
    float azang_;                        /**< Solar Angle Azimut, Angle @ Solar time */
    float pvtiltang_;                    /**< PV Panel tilt Angle, local, horizontal over floor*/
    float hsr_;                          // Azimuth @ Sunrise in degrees
    datetime_t sunrisewoz_;                    // time of sunrise in solar time
    float b_;                            // used for calculation solar day
    float e_;                            // equation of time (woz-moz) in minutes
    datetime_t moz_;                           // mean local time -> woz-e

    datetime_t ct_;                            // local clocktime, mez @ local longitude
    datetime_t st_;                            // local summer time

    float geoh_;                         // geometric horizon height
    datetime_t timediff_;

    datetime_t srwoz_;                         // sunrise in WOZ
    datetime_t srmoz_;                         // sunrise in MOZ
    datetime_t srmez_;                         // sunrise in MEZ
    datetime_t sswoz_;                         // sunset in WOZ
    datetime_t ssmoz_;                         // sunset in MOZ
    datetime_t ssmez_;                         // sunset in MEZ
    datetime_t twelveoclock_;                  //
    datetime_t oneoclock_;                     //

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
