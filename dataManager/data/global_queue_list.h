/* Derek Schacht
 *  2017 09 22
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and cite code that I find on the internet but I am not 
 *               perfect. If you find something that should be cited let me know
 *               and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right 
 *  to modify this in the future.
 */

#ifndef GLOBAL_QUEUE_LIST_H
#define GLOBAL_QUEUE_LIST_H

#include <string>

#include "weather_data_type.h"
#include "status_data_types.h"
#include "station_interface.h"
#include "queue_manager.h"

// The UDP Thread Receives all messages from the sensors. This data is 
// then processed by either the Weather Thread or the Status Thread. The
// processed data is then put into their respective reportQueues. The
// reportQueues are used to generate the outgoing station messages.
//
// The phantQueue holds the HTTP GET/POST message frame that the phant 
// thread will use when it connects to the server. <REMOVED> <Phant was
// turned off by sparkfun>
// 
// The wundergroundQueue is for weather underground.
// 
// The csvQueue is for writing the data to a csv.

namespace GlobalQueueList
{

  extern Queue_Manager<WeatherMessageType>      weatherQueue;
  extern Queue_Manager<ReportedWeatherDataType> reportWeatherQueue;
  
  extern Queue_Manager<StatusMessageType>       statusQueue;
  extern Queue_Manager<ReportedStatusDataType>  reportStatusQueue;

  extern Queue_Manager<std::string>  wundergroundQueue;
  extern Queue_Manager<std::string>  csvQueue;

}
#endif
