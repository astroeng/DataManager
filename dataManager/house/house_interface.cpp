/* Derek Schacht
 *  20170924
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and cite code that I find on the internet but I am not 
 *               perfect. If you find something that should be cited let me know
 *               and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right 
 *  to modify this in the future.
 */

#include <string>

using namespace std;

#include "element.h"
#include "multicast.h"
#include "serialize.h"
#include "house_interface.h"
#include "data_queue.h"
#include "global_queue_list.h"
#include "global_properties.h"

#define DEBUG(x) //x

Data_Queue<Serialize> houseQueue;

double windDirection2(double direction)
{
  while (direction < 0)
  {
    direction += 360.0;
  }
  while (direction >= 360.0)
  {
    direction -= 360.0;
  }
  return direction;
}

Element_Type buildElement(ElementKind_Type elementKind,
                          double           data)
{
  unsigned int weatherStationID;

  properties.getElement(PROPERTY__Weather_Station_ID, 
                        &weatherStationID);
  
  Element_Type element;
  
  element.valid     = true;
  element.elementID = elementKind;
  element.dataKind  = DataKind__Double;
  element.stationID = weatherStationID;
  element.double64  = data;

  strcpy(element.description, ElementKind_String[element.elementID]);
  
  return element;
}

Serialize serializeWeatherData(const ReportedWeatherDataType sensedData, int interval, int messageCount)
{
  double altitude;
  properties.getElement(PROPERTY__Weather_Station_Altitude,
                        &altitude);

  Serialize localDataElements(9);
  
  localDataElements.setElement(buildElement(ElementKind__Index,          messageCount));
  localDataElements.setElement(buildElement(ElementKind__Duration,       interval));
  localDataElements.setElement(buildElement(ElementKind__Humidity,       sensedData.humidity));
  localDataElements.setElement(buildElement(ElementKind__Pressure,       sensedData.pressure));
  localDataElements.setElement(buildElement(ElementKind__Temperature,    sensedData.temperature));
  localDataElements.setElement(buildElement(ElementKind__Wind_Direction, windDirection2(sensedData.windDirection)));
  localDataElements.setElement(buildElement(ElementKind__Wind_Speed,     sensedData.windSpeed));
  localDataElements.setElement(buildElement(ElementKind__Rain_Fall,      sensedData.rainfall));
  localDataElements.setElement(buildElement(ElementKind__Altitude,       altitude));
 
  return localDataElements;
}

/** THREAD: House Weather Thread
 */

void* houseWeatherThread(void*)
{
  Data_Queue<ReportedWeatherDataType> weatherReportQueue;
  GlobalQueueList::reportWeatherQueue.connect(&weatherReportQueue);

  unsigned int count = 0;

  while (serverRunning != 0)
  {
    time64_t currentTime = micros();

    ReportedWeatherDataType weatherReportData = weatherReportQueue.popHead();
    time64_t start1 = micros();
    Serialize data = serializeWeatherData(weatherReportData,millis() - currentTime,count++);
    houseQueue.pushTail(data);

    DEBUG(std::cout << "Serialize Push (us) ," << micros() - start1 << "," << micros() - currentTime << std::endl);
  }
}


/** THREAD: HOUSE Thread
*/
void *houseThread (void*)
{
  // Spawn two threads to consume the data that will be placed into the
  // Multicast message.
  pthread_t weatherThread_fd;
//  pthread_t statusThread_fd;

  pthread_create(&weatherThread_fd, NULL, houseWeatherThread, (void*)0);
//  pthread_create(&weatherThread_fd, NULL, houseStatusThread, (void*)0);

  string multicastAddress;
  string localAddress;
  unsigned int multicastPort;

  properties.getElement(PROPERTY__Multicast_Remote_Address, &multicastAddress);
  properties.getElement(PROPERTY__Multicast_Bind_Address, &localAddress);
  properties.getElement(PROPERTY__Multicast_Port, &multicastPort);

  Multicast_Interface multicast = Multicast_Interface
                                        (multicastAddress.c_str(),
                                         multicastPort,
                                         localAddress.c_str(),
                                         multicastPort);

  while (serverRunning != 0)
  {

    Serialize data = houseQueue.popHead();

    data.getElement(ElementKind__Temperature).print();

    unsigned char multicastBuffer[data.structureSize()];

    data.returnBuffer(multicastBuffer, data.structureSize());
    multicast.sendTo(multicastBuffer, data.structureSize());

  }

  pthread_join(weatherThread_fd, NULL);
}
