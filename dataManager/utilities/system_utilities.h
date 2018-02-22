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
#ifndef SYSTEM_UTILITIES_H
#define SYSTEM_UTILITIES_H

#include <ctime>

/*! \file system_utilities.h
    \brief Provides an easy wrapper some basic system functions.
*/

typedef long long time64_t;

extern timespec boot;

/*! \var serverRunning
    \brief Provides an indication to the threads of whether or not to continue.
*/
extern volatile int serverRunning;

// Initialization call for time functions in this file. Must be called
// at program start.. or anytime time needs to be rebaselined to now.

void time_init();

// Arduino style time functions. 

time64_t micros(); // 2^63 / 1000000 / 60 / 60 / 24 / 365 = ~ 292471 (Years)
time64_t millis(); // ~ micros * 1000 

// Arduino style delay functions.

void delay(time64_t millis);
void delayMicroseconds(time64_t micros);

// Probably the one function that Arduino would benefit from the most.

void delayUntil(time64_t waitMillis);

// This function will return a time string of the form:
//   YYYY-MM-DDTHH:MM:SS:mmmZ

char* timeDateString(char* buffer, int buffer_len);

// This function will return true if time has crossed midnight since
// the last call to the function.

class Detection_Functions
{
  public:
    Detection_Functions();
    bool crossedMidnight();
    
  private:
    struct tm previous_tm;
  
};


#endif
