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

#include <iostream>
#include <string>

using namespace std;

#include "global_queue_list.h"

#include "data_queue.h"
#include "status_data_types.h"
#include "system_utilities.h"
#include "system_message.h"
#include "receive_thread.h"
#include "weather_thread.h"
#include "status_thread.h"
#include "csv_interface.h"
#include "house_interface.h"

#include "build_date.h"

/*! \file data_manager.cpp
    \brief Provides the main routine and spawns data threads.
     
 */

/*!  DEBUG(x) x
    \brief Turns debug statements 'on' or 'off'
 */
#define DEBUG(x) //x

void* monitor(void*)
{
  Data_Queue<string> csvOutputQueue;
  GlobalQueueList::csvQueue.connect(&csvOutputQueue);  
  time64_t monitorTime = millis();

  while (serverRunning)
  {
    cout << "Connections :"<< 
            GlobalQueueList::weatherQueue.connections() << "," <<
            GlobalQueueList::reportWeatherQueue.connections() << "," <<  
            GlobalQueueList::statusQueue.connections() << "," <<  
            GlobalQueueList::reportStatusQueue.connections() << "," <<  
            GlobalQueueList::wundergroundQueue.connections() << "," <<  
            GlobalQueueList::csvQueue.connections() << endl;
    cout << "Status :"<< 
            GlobalQueueList::weatherQueue.status() << ";" <<
            GlobalQueueList::reportWeatherQueue.status() << ";" <<
            GlobalQueueList::statusQueue.status() << ";" <<
            GlobalQueueList::reportStatusQueue.status() << ";" <<
            GlobalQueueList::wundergroundQueue.status() << ";" <<
            GlobalQueueList::csvQueue.status() << endl;
    //cout << csvOutputQueue.popHead() << endl;
    
    delayUntil(monitorTime + 30000);
    monitorTime += 30000;

  }
}

/* Threads are spawned to perform basic functions. The Data_Manager does
 * not do anything after all of the threads are spawned.
 */

/*** Thread Spawn Tree ***
 * Data_Manager
 *  | + ReceieveThread
 *  | + WeatherThread
 *  |    |+ WeatherUploadThread
 *  | + StatusThread
 *  |    |+ StatusUploadThread
 *  | + houseThread
 *  |    |+ houseWeatherThread
 *  |    |+
 *  | + csvThread
 */

typedef enum
{
  ReceieveThread = 0,   // Thread to receive data from the sensor unit.
  WeatherThread,        // Thread to process weather data.
  StatusThread,         // Thread to process status data. 
  CSVThread,
  MonitorThread,
  HouseThread,
  ThreadCount
} ThreadDescriptorType;

/*! main()
    \brief Main routine.
    Performs program initialization duties and then spawns working threads.
 */
int main()
{

  cout << "Build Date: " << BUILD_DATE << endl;

  try
  {
    char buffer[30] = {};
    time_init();
    serverRunning = 1;
    
    cout << timeDateString(buffer, 30) << endl;
    time64_t start = micros();
    delayUntil((start/1000) + 1000);
    cout << micros() - start << endl;
    cout << timeDateString(buffer, 30) << endl;
  

    while (serverRunning)
    {
      pthread_t threads[ThreadCount];
      
      pthread_create(&threads[ReceieveThread], NULL, receiveThread, (void*)0);
      pthread_create(&threads[WeatherThread],  NULL, weatherThread, (void*)0);  
      pthread_create(&threads[StatusThread],   NULL, statusThread,  (void*)0);
      pthread_create(&threads[CSVThread],      NULL, csvThread,     (void*)0);
      pthread_create(&threads[HouseThread],    NULL, houseThread,   (void*)0);
//      pthread_create(&threads[MonitorThread], NULL, monitor, (void*)0);

      // Apparently this is not a function that exists in the buildroot compiler... uclibc thing?
      // pthread_setname_np(threads[ReceieveThread], "RECEIVE");
      // pthread_setname_np(threads[WeatherThread],  "WEATHER");
      // pthread_setname_np(threads[StatusThread],   "STATUS");
      // pthread_setname_np(threads[CSVThread],      "CSVTHREAD");
      // pthread_setname_np(threads[HouseThread],    "HOUSE");

      for (int i = 0; i < ThreadCount; i++)
      {
        pthread_join(threads[i], NULL);
      }
    }
  }
  catch (...)
  {
    outputError("Well thats the end");
    serverRunning = 0;
  }

  return 0;
}


