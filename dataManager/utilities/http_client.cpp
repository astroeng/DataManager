/* Derek Schacht
 *  2016 10 25
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
#include <cstring>
#include <sstream>

using namespace std;

#include "system_utilities.h"
#include "system_message.h"

#include "http_client.h"

/*! \file http_client.cpp
    \brief Provides an interface to basic HTTP message structures.
*/

#define DEBUG(x) //x

HTTP_Client::HTTP_Client(string server, string port, int timeout=0) : TCP_Client(server.c_str(), port.c_str(), timeout)
{
  responseCode = 0;
  response = "";
}

HTTP_Send_Return_Type HTTP_Client::sendMessage(string &message)
{

  // Try and send the message to the connected server. The TCP_Client
  // class will throw exceptions if for some reason the socket cannot
  // be created, or the connection cannot be made. Just catch them 
  // and proceed.
  try 
  {
    char responseBuffer[1000];
    bzero (responseBuffer, 1000);

    sendData(message.c_str(), message.length());
    
    delay(1);
 
    readData(responseBuffer, 999);
    response = string(responseBuffer);
      
    if (getResponseCode() == HTTP_OK)
    {
      return HTTP_Send_Success;
    }

  }
  catch (...)
  { 
    // Report the error and then wait just a little bit before
    // allowing the loop to continue.
    outputError("HTTP Exception");
  }

  return HTTP_Send_Fail;
}


/* Function to get the server response.
 */
string HTTP_Client::getResponse()
{

  DEBUG(cout << "RESPONSE: " << response << endl);
  
  return string(response);
}


int HTTP_Client::getResponseCode()
{
  string version = "";
  string statusCode = "";

  stringstream responseStream;
  
  responseStream.str(response);
  string statusLine;
  getline(responseStream, statusLine);
  
  DEBUG(cout << "STATUS: " << statusLine << endl);
  
  if (statusLine.find("HTTP") == 0)
  {
    int space = statusLine.find(" ") + 1;
    int space2 = statusLine.find(" ", space);
    int slash = statusLine.find("/") + 1;
    
    version    = statusLine.substr(slash,space-slash);
    statusCode = statusLine.substr(space,space2-space);
    
  }
  
  DEBUG(cout << "VERSION: " << version << endl);
  DEBUG(cout << "STATUSCODE: " << statusCode << endl);
  
  if (statusCode.length() > 1)
  {
    stringstream convertStream;
  
    convertStream << statusCode;
    convertStream >> responseCode;
  }
  else
  {
    responseCode = 0;
  }

  return responseCode;
}

