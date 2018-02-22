#ifndef MULTICAST_H
#define MULTICAST_H

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

#include "system_message.h"

typedef enum
{
  Multicast_Socket_Error,
  Multicast_Bind_Error,
  Multicast_Setup_Error,
  Multicast_Join_Error
} Multicast_InterfaceExceptionType;


class Multicast_Interface
{
public:
	Multicast_Interface(const char* multicastAddress, 
                      const int multicastPort, 
                      const char* sourceAddress, 
                      const int sourcePort)
	{
		// Begin by setting up a normal UDP socket.
		socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

		if (socket_fd < 0)
		{
			outputError("MULTICAST: Socket Create Failed");
      throw Multicast_Socket_Error;
		}
		
		// Bind the socket to the required port.
		//   Note: For this socket to receive traffic that it generates
		//         multicastPort and sourcePort must match.
		struct sockaddr_in bind_addr;
		bind_addr.sin_family      = AF_INET;
		bind_addr.sin_port        = htons(sourcePort);
		//bind_addr.sin_addr.s_addr = INADDR_ANY;
		bind_addr.sin_addr.s_addr = inet_addr(multicastAddress);

		int value = 0;
		
		value = ::bind(socket_fd, 
                   (struct sockaddr*)&bind_addr, 
                   sizeof(bind_addr));

		if (value < 0)
		{
			outputError("MULTICAST: Socket Bind Failed");
      throw Multicast_Bind_Error;
		}
		
		// This tells the OS that the local interface defined by 
		// sourceAddress will be used to receive multicast traffic.
		struct in_addr multicastInterface;
		multicastInterface.s_addr = inet_addr(sourceAddress);
		
		value = setsockopt(socket_fd, 
                       IPPROTO_IP, 
                       IP_MULTICAST_IF, 
                       (const void*)&multicastInterface, 
                       sizeof(multicastInterface));
		if (value < 0)
		{
			outputError("MULTICAST: Multicast Receive Setup Failed");
      throw Multicast_Setup_Error;
		}
		
		// This request that the OS perform a multicast group membership
		// join. A message will be generated and sent to devices on the
		// network.
		struct ip_mreq multicastGroup;
		multicastGroup.imr_interface.s_addr = inet_addr(sourceAddress);
		multicastGroup.imr_multiaddr.s_addr = inet_addr(multicastAddress);
		
		value = setsockopt(socket_fd, 
                       IPPROTO_IP, 
                       IP_ADD_MEMBERSHIP, 
                       (const void*)(&multicastGroup), 
                       sizeof(multicastGroup));

		if (value < 0)
		{
			outputError("MULTICAST: Multicast Join Failed");
      throw Multicast_Join_Error;
		}

		// Finally build the structure that contains the information
		// needed to send packets.
		remoteAddress.sin_family      = AF_INET;
		remoteAddress.sin_port        = htons(multicastPort);
		remoteAddress.sin_addr.s_addr = inet_addr(multicastAddress);

	}
	
	~Multicast_Interface()
	{
		shutdown (socket_fd, SHUT_RDWR);
		close    (socket_fd);
	}
	
  int sendTo(unsigned char* buffer, int bytes)
  {
		if (socket_fd > 0)
		{
			return sendto(socket_fd, 
                    buffer, 
                    bytes, 
                    0, 
                    (struct sockaddr*)&remoteAddress, 
                    sizeof (remoteAddress));
		}
		return -1;
	}

	int readFrom(unsigned char* buffer, int bytes)
	{
		if (socket_fd > 0)
		{
			return read(socket_fd, buffer, bytes);
		}
		return -1;
	}


private:
	int socket_fd;
	struct sockaddr_in remoteAddress;
	
};

#endif
