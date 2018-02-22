#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "property_reader.h"

const string propertyStrings[PROPERTY__COUNT] =
{
  "PROPERTY__Weather_Station_ID",
  "PROPERTY__Weather_Station_Altitude",
  "PROPERTY__Status_Upload_Interval",
  "PROPERTY__Weather_Upload_Interval",
  "PROPERTY__UDP_Receive_Port",
  "PROPERTY__Multicast_Port",
  "PROPERTY__Multicast_Remote_Address",
  "PROPERTY__Multicast_Bind_Address",
  "PROPERTY__TCP_Retry_Limit",
  "PROPERTY__TCP_Retry_Interval",
  "PROPERTY__TCP_Timeout"
};


Property_Reader::Property_Reader (string propertyFileName)
{
  ifstream propertiesFile (propertyFileName);
  string currentLine;
  bool found = false;

  while (getline(propertiesFile, currentLine))
  {
    for (int i = 0; i < PROPERTY__COUNT; i++)
    {
      if (currentLine.find(propertyStrings[i]) == 0)
      {
        cout << "FOUND: " << currentLine.substr(0,currentLine.find(":")) << ":";

        globalProperties[i] = currentLine.substr(currentLine.find(":") + 1, 
                              currentLine.length() - currentLine.find(":") - 1);

        cout << globalProperties[i] << endl;
        found = true;
      }
    }
    if (!found)
    {
      cout << "UNKNOWN: " << currentLine << endl;
    }
    found = false;
  }
}

Property_Reader::~Property_Reader ()
{
  
}
    
int Property_Reader::getElement (Property_Type property, string* value)
{
  (*value) = globalProperties[property];
}

int Property_Reader::getElement (Property_Type property, double* value)
{
  (*value) = atof(globalProperties[property].c_str());
}

int Property_Reader::getElement (Property_Type property, long* value)
{
  (*value) = atol(globalProperties[property].c_str());
}

int Property_Reader::getElement (Property_Type property, long long* value)
{
  (*value) = atoll(globalProperties[property].c_str());
}

int Property_Reader::getElement (Property_Type property, int* value)
{
  (*value) = atoi(globalProperties[property].c_str());
}

int Property_Reader::getElement (Property_Type property, unsigned int* value)
{
  (*value) = atoi(globalProperties[property].c_str());
}
