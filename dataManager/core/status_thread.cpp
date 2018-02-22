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

using namespace std;

#include "global_queue_list.h"
#include "global_properties.h"
#include "numerical_statistics.h"
#include "system_utilities.h"      // for the serverRunning flag.
#include "station_interface.h"
#include "status_thread.h"


/*! \file status_thread.cpp
    \brief Provides threads for collecting and reporting status data.
*/

typedef struct
{
  Numerical_Statistics<double>* uploadData;
  mutex* uploadMutex;
  int uploadInterval;
} StatusUploadThreadParamType;


void recordStatusData (StatusMessageType data, 
                       Numerical_Statistics<double>* sensedData, 
                       mutex* numericalData)
{
  double value = sensedData[data.messageKind].getMean();
  lock_guard<mutex> lock(*numericalData);
  sensedData[data.messageKind].storeValue(data.data);

  if (sensedData[data.messageKind].getMean() < 0.0 && 
      data.messageKind == Sensor_Read_Interval)
  {
      cout << "New : Added : Previous" << 
              sensedData[data.messageKind].getMean() << " : " << 
              data.data << " : " << 
              value << endl;
  }
}

ReportedStatusDataType populateRecordedStatusData (Numerical_Statistics<double>* sensedData, 
                                                   int interval, 
                                                   int messageCount)
{
  const int bufferSize = 32;
  ReportedStatusDataType reportedStatusData;
  
  char* dateBuffer = new char[bufferSize];
  dateBuffer = timeDateString(dateBuffer, bufferSize - 1);
  
  memcpy(reportedStatusData.dateString, dateBuffer, bufferSize - 1);  
  delete dateBuffer;

  reportedStatusData.messageCount                 = messageCount;
  reportedStatusData.uptime                       = sensedData[Sensor_Uptime].getStoredValue();
  reportedStatusData.task1_average_execution_time = sensedData[BMP180_Read_Time].getMean();
  reportedStatusData.task1_max_execution_time     = sensedData[BMP180_Read_Time].getMax();
  reportedStatusData.task2_average_execution_time = sensedData[HTU21D_Read_Time].getMean();
  reportedStatusData.task2_max_execution_time     = sensedData[HTU21D_Read_Time].getMax();
  reportedStatusData.task3_average_execution_time = sensedData[Argent_Read_Time].getMean();
  reportedStatusData.task3_max_execution_time     = sensedData[Argent_Read_Time].getMax();
  reportedStatusData.task4_average_execution_time = sensedData[Sensor_Read_Interval].getMean();
  reportedStatusData.task4_max_execution_time     = sensedData[Sensor_Read_Interval].getMax();
  reportedStatusData.task5_average_execution_time = sensedData[Phant_Tx_Time].getMean();
  reportedStatusData.task5_max_execution_time     = sensedData[Phant_Tx_Time].getMax();
  reportedStatusData.batteryVoltage               = sensedData[Battery_Voltage].getMean();

  return reportedStatusData;
}

void* statusUploadThread(void* param)
{
  StatusUploadThreadParamType* parameters = (StatusUploadThreadParamType*)param;
  
  cout << "SREPORT(ms): " << parameters->uploadInterval << endl;
  
  time64_t statusTime = millis();
  int statusMessageCount = 0;
  int correctionFactor = 0;
  
  while (serverRunning != 0)
  {
    delayUntil(statusTime + parameters->uploadInterval);

    // Snapshot the time to prevent processing time from altering
    // desired behavior.
    
    time64_t currentTime = millis();
    time64_t start1 = micros();
    {
      lock_guard<mutex> lock(*(parameters->uploadMutex));

      DEBUG(cout << "SINTERVAL(ms):" << currentTime - statusTime << endl);

      GlobalQueueList::reportStatusQueue.publish
           (populateRecordedStatusData
                (parameters->uploadData, 
                 currentTime - statusTime, 
                 statusMessageCount++));

      // Reset the status data so that the next window of status data
      // has the same statistics weight.

      for (int i = 0; i < StatusMessageKindCount; i++)
      {
        parameters->uploadData[i].reset();
      }
    }

    DEBUG(cout << "SUPLOAD(us, queue):," << 
                  micros() - start1 << "," << 
                  GlobalQueueList::reportStatusQueue.status() << endl);

    statusTime += parameters->uploadInterval;
  }
}


void *statusThread (void* param)
{

  Detection_Functions time;

  int uploadInterval;
  properties.getElement(PROPERTY__Status_Upload_Interval, &uploadInterval);
  
  // Output a quick message indicating that the tread is alive.
  DEBUG(cout << "STATUS: " <<  uploadInterval << endl);
  
  // Snapshot the time. This will later be used to compute when to put 
  // data onto the phant queue.
  time64_t statusTime = millis();
  
  // This array keeps all the statistics information for the current 
  // window of weather data.
  Numerical_Statistics<double> statusData[StatusMessageKindCount];
  
  // Controls access to the statusData declared above.
  mutex uploadMutex;
  
  // Structure for the upload thread.
  StatusUploadThreadParamType uploadThreadData = {statusData,
                                                  &uploadMutex,
                                                  uploadInterval};
  
  pthread_t uploadThread_fd;      
  pthread_create(&uploadThread_fd, NULL, statusUploadThread, (void*)&uploadThreadData);
  
  Data_Queue<StatusMessageType> statusQueue;
  GlobalQueueList::statusQueue.connect(&statusQueue);
  
  // Loop forever performing the functions of this thread:
  //  - read messages from the status queue.
  //  - record the status data into the status statistics data.
  //  - perform the midnight resets.

  while (serverRunning != 0)
  {
    StatusMessageType statusMessage = statusQueue.popHead();
    
    // Record the status data from the tasks into the statistics data.
    recordStatusData(statusMessage, statusData, &uploadMutex);

    // Data elements get reset at midnight.
    if (time.crossedMidnight())
    {
      for (int i = 0; i < StatusMessageKindCount; i++)
      {
        statusData[i].reset();
      }
    }
  }
  
  pthread_join(uploadThread_fd, NULL);
}

