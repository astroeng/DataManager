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
 
#ifndef RECEIVE_THREAD_H
#define RECEIVE_THREAD_H

/*! \file receive_thread.h
    \brief Provides the thread that processes the data from the sensors.
*/

#include "data_queue.h"
#include "weather_data_type.h"
#include "status_data_types.h"

/*! \file receive_thread.h
    \brief Provides the receiver for the data from the sensor unit.
*/

/*! \fn void* dataThread( void* )
    \brief Provides the entry point for the dataThread task.
*/
void* receiveThread( void* );
 
#endif
