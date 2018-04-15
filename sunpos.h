#ifndef SUNPOS_H
#define SUNPOS_H

struct time {
    int sec;
    int min;
    int hour;
};


class Sunpos {
private:
    float latitude;
    float longitude;

    time woz;                           // Wahre Ortszeit, Sonnenzeit
    time mez;                           // Lokale Ortszeit
    int doy;                            // Day of Year; 1. Januar = 0

    const float eclipticang=23.45;      // Ekliptikwinkel der Erde
    float declang;                      // Earth Declination Winkel in Gard
    float hourang;                      //

public:
  //Deklaration der Konstruktoren
    Sunpos(float, float);               // (latitude, longitude)
    Sunpos();                           // default ist Wiltz
  //Deklaration der Elementfunktionen
    void set_latitude(float);
    float get_latitude();
    void set_longitude(float);
    float get_longitude();

    void set_woz_sec(int);
    int get_woz_sec();
    void set_woz_min(int);
    int get_woz_min();
    void set_woz_hour(int);
    int get_woz_hour();
    void set_mez_sec(int);
    int get_mez_sec();
    void set_mez_min(int);
    int get_mez_min();
    void set_mez_hour(int);
    int get_mez_hour();
    void set_doy(int);
    int get_doy();

    float get_eclipticang();
    float get_declang();
};

#endif // SUNPOS_H
