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

using namespace std;

#include "system_utilities.h"
#include "system_message.h"

/*! \file system_message.cpp
    \brief Provides convenience functions for printing messages.
*/

void outputError(const char* string)
{
  cout << "ERROR: " << string << endl;
  delay(1);
}


void outputMessage(const char* string)
{
  cout << "MSG: " << string << endl;
  delay(1);
}

