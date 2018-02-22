/* Derek Schacht
 *  20170922
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
#include <cstring>
#include <sstream>
#include <fstream>

using namespace std;

#include "csv_interface.h"
#include "data_queue.h"
#include "global_queue_list.h"

#define DEBUG(x) //x

static const string WHEADER = "Weather,\
Message Count,\
Interval Time,\
Humidity,\
Pressure,\
Temperature,\
IR Light,\
UV Light,\
White Light,\
Wind Direction,\
Wind Speed,\
Wind Speed (stdev),\
Wind Speed (max),\
Wind Direction @ Max,\
Rain Fall,\
Date";

static const string SHEADER = "Status,\
Message Count,\
Uptime,\
T1 execute (avg),\
T1 execute (max),\
T2 execute (avg),\
T2 execute (max),\
T3 execute (avg),\
T3 execute (max),\
T4 execute (avg),\
T4 execute (max),\
T5 execute (avg),\
T5 execute (max),\
Battery Voltage,\
Date";

/** THREAD: CSV Weather Thread
 */

void* csvWeatherThread(void* param)
{
  Data_Queue<ReportedWeatherDataType> weatherReportQueue;
  GlobalQueueList::reportWeatherQueue.connect(&weatherReportQueue);

  while (serverRunning != 0)
  {
    time64_t currentTime = micros();
    ReportedWeatherDataType weatherReportData = weatherReportQueue.popHead();
    time64_t start1 = micros();
    stringstream csvString;

    csvString << "Weather," << weatherReportData.messageCount;
    csvString << ","        << weatherReportData.intervalTime;
    csvString << ","        << weatherReportData.humidity;
    csvString << ","        << weatherReportData.pressure;
    csvString << ","        << weatherReportData.temperature;
    csvString << ","        << weatherReportData.irLight;
    csvString << ","        << weatherReportData.uvLight;
    csvString << ","        << weatherReportData.whiteLight;
    csvString << ","        << weatherReportData.windDirection;
    csvString << ","        << weatherReportData.windSpeed;
    csvString << ","        << weatherReportData.windSpeedStdev;
    csvString << ","        << weatherReportData.maxWindSpeed;
    csvString << ","        << weatherReportData.windDirectionAtMaxWindSpeed;
    csvString << ","        << weatherReportData.rainfall;
    csvString << ","        << weatherReportData.dateString;
    csvString << endl;

    GlobalQueueList::csvQueue.publish(csvString.str());
    DEBUG(std::cout << "Weather CSV Push (us) ," << micros() - start1 << "," << micros() - currentTime << std::endl);
  }
}

/** THREAD: CSV Status Thread
*/
void *csvStatusThread (void* param)
{
  Data_Queue<ReportedStatusDataType> statusReportQueue;
  GlobalQueueList::reportStatusQueue.connect(&statusReportQueue);

  while (serverRunning != 0)
  {
    ReportedStatusDataType statusReportData = statusReportQueue.popHead();

    stringstream csvString;

    csvString << "Status," << statusReportData.messageCount;
    csvString << ","       << statusReportData.uptime;
    csvString << ","       << statusReportData.task1_average_execution_time;
    csvString << ","       << statusReportData.task1_max_execution_time;
    csvString << ","       << statusReportData.task2_average_execution_time;
    csvString << ","       << statusReportData.task2_max_execution_time;
    csvString << ","       << statusReportData.task3_average_execution_time;
    csvString << ","       << statusReportData.task3_max_execution_time;
    csvString << ","       << statusReportData.task4_average_execution_time;
    csvString << ","       << statusReportData.task4_max_execution_time;
    csvString << ","       << statusReportData.task5_average_execution_time;
    csvString << ","       << statusReportData.task5_max_execution_time;
    csvString << ","       << statusReportData.dateString;
    csvString << endl;

    GlobalQueueList::csvQueue.publish(csvString.str());
  }
}

string createFilename()
{
  char dateBuffer[30];
  timeDateString(dateBuffer, 30);
  string date(dateBuffer);
  return (date + ".csv");
}

/** THREAD: CSV Thread
*/
void *csvThread (void* param)
{
  // Connect to the output queue. This will contain the csv strings to
  // write into the output file.
  Data_Queue<string> outputQueue;
  GlobalQueueList::csvQueue.connect(&outputQueue);  
  
  // Spawn two threads to consume the data that will be placed into the
  // CSV file.
  pthread_t weatherThread_fd;
  pthread_t statusThread_fd;
      
  pthread_create(&weatherThread_fd, NULL, csvWeatherThread, (void*)0);
  pthread_create(&statusThread_fd, NULL, csvStatusThread, (void*)0);
  
  ofstream csvFile;
  csvFile.open(createFilename());
  if (csvFile.is_open())
  {
    csvFile << WHEADER << endl;
    csvFile << SHEADER << endl;
  } else {
    return (void*)0;
  }

  Detection_Functions detect;
  
  while (serverRunning != 0)
  {
    string outputString = outputQueue.popHead();
    if (detect.crossedMidnight())
    {
      csvFile.close();
      delay(1000);
      csvFile.open(createFilename());
      if (csvFile.is_open())
      {
        csvFile << WHEADER << endl;
        csvFile << SHEADER << endl;
      } else {
        return (void*)0;
      }
    }
    csvFile << outputString;
    csvFile.flush();
  }

  csvFile.close();
  
  pthread_join(weatherThread_fd, NULL);
  pthread_join(statusThread_fd, NULL);
}
