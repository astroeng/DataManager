#ifndef PROPERTY_READER_H
#define PROPERTY_READER_H

#include <string>

typedef enum
{
  PROPERTY__Weather_Station_ID = 0,
  PROPERTY__Weather_Station_Altitude,
  PROPERTY__Status_Upload_Interval,
  PROPERTY__Weather_Upload_Interval,
  PROPERTY__UDP_Receive_Port,
  PROPERTY__Multicast_Port,
  PROPERTY__Multicast_Remote_Address,
  PROPERTY__Multicast_Bind_Address,
  PROPERTY__TCP_Retry_Limit,
  PROPERTY__TCP_Retry_Interval,
  PROPERTY__TCP_Timeout,
  PROPERTY__COUNT
} Property_Type;

class Property_Reader 
{
  public:
    Property_Reader (std::string propertyFileName);
    ~Property_Reader ();
    
    int getElement(Property_Type property, std::string* value);
    int getElement(Property_Type property, double* value);
    int getElement(Property_Type property, long* value);
    int getElement(Property_Type property, long long* value);
    int getElement(Property_Type property, int* value);
    int getElement(Property_Type property, unsigned int* value);

  private:

    std::string globalProperties[PROPERTY__COUNT];

};



#endif
