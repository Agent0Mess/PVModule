#include "rtcadapter.h"

//time

datetime_t datetime_t::operator + (const datetime_t& time2) {
    datetime_t result;
    result.sec=this->sec + time2.sec;
    result.min=this->min + time2.min;
    result.hour=this->hour + time2.hour;
    result.doy=this->doy;
    return update_format(result);
}

datetime_t datetime_t::operator-(const datetime_t& time2) {
    datetime_t result;
    result.sec=this->sec-time2.sec;
    result.min=this->min-time2.min;
    result.hour=this->hour-time2.hour;
    result.doy=this->doy;
    return update_format(result);
}

datetime_t datetime_t::update_format( datetime_t& falseform){
    datetime_t formattedt;
    ldiv_t doyres;
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

    formattedt.doy=falseform.doy+hourres.quot-neg_hour;        /**< calculate day of year */
    doyres=ldiv(formattedt.doy,365);
    formattedt.doy=doyres.rem;
    if (formattedt.doy < 0) {
        formattedt.doy=356+formattedt.doy;
    }

    return formattedt;
}


RtcAdapter::RtcAdapter()
{
}

datetime_t RtcAdapter::read_time() {
    datetime_t return_time;
    int current_month;
    int current_day_in_month;

    time_now = this->now();
    return_time.sec= time_now.second();
    return_time.min= time_now.minute();
    return_time.hour=time_now.hour();

    current_day_in_month=time_now.day();
    current_month=time_now.month();

    return_time.doy=calc_doy(current_day_in_month, current_month);

    return return_time;
}


int RtcAdapter::days_in_month(int month) {
    int num_days;
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        num_days=31;
        break;
    case 4: case 6: case 9: case 11:
        num_days=30;
        break;
    case 2:
        num_days=28;
        break;
    default:
        num_days=31;
        break;
    };

    return num_days;
}

int RtcAdapter::calc_doy(int current_day_in_month, int current_month) {
    int i=1;
    int doy_month=0;
    for (i=1;i<current_month;i++) {
        doy_month=doy_month+days_in_month(i);
    }
    return doy_month+current_day_in_month;
}
