/* Derek Schacht
 *  2016 10 22
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and cite code that I find on the internet but I am not 
 *               perfect. If you find something that should be cited let me know
 *               and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right 
 *  to modify this in the future.
 */

#ifndef STATION_INTERFACE_H
#define STATION_INTERFACE_H

#include <string>

#include "system_types.h"


/*! \file station_interface.h
    \brief Provides the data types and methods that are used to build messages for reporting.
*/

/*! \struct ReportedWeatherDataType
    \brief Contains the data elements that are reported in the weather report.
 */
typedef struct
{
  u_int32 messageCount;
  double intervalTime;
  double humidity;
  double pressure;
  double temperature;
  double rainfall;
  double windDirection;
  double windSpeed;
  double windDirectionAtMaxWindSpeed;
  double maxWindSpeed;
  double windSpeedStdev;
  double irLight;
  double uvLight;
  double whiteLight;
  char dateString[32];
} ReportedWeatherDataType;

/*! \struct ReportedStatusDataType
    \brief Contains the data elements that are reported in the station status report.
 */
typedef struct
{
  u_int32 messageCount;
  u_int32 uptime;
  double task1_average_execution_time;
  double task1_max_execution_time;
  double task2_average_execution_time;
  double task2_max_execution_time;
  double task3_average_execution_time;
  double task3_max_execution_time;
  double task4_average_execution_time;
  double task4_max_execution_time;
  double task5_average_execution_time;
  double task5_max_execution_time;
  double batteryVoltage;
  char dateString[32];
} ReportedStatusDataType;

#endif
