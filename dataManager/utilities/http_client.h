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

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>

using namespace std;

#include "tcp_client.h"

/*! \file http_client.h
    \brief Provides an interface to basic HTTP message structures.
*/

/*! \enum HTTP_Send_Return_Type
    \brief Indicates the data send status.
 */
typedef enum
{
    HTTP_Send_Success = 0,
    HTTP_Send_Fail
} HTTP_Send_Return_Type;

/*! \enum HTTP_Return_Type
    \brief Defines the possible HTTP_Response_Codes
 */
typedef enum
{
    HTTP_Continue                     = 100,
    HTTP_Switching_Protocols          = 101,
    HTTP_Processing                   = 102,
    HTTP_OK                           = 200,
    HTTP_Created                      = 201,
    HTTP_Accepted                     = 202,
    HTTP_NonAuthoritative_Information = 203,
    HTTP_No_Content                   = 204,
    HTTP_Reset_Content                = 205,
    HTTP_Partial_Content              = 206,
    HTTP_MultiStatus                  = 207,
    HTTP_Already_Reported             = 208,
    HTTP_IM_Used                      = 226,
    HTTP_Multiple_Choices             = 300,
    HTTP_Moved_Permanently            = 301,
    HTTP_Found                        = 302,
    HTTP_See_Other                    = 303,
    HTTP_Not_Modified                 = 304,
    HTTP_Use_Proxy                    = 305,
    HTTP_Switch_Proxy                 = 306,
    HTTP_Temporary_Redirect           = 307,
    HTTP_Permanent_Redirect           = 308,
    HTTP_Bad_Request                  = 400,
    HTTP_Service_Unavailable          = 503
    //TODO: Finish this enumeration...
} HTTP_Return_Type;

class HTTP_Client: public TCP_Client
{
  public:
    HTTP_Client(string server, string port, int timeout);
   
    HTTP_Send_Return_Type sendMessage(string& message);
    string                getResponse();
    int                   getResponseCode();

  private:
    int    responseCode;
    string response;
};

#endif
