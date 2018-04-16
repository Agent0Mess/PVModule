#ifndef SUNPOS_H
#define SUNPOS_H

#define PI 3.14159265

struct time {
    int sec;
    int min;
    int hour;
};


class Sunpos {
private:
    const float deg=180/PI;
    const float rad=PI/180;

    float latitude;
    float longitude;

    time woz;                           // Wahre Ortszeit, Sonnenzeit
    time mez;                           // Lokale Ortszeit
    int doy;                            // Day of Year; 1. Januar = 1

    const float eclipticang=23.45;      // Ekliptikwinkel der Erde
    float declang;                      // Earth Declination Winkel in Gard
    time  hourangref;                   // Referenzzeit für hourang
    float hourang;                      // HA - Hour Angle
    float elevang;                      // Elevation Angle Beta @ Solar Time
    float azang;                        // Solar Angle Azimut Angle @ Solar time
    float pvtiltang;                    // PV Panel tilt Angle, lokal, horizontal gegenüber Boden

public:
  //Deklaration der Konstruktoren
    Sunpos(float, float);               // (latitude, longitude)
    Sunpos();                           // default ist Wiltz
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


};

#endif // SUNPOS_H
