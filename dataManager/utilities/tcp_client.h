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

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*! \file tcp_client.h
    \brief Provides a wrapper for a TCP Client instance
*/

typedef enum
{
  TCP_ClientSocketFailure,
  TCP_ClientHostNameFailure,
  TCP_ClientConnectFailure,
} TCP_ClientExceptionsType;

/*! \class TCP_Client
    \brief Class to provide TCP connection to a server.
    
    This class provides a simplified interface to a TCP Client socket. The 
    constructor sets up the connection to the server. Exceptions will be thrown
    from the constructor if the connection fails.
    \li TCP_ClientSocketFailure - Thrown if the socket cannot be created.
    \li TCP_ClientHostNameFailure - Thrown if the provided host does not resolve.
    \li TCP_ClientConnectFailure - Thrown if the connection to the server cannot be created.
    
 */

class TCP_Client
{
public:
  
  /*! \fn TCP_Client
      \brief Constructor for the TCP_Client class.
      \param host to form connection with.
      \param port to form connection on.
      \param socketTimeout the read call will only block for the specified duration. Default = 0 (waitforever) Unit = Seconds

   */
  TCP_Client(const char* host,   
             const char* port,   
             int socketTimeout); 
  ~TCP_Client();
  int sendData(const char* buffer, int bytes);
  int readData(char* buffer, int bytes);
private:
  int socket_fd;

};

#endif
