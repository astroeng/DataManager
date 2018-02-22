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
 
#ifndef WEATHER_THREAD_H
#define WEATHER_THREAD_H

#include <string>

using namespace std;

#include "data_queue.h"
#include "weather_data_type.h"

/*! \file weather_thread.h
    \brief Provides threads for collecting and reporting weather data.
*/

/*! \fn void *weatherThread (void* param)
    \brief Collects and stores the weather data from sensors
    \param param Parameters for thread operation.
 */

void *weatherThread (void* param);

#endif
