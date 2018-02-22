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
#include <cstring>

using namespace std;

#include "global_queue_list.h"
#include "global_properties.h"
#include "system_utilities.h"
#include "system_message.h"
#include "udp_interface.h"
#include "numerical_statistics.h"
#include "weather_data_type.h"
#include "receive_thread.h"

/*! \file receive_thread.cpp
    \brief Provides the thread that processes the data from the sensors.
*/

#define DEBUG(x) //x


void *receiveThread( void* )
{

  Detection_Functions time;

  string networkPort;
  properties.getElement(PROPERTY__UDP_Receive_Port, &networkPort);

  cout << "RECEIVE: " << networkPort << endl;

  try 
  {
  
    UDP_Interface newClient(networkPort.c_str());  

    const int bufferSize     = 128;
    const int timeBufferSize = 80;
    
    time64_t start;
    
    time64_t statusTime      = millis();
    time64_t weatherInterval = micros();
    time64_t statusInterval  = micros();
    
    Numerical_Statistics<long long> receiveStats;
    Numerical_Statistics<long long> processingStats;
    Numerical_Statistics<long long> weatherDataIntervalStats;
    Numerical_Statistics<long long> statusDataIntervalStats;

    while(serverRunning != 0)
    {
      char timeBuffer[timeBufferSize];
      byte buffer[bufferSize];
      
      bzero(buffer, bufferSize);

      int bytes = newClient.readFrom(buffer, bufferSize);
      start = micros();
      
      DEBUG(cout << endl << "Received Bytes: " << bytes << endl);
      DEBUG(cout << "Local Time: " << timeDateString(timeBuffer, timeBufferSize) << endl);
      
      MessageHeaderType header;
      memcpy(&header, buffer, sizeof(MessageHeaderType));

      switch(header.messageKindType)
      {
      case WeatherMessage:
        DEBUG(cout << "Message Type: Weather Header @ " << 
                      "Message Time: " << timeDateString(timeBuffer, timeBufferSize) << 
                      "Message Size: " << bytes << endl);
    
        // weatherMessage is a typed structure that matches the type
        // structure of the message sent by the client.
        WeatherMessageType weatherMessage;

        // If the expected size matches the number of read bytes copy the 
        // data into a data structure and then process the received data.
        if (bytes == sizeof (WeatherMessageType))
        {
          memcpy(&weatherMessage, buffer, sizeof (WeatherMessageType));
          
          GlobalQueueList::
          weatherQueue.publish(weatherMessage);
          
          StatusMessageType statusMessage;
          statusMessage.messageKind = Sensor_Message_Receive_Interval;
          statusMessage.data        = (start - weatherInterval) / 1000;
          GlobalQueueList::statusQueue.publish(statusMessage);
          
          statusMessage.messageKind = Sensor_Read_Interval;
          statusMessage.data        = (start - weatherInterval) / 1000;
          GlobalQueueList::statusQueue.publish(statusMessage);
          
          weatherDataIntervalStats.storeValue(start - weatherInterval);
          weatherInterval = start;
        }
        else
        {
          outputError("DATA THREAD: Bad Weather Message");
        }
        break;

      case StatusMessage:
        DEBUG(cout << "Message Type: Status Header @ " << 
                      "Message Time: " << timeDateString(timeBuffer, timeBufferSize) << 
                      "Message Size: " << bytes << endl);

        SensorStatusMessageType sensorStatusMessage;

        if (bytes == sizeof (SensorStatusMessageType))
        {
          memcpy(&sensorStatusMessage, buffer, sizeof (SensorStatusMessageType));
          
          StatusMessageType statusMessage;
          statusMessage.messageKind = BMP180_Read_Time;
          statusMessage.data        = sensorStatusMessage.data.bmp180ReadTime;
          GlobalQueueList::statusQueue.publish(statusMessage);
          
          statusMessage.messageKind = HTU21D_Read_Time;
          statusMessage.data        = sensorStatusMessage.data.htu21dReadTime;
          GlobalQueueList::statusQueue.publish(statusMessage);
          
          statusMessage.messageKind = Argent_Read_Time;
          statusMessage.data        = sensorStatusMessage.data.argentReadTime;
          GlobalQueueList::statusQueue.publish(statusMessage);
          
          statusMessage.messageKind = Battery_Voltage;
          statusMessage.data        = sensorStatusMessage.data.batteryVoltage;
          GlobalQueueList::statusQueue.publish(statusMessage);
          
          statusMessage.messageKind = Sensor_Uptime;
          statusMessage.data        = sensorStatusMessage.data.uptime;
          GlobalQueueList::statusQueue.publish(statusMessage);
          
          statusDataIntervalStats.storeValue(start - statusInterval);
          statusInterval = start;
        }
        else
        {
          outputError("DATA THREAD: Bad Status Message");
        }

        break;
      case TextMessage:
      
        TextMessageType textMessage;
        if (bytes == sizeof (TextMessageType))
        {
          memcpy(&textMessage, buffer, sizeof (TextMessageType));
          cout << "SENSOR MESSAGE: " << textMessage.data << endl;
        }
        else
        {
          outputError("DATA THREAD: Bad Error Message");
        }        
      
        break;
      case ResetMessage:
        DEBUG(cout << "Message Type: Reset Header @" << 
                      "Message Time: " << timeDateString(timeBuffer, timeBufferSize) << 
                      "Message Size: " << bytes << endl);

        serverRunning = 0;

        break;
      default:
        outputError("MAIN: Bad Header");
        break;
      }

      DEBUG(buffer[4] = 0);
      DEBUG(cout << buffer << " " << endl);

      // Compute how long it took to receive and process the message from the
      // sensor unit.
      receiveStats.storeValue(micros() - start);

      time64_t currentTime = millis();

      if (time.crossedMidnight())
      {
        receiveStats.reset();
        processingStats.reset();
        weatherDataIntervalStats.reset();
      }

      processingStats.storeValue(micros() - start);

      DEBUG(cout << "WDATA: " << weatherDataIntervalStats.getMean() << " " <<
                                 weatherDataIntervalStats.getMax() << " " << 
                                 weatherDataIntervalStats.getMin() << " " << 
                                 weatherDataIntervalStats.getStdev() << endl);

      DEBUG(cout << "RTIME: " << receiveStats.getMean() << " " <<
                                 receiveStats.getMax() << " " << 
                                 receiveStats.getMin() << " " << 
                                 receiveStats.getStdev() << endl);
                                 
      DEBUG(cout << "PTIME: " << processingStats.getMean() << " " <<
                                 processingStats.getMax() << " " << 
                                 processingStats.getMin() << " " << 
                                 processingStats.getStdev() << endl);
    }
  
  }
  catch ( UDP_InterfaceExceptionType ex )
  {
  
    outputError("RECEIVE: Could not create UDP Interface");
    serverRunning = 0;
  
  }

}
