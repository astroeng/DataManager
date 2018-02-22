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


#include <cstring>
#include <iostream>

using namespace std;

#include "system_utilities.h"

/*! \file system_utilities.cpp
    \brief Provides an easy wrapper some basic system functions.
*/

#define DEBUG(x) //x

timespec boot;
volatile int serverRunning;

void time_init()
{
  // Prime the boot structure with the current time.
  clock_gettime(CLOCK_REALTIME, &boot);
}


timespec ts_diff(timespec current, timespec past)
{
  current.tv_sec  = current.tv_sec - past.tv_sec;
  current.tv_nsec = current.tv_nsec - past.tv_nsec;
  if (current.tv_nsec < 0)
  {
    current.tv_sec--;
    current.tv_nsec += 1000000000ll;
  }
  
  return current;

}

time64_t micros()
{
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts = ts_diff(ts, boot);

  return ts.tv_sec * 1000000ll + ts.tv_nsec / 1000ll;
}

time64_t millis()
{
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts = ts_diff(ts, boot);

  return ts.tv_sec * 1000ll + ts.tv_nsec / 1000000ll;
}

void delay(time64_t millis)
{
  timespec ts;
  ts.tv_sec = millis/1000ll;
  ts.tv_nsec = (millis % 1000ll) * 1000000ll;
  nanosleep(&ts, 0);
}

void delayMicroseconds(time64_t micros)
{
  timespec ts;
  ts.tv_sec = micros/1000000ll;
  ts.tv_nsec = (micros % 1000000ll) * 1000ll;
  nanosleep(&ts, 0);
}

void delayUntil(time64_t waitMillis)
{
  waitMillis = waitMillis - millis();
  delay(waitMillis);
}

char* timeDateString(char* buffer, int buffer_len)
{
  char milliSecondsBuffer[4];
  timespec rawTime;
  struct tm * timeinfo;

  clock_gettime(CLOCK_REALTIME, &rawTime);
  
  timeinfo = localtime(&rawTime.tv_sec);

  strftime(buffer, buffer_len, "%FT%T.xxxZ",timeinfo);

  sprintf(milliSecondsBuffer,"%03lld",rawTime.tv_nsec / 1000000ll);

  memcpy(strstr(buffer, "xxx"),milliSecondsBuffer,3);

  return buffer;
}

Detection_Functions::Detection_Functions()
{
  crossedMidnight();
}

bool Detection_Functions::crossedMidnight()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime(&rawtime);

  DEBUG(std::cout << "midnight: " << previous_tm.tm_mday << " " << timeinfo->tm_mday << std::endl);

  if (previous_tm.tm_mday != timeinfo->tm_mday)
  {
    memcpy(&previous_tm, timeinfo, sizeof(previous_tm));
    return true;
  }

  memcpy(&previous_tm, timeinfo, sizeof(previous_tm));
  return false;
}

