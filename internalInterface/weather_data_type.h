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

#ifndef WEATHER_DATA_TYPE_H
#define WEATHER_DATA_TYPE_H

#include "system_types.h"

 /*! \file weather_data_type.h
     \brief Definition of the network message(s) for the sensors <-> dataManager 
     connection.
 */

 /*! \enum MessageKindType
     \brief Values that provide message recognition
 */
typedef enum
{
  WeatherMessage  = 0x57454154, // WEAT
  StatusMessage   = 0x53544154, // STAT
  InternalMessage = 0x494E5445, // INTE
  TextMessage     = 0x54455854, // TEXT
  ResetMessage    = 0x52455345, // RESE
  InvalidMessage  = 0x494e5641  // INVA
} MessageKindType;


/*! \type MessageHeaderType
    \brief Header definition for these messages.
 */
typedef struct
{
  u_int32 messageKindType;
  u_int32 messageCount;
} MessageHeaderType;


/*! \type WeatherDataStruct
    \brief This structure is used in the WeatherMessageType.
 */
typedef struct
{                               // multiplier   | unit
  u_int32 intervalTime;  /*!< \brief Unit: 1      ms                  */
  u_int32 humidity;      /*!< \brief Unit: 0.01   rh%                 */
  u_int32 pressure;      /*!< \brief Unit: 0.001  pascal              */
  int32   temperature;   /*!< \brief Unit: 0.01   C                   */
  u_int32 windDirection; /*!< \brief Unit: 0.1    degree (north = 0)  */
  u_int32 windSpeed;     /*!< \brief Unit: 0.01   mph                 */
  u_int32 rainfall;      /*!< \brief Unit: 0.01   in                  */
  u_int32 irLight;       /*!< \brief Unit: 1      lux                 */
  u_int32 uvLight;       /*!< \brief Unit: 1      lux                 */
  u_int32 whiteLight;    /*!< \brief Unit: 1      lux                 */
} WeatherDataType;


/*! \type WeatherMessageType
    \brief Overall weather message.
 */
typedef struct
{
  MessageHeaderType header;
  WeatherDataType   data;
} WeatherMessageType;


/*! \type SensorStatusDataType
    \brief This structure is used in the StatusMessageType
 */
typedef struct
{                                      // multiplier | unit
  u_int32 bmp180ReadTime;              /*!< \brief Unit: 1     ms                  */
  u_int32 htu21dReadTime;              /*!< \brief Unit: 1     ms                  */
  u_int32 argentReadTime;              /*!< \brief Unit: 1     ms                  */
  u_int32 batteryVoltage;              /*!< \brief Unit: 0.1   volt                */
  u_int32 uptime;                      /*!< \brief Unit: 1     ms                  */
} SensorStatusDataType;

/*! \type WeatherStatusDataType
    \brief This structure is used in the StatusMessageType
 */
typedef struct
{                                          // multiplier | unit
  u_int32 weatherMessageIntervalTime;  /*!< \brief Unit: 1     ms       */
  u_int32 weatherMessageExecutionTime; /*!< \brief Unit: 1     ms       */
  u_int32 weatherQueueCount;           /*!< \brief Unit: 1     n/a      */
  u_int32 weatherTransmitFailureCount; /*!< \brief Unit: 1     n/a      */
} WeatherStatusDataType;


/*! \type WeatherStatusDataType
    \brief This structure is used in the StatusMessageType
 */
typedef struct
{                                          // multiplier | unit
  u_int32 phantIntervalTime;   /*!< \brief Unit: 1     ms       */
  u_int32 phantExecutionTime;  /*!< \brief Unit: 1     ms       */
  u_int32 phantQueueCount;     /*!< \brief Unit: 1     n/a      */
  u_int32 phantFailureCount;   /*!< \brief Unit: 1     n/a      */
} PhantStatusDataType;


/*! \type SensorStatusMessageType
    \brief Overall status message
 */
typedef struct
{
  MessageHeaderType     header;
  SensorStatusDataType  data;

} SensorStatusMessageType;

/*! \type TextMessageType
    \brief Message that provides a way to report text.
 */
typedef struct
{
  MessageHeaderType header;
  byte              data[64]; /*!< \brief Error Description                */
} TextMessageType;


/*! \type ResetMessageType
    \brief Reset message for the Data Manager.
 */
typedef struct
{
  MessageHeaderType header;
} ResetMessageType;


#endif

