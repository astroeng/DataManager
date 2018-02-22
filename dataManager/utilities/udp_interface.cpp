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
#include <cstdio>

#include <unistd.h>

using namespace std;

#include <netdb.h>

#include "udp_interface.h"
#include "system_message.h"

/*! \file udp_interface.cpp
    \brief Provides an easy wrapper for a UDP receiver.
*/

#define DEBUG(x) //x

// This constructor establishes a UDP connection on a given port.

UDP_Interface::UDP_Interface(const char* port)
{
  struct addrinfo hostLookupHints = {};
  struct addrinfo *hostLookupResults;
  
  hostLookupHints.ai_family    = AF_INET;
  hostLookupHints.ai_socktype  = SOCK_DGRAM;
  hostLookupHints.ai_flags     = AI_PASSIVE;
  hostLookupHints.ai_protocol  = IPPROTO_UDP;
  hostLookupHints.ai_canonname = NULL;
  hostLookupHints.ai_addr      = NULL;
  hostLookupHints.ai_next      = NULL;
  
  if (getaddrinfo(NULL, port, &hostLookupHints, &hostLookupResults) != 0)
  {
    char error[128] = {};
    sprintf(error, "CLIENT_INTERFACE: getaddrinfo Failed %s:%s", "ANY", port);
    outputError(error);

    throw UDP_InterfaceAddressError;
  }
  
  // getaddrinfo returns a linked list of possible results. Loop through
  // them and attempt a connection, break on the first successful result

  client_fd = socket(hostLookupHints.ai_family, 
                     hostLookupHints.ai_socktype, 
                     hostLookupHints.ai_protocol);

  for (struct addrinfo *results_i = hostLookupResults; results_i != NULL; results_i = results_i->ai_next)
  {
    DEBUG(cout << "CI: Address " << ((struct sockaddr_in*)results_i->ai_addr)->sin_addr.s_addr << endl);
  
    if (bind(client_fd,results_i->ai_addr, results_i->ai_addrlen) == 0)
    {
      DEBUG(cout << "CI: Connected" << endl);
      // The addrinfo results are no longer needed.
      freeaddrinfo(hostLookupResults);
      return;
    }
  }

  // If the code reaches this point it means that bind did not succeed. Free
  // up any memory and report the issue.

  freeaddrinfo(hostLookupResults);
  outputError("CLIENT_INTERFACE: UDP Bind Failed");
  throw UDP_InterfaceBindError;

}

UDP_Interface::~UDP_Interface()
{
  close(client_fd);
}


int UDP_Interface::readFrom(byte* buffer, int bytes)
{
  return read(client_fd, buffer, bytes);
}


int UDP_Interface::replyTo(byte* buffer, int bytes)
{
  return write(client_fd, buffer, bytes);
}
