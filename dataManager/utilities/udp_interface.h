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

#ifndef UDP_INTERFACE_H
#define UDP_INTERFACE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "system_types.h"

/*! \file udp_interface.h
    \brief Provides an easy wrapper for a UDP receiver.
*/

typedef enum
{
  UDP_InterfaceAcceptError,
  UDP_InterfaceAddressError,
  UDP_InterfaceBindError

} UDP_InterfaceExceptionType;

/*! \class UDP_Interface
    \brief Interface to the raw sensed data provider.
 */

class UDP_Interface
{
  public:
    UDP_Interface(const char* port);
    ~UDP_Interface();
    int readFrom(byte* buffer, int bytes);
    int replyTo(byte* buffer, int bytes);

  private:
    int client_fd;
    struct sockaddr_in remoteAddress;
};

#endif
