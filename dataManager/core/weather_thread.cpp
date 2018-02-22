/* Derek Schacht
 *  2016 10 28
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and cite code that I find on the internet but I am not 
 *               perfect. If you find something that should be cited let me know
 *               and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right 
 *  to modify this in the future.
 */

#define DEBUG(x) //x

#include <iostream>
#include <cstring>
#include <mutex>
#include <cmath>

using namespace std;

#include "global_queue_list.h"
#include "global_properties.h"

#include "numerical_statistics.h"
#include "system_utilities.h"
#include "station_interface.h"
#include "weather_thread.h"

#include "element.h"
#include "serialize.h"
#include "multicast.h"

#define PASCAL_TO_INHG(x) 0.295333727*x
#define C_TO_F(x) ((9.0*x)/5.0)+32.00

/*! \file weather_thread.cpp
    \brief Provides threads for collecting and reporting weather data.
*/

/*! \struct WeatherUploadThreadParamType
    \brief Data for thread operation
    \param sendQueue The queue to push reports onto.
    \param uploadData The data store to generate the report from.
    \param uploadMutex Mutext to control access to uploadData.
    \param uploadInterval How often to send the data (ms)
 */

typedef struct
{
  Numerical_Statistics<double>* uploadData;
  mutex* uploadMutex;
  int uploadInterval;
} WeatherUploadThreadParamType;

typedef enum 
{
  Humidity = 0,
  Pressure,
  Temperature,
  WindDirection,
  WindDirectionAtMaxSpeed,
  WindSpeed,
  Rainfall,
  IrLight,
  UvLight,
  WhiteLight,
  WeatherDataCount
} WeatherDataFields;


/*! \fn double windDirection(double direction)
    \brief Saves the current wind direction
    \param direction wind direction
    
    This function resolves any issues with range that might have been introduced
    by the circular correction functions.
 */

double windDirection(double direction)
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


/*! \fn double currentWindDirection(double currentDirection, double meanDirection)
    \brief Computes the corrected wind direction.
    \param currentDirection current observed wind direction
    \param meanDirection average observed wind direction
    
    This function compares the two values and produces a corrected wind direction.
 */

double currentWindDirection(double currentDirection, double meanDirection)
{
  if ((abs(currentDirection - meanDirection)) > 180.0)
  {
    currentDirection -= 360.0;
  }
  return currentDirection;
}


/*! \fn void addWindDirection(double newDirection, Numerical_Statistics<double> &stats)
    \brief Saves the current wind direction
    \param newDirection current observed wind direction
    \param stats where to save the wind direction
    
    This function uses the other wind related functions to correct for the 
    circular nature of wind direction (360 = 0)
 */

void addWindDirection(double newDirection, Numerical_Statistics<double> &stats)
{
  stats.storeValue(currentWindDirection(newDirection,stats.getMean()));
}


/*! \fn void recordWeatherData (WeatherMessageType data, Numerical_Statistics<double>* sensedData, mutex* numericalData)
    \brief Saves the current sensor data.
    \param data weather data collected from sensors
    \param sensedData where the weather data is saved
    \param numericalData mutext to control access to the sensedData
 */

void recordWeatherData (WeatherMessageType data, Numerical_Statistics<double>* sensedData, mutex* numericalData)
{
  lock_guard<mutex> lock(*numericalData);
  
  sensedData[Humidity].includeValue(((double)data.data.humidity)/100.0);
  sensedData[Pressure].includeValue(((double)data.data.pressure)/1000.0);
  sensedData[Temperature].includeValue(((double)data.data.temperature)/100.0);
  sensedData[WindSpeed].includeValue(((double)data.data.windSpeed)/100.0);
  sensedData[Rainfall].storeValue(((double)data.data.rainfall/1000.0) + sensedData[Rainfall].getStoredValue());
  sensedData[IrLight].includeValue(((double)data.data.irLight));
  sensedData[UvLight].includeValue(((double)data.data.uvLight));
  sensedData[WhiteLight].includeValue(((double)data.data.whiteLight));
  
  addWindDirection(((double)data.data.windDirection)/10.0,sensedData[WindDirection]);
  
  if (sensedData[WindSpeed].isNewMax())
  {
    sensedData[WindDirectionAtMaxSpeed].storeValue(((double)data.data.windDirection/10.0));
  }
}

/*! \fn ReportedWeatherDataType populateRecordedWeatherData(Numerical_Statistics<double>* sensedData, int interval, int messageCount)
    \brief Populates the weather report with data
    \param sensedData weather data collected from sensors
    \param interval duration since last weather report
    \param messageCount number of weather reports sent
 */

