/**
  * @file Provides the functionality to convert the DateTime into datetime_t
  *
  * DateTime is the time and date format used in the RTClib.h
  * The calculations in sunpos use the datetime_t format, so we
  * convert the DateTime into datetime_t.
  *
  * Moreover this has the advantage of being more flexible if one wants
  * to use another RTC Module and/or another RTC library. In that case,
  * one only has to adapt the RTCAdapter class.
  */

#ifndef RTCADAPTER_H
#define RTCADAPTER_H

#include <RTClib.h>


struct datetime_t {
    long int sec;
    long int min;
    long int hour;
    int      doy;                             /**< Day of Year; first January = 1*/

    datetime_t update_format( datetime_t& falseform);

    datetime_t operator + (const datetime_t& time2);
    datetime_t operator - (const datetime_t& time2);

    bool operator < (const datetime_t& time2);
    bool operator > (const datetime_t& time2);
};


class RtcAdapter : public RTC_DS3231
{
private:
    DateTime time_now;

public:
    /**
     * @brief RtcAdapter - Constructor for the class
     */
    RtcAdapter();

    /**
     * @brief read_time - reads the current time and date from RTC
     * @return current time in datetime_t format
     */
    datetime_t read_time();

    /**
     * @brief days_in_month - Simply checks how many days are in a specific month
     * @param month - number of month (e.g. 3 for March)
     * @return number of days in month (e.g. 31 for March)
     */
    int days_in_month(int month);

    /**
     * @brief calc_doy - Calculates the day of year (e.g. 2 for 2th January)
     * @param current_day_in_month The day of the date (i.e. dd of dd/mm/yyyy)
     * @param current_month The month of the date (i.e. mm of dd/mm/yyyy)
     * @return  The number of the day of the year (e.g. 365 for 31th December)
     */
    int calc_doy(int current_day_in_month, int current_month);

};

#endif // RTCADAPTER_H
