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
 
#ifndef STATUS_THREAD_H
#define STATUS_THREAD_H

#include "data_queue.h"
#include "status_data_types.h"

/*! \file status_thread.h
    \brief Provides threads for collecting and reporting status data.
*/

void *statusThread (void* param);

#endif