ReportedWeatherDataType populateRecordedWeatherData
    (Numerical_Statistics<double>* sensedData, 
     int interval, 
     int messageCount)
{

  ReportedWeatherDataType reportedWeatherData;
  char* dateBuffer = new char[32];
  dateBuffer = timeDateString(dateBuffer,31);

  // Copy over the data elements and perform any unit conversions.
  memcpy(reportedWeatherData.dateString, dateBuffer, 31);
  delete dateBuffer;

  reportedWeatherData.messageCount   = messageCount;
  reportedWeatherData.intervalTime   = interval;
  reportedWeatherData.humidity       = sensedData[Humidity].getMean();
  reportedWeatherData.pressure       = PASCAL_TO_INHG(sensedData[Pressure].getMean());
  reportedWeatherData.temperature    = C_TO_F(sensedData[Temperature].getMean());
  reportedWeatherData.rainfall       = sensedData[Rainfall].getStoredValue();
  reportedWeatherData.windDirection  = windDirection(sensedData[WindDirection].getMean());
  reportedWeatherData.windSpeed      = sensedData[WindSpeed].getMean();
  reportedWeatherData.maxWindSpeed   = sensedData[WindSpeed].getMax();
  reportedWeatherData.windSpeedStdev = sensedData[WindSpeed].getStdev();
  reportedWeatherData.irLight        = sensedData[IrLight].getMean();
  reportedWeatherData.uvLight        = sensedData[UvLight].getMean();
  reportedWeatherData.whiteLight     = sensedData[WhiteLight].getMean();
        
  reportedWeatherData.windDirectionAtMaxWindSpeed = windDirection(sensedData[WindDirectionAtMaxSpeed].getStoredValue());
  
  return reportedWeatherData;
}


/** THREAD: Weather Upload Thread
 * 
 * The weather upload thread waits for a given interval before building and 
 * sending the weather report.
 */

/*! \fn void* weatherUploadThread(void* param)
    \brief Generates and Uploads Weather Reports
    \param param Parameters for thread operation.
 */

void* weatherUploadThread(void* param)
{
  WeatherUploadThreadParamType* parameters = (WeatherUploadThreadParamType*)param;
  
  cout << "WREPORT(ms): " << parameters->uploadInterval << endl;
  
  time64_t weatherTime = millis();
  int weatherMessageCount = 0;
  int correctionFactor = 0;
  
  while (serverRunning != 0)
  {
    delayUntil(weatherTime + parameters->uploadInterval);

    // Snapshot the time to prevent processing time from altering 
    // desired behavior.    
    
    time64_t currentTime = millis();
    time64_t start1 = micros();

    {
      lock_guard<mutex> lock(*(parameters->uploadMutex));

      DEBUG(cout << "WINTERVAL(ms):" << currentTime - weatherTime << endl);

      GlobalQueueList::reportWeatherQueue.publish
            (populateRecordedWeatherData(parameters->uploadData, 
                                         currentTime - weatherTime, 
                                         weatherMessageCount++));

      // reset the weather data so that the next window of weather data
      // has the same statistics weight.
      for (int i = 0; i < WeatherDataCount; i++)
      {
        parameters->uploadData[i].reset();
      }
    }

    DEBUG(cout << "WREPORT(us, queue):," << micros() - start1 << "," << GlobalQueueList::reportWeatherQueue.status() << endl);

    weatherTime += parameters->uploadInterval;
  }
}


/** THREAD: Weather Thread
 *
 * The weather thread collects individual sensor reports and adds them to the
 * data store for weather data. It also spawns a worker thread that will build 
 * the weather report from the data store and then pass it to the report sender.
 */

/*! \fn void *weatherThread (void* param)
    \brief Collects and stores the weather data from sensors
    \param param Parameters for thread operation.
 */

void *weatherThread (void* param)
{

  Detection_Functions time;
  
  int uploadInterval;
  properties.getElement(PROPERTY__Weather_Upload_Interval, &uploadInterval);

  // Output a quick message indicating that the tread is alive.
  DEBUG(cout << "WEATHER: " << uploadInterval << endl);
  
  // Snapshot the time. This will later be used to compute when to put
  // data onto the phant queue.
  time64_t weatherTime = millis();
  
  // This array keeps all the statistics information for the current
  // window of weather data.
  Numerical_Statistics<double> weatherData[WeatherDataCount];
  
  // Controls access to the weatherData declared above.
  mutex uploadMutex;
  
  // Structure for the upload thread.
  WeatherUploadThreadParamType uploadThreadData = {weatherData,
                                                   &uploadMutex,
                                                   uploadInterval};
  
  pthread_t uploadThread_fd;

  pthread_create(&uploadThread_fd, NULL, weatherUploadThread, (void*)&uploadThreadData);
  
  Data_Queue<WeatherMessageType> weatherQueue;
  GlobalQueueList::weatherQueue.connect(&weatherQueue);
  
  // Loop forever performing the functions of this thread:
  //  - read messages from the weather queue.
  //  - record the weather data into the weather statistics data.
  //  - check to see if enough time has elapsed since sending the last
  //    weather report to the remote service.
  //  - upon reporting the weather data, reset the statistics fields.
  //  - perform the midnight resets.
  while (serverRunning != 0)
  {
    WeatherMessageType sensorData = weatherQueue.popHead();
    
    // Record the weather data from the sensors into the statistics data.
    recordWeatherData(sensorData, weatherData, &uploadMutex);

    // Some data elements get reset at midnight.
    if (time.crossedMidnight())
    {
      weatherData[Rainfall].storeValue(0);
    }
    
  }
  
  pthread_join(uploadThread_fd, NULL);
}

