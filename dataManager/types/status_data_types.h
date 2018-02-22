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

#ifndef STATUS_DATA_TYPE_H
#define STATUS_DATA_TYPE_H

#include "system_types.h"

 /*! \enum StatusMessageKindType
     \brief Values that provide message recognition
 */
typedef enum
{
  BMP180_Read_Time = 0, // Sent from the sensorManager
  HTU21D_Read_Time,     // Sent from the sensorManager
  Argent_Read_Time,     // Sent from the sensorManager
  Sensor_Read_Interval,
  Battery_Voltage,      // Sent from the sensorManager
  Sensor_Uptime,        // Sent from the sensorManager
  Phant_Tx_Time,
  Phant_Interval_Time,
  Weather_Underground_Tx_Time,
  Weather_Underground_Interval_Time,
  Sensor_Message_Receive_Processing_Time,
  Sensor_Message_Receive_Interval,
  StatusMessageKindCount
} StatusMessageKindType;

static const char* StatusMessageKindTypeText[] = 
{
  "BMP180_Read_Time",
  "HTU21D_Read_Time",
  "Argent_Read_Time",
  "Sensor_Read_Interval",
  "Battery_Voltage",
  "Sensor_Uptime",
  "Phant_Tx_Time",
  "Phant_Interval_Time",
  "Weather_Underground_Tx_Time",
  "Weather_Underground_Interval_Time",
  "Sensor_Message_Receive_Processing_Time",
  "Sensor_Message_Receive_Interval"
};

 /*! \enum StatusMessageType
     \brief Structure of the status message.
 */
typedef struct
{
  StatusMessageKindType messageKind;
  int32_t               data;
} StatusMessageType;

#endif
