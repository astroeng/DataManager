/* Derek Schacht
 *  2017 09 23
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and cite code that I find on the internet but I am not 
 *               perfect. If you find something that should be cited let me know
 *               and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right 
 *  to modify this in the future.
 */

#include "weather_data_type.h"
#include "status_data_types.h"
#include "station_interface.h"
#include "queue_manager.h"


namespace GlobalQueueList
{

  Queue_Manager<WeatherMessageType>      weatherQueue;
  Queue_Manager<ReportedWeatherDataType> reportWeatherQueue;
  
  Queue_Manager<StatusMessageType>       statusQueue;
  Queue_Manager<ReportedStatusDataType>  reportStatusQueue;

  Queue_Manager<std::string>  wundergroundQueue;
  Queue_Manager<std::string>  csvQueue;

}
