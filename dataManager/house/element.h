#ifndef ELEMENT_H
#define ELEMENT_H

#include <time.h>
#include <cstring>
#include <iostream>

typedef enum
{
	DataKind__Integer64 = 0,
	DataKind__Integer32,
	DataKind__Unsigned64,
	DataKind__Unsigned32,
	DataKind__Double,
	DataKind__Float,
	DataKind__Other,
	DataKind__Count
} DataKind_Type;

static const char * DataKind_String[DataKind__Count+1] =
{
	"DataKind__Integer64",
	"DataKind__Integer32",
	"DataKind__Unsigned64",
	"DataKind__Unsigned32",
	"DataKind__Double",
	"DataKind__Float",
	"DataKind__Other",
	"DataKind__Invalid"
};

typedef enum
{
	ElementKind__Index = 0,

	ElementKind__Temperature,     // Degrees Centigrade
	ElementKind__Humidity,        // Relative %
	ElementKind__Pressure,        // Pascal
	ElementKind__Wind_Speed,      // Knots
	ElementKind__Wind_Direction,  // Degrees Relative to True North
	ElementKind__White_Light,     // lux
	ElementKind__IR_Light,		    // lux
	ElementKind__UV_Light,        // lux
	ElementKind__Rain_Fall,       // inches today
	ElementKind__Lightning,       // detections since last message
	ElementKind__Salinity,		    // ppm (parts per million)
	ElementKind__Depth,           // feet
	ElementKind__Soil_Moisture,   // number
	ElementKind__Snow_Fall,       // inches today
	ElementKind__Magnetic_North,  // Offset from True North
	
	ElementKind__Voltage,         // Volts AC or DC
	ElementKind__Current,         // Ohms
	ElementKind__Power,           // Watts
	ElementKind__Magnetic_Field,  // gauss
	
	ElementKind__Coordinates,     // Lat/Log Pair or X,Y pair
	ElementKind__Latitude,		    // degrees (negative South)
	ElementKind__Longitude,       // degrees (negative West)
	ElementKind__Altitude,        // feet above MSL
	ElementKind__Pitch,           // degrees (negative down)
	ElementKind__Roll,            // degrees (negative CCW)
	ElementKind__Heading,         // degrees (0 <= x < 360)
	ElementKind__Course,          // degrees (0 <= x < 360)
	ElementKind__Speed,           // Knots
	ElementKind__Position_X,      // feet
	ElementKind__Position_Y,      // feet
	ElementKind__Position_Z,      // feet
	ElementKind__Velocity_X,      // feet/second
	ElementKind__Velocity_Y,      // feet/second
	ElementKind__Velocity_Z,      // feet/second
	ElementKind__Acceleration_X,  // feet/second^2
	ElementKind__Acceleration_Y,  // feet/second^2
	ElementKind__Acceleration_Z,  // feet/second^2

	ElementKind__Time,			      // POSIX Time
	ElementKind__Start,           // Start an Event
	ElementKind__Stop,            // Stop an Event
	ElementKind__Pause_Resume,    // Pause or Resume an Event
	ElementKind__Duration,        // Duration since Event
	ElementKind__Counter,         // Incremental Counter
	ElementKind__Version,         // Version Information (MAJOR | MINOR)
	ElementKind__Begin_Log,       // Begin a log with description as filename
	ElementKind__End_Log,         // Stop the log
	ElementKind__State,           // State of a described item

	ElementKind__Custom_1,
	ElementKind__Custom_2,
	ElementKind__Custom_3,
	ElementKind__Custom_4,
	ElementKind__Custom_5,

	ElementKind__Count
} ElementKind_Type;

static const char * ElementKind_String[ElementKind__Count] =
{
	"ElementKind__Index",

	"ElementKind__Temperature",
	"ElementKind__Humidity",
	"ElementKind__Pressure",
	"ElementKind__Wind_Speed",
	"ElementKind__Wind_Direction",
	"ElementKind__White_Light",
	"ElementKind__IR_Light",
	"ElementKind__UV_Light",
	"ElementKind__Rain_Fall",
	"ElementKind__Lightning",
	"ElementKind__Salinity",
	"ElementKind__Depth",
	"ElementKind__Soil_Moisture",
	"ElementKind__Snow_Fall",
	"ElementKind__Magnetic_North",

	"ElementKind__Voltage",
	"ElementKind__Current",
	"ElementKind__Power",
	"ElementKind__Magnetic_Field",

	"ElementKind__Coordinates",
	"ElementKind__Latitude",
	"ElementKind__Longitude",
	"ElementKind__Altitude",
	"ElementKind__Pitch",
	"ElementKind__Roll",
	"ElementKind__Heading",
	"ElementKind__Course",
	"ElementKind__Speed",
	"ElementKind__Position_X",
	"ElementKind__Position_Y",
	"ElementKind__Position_Z",
	"ElementKind__Velocity_X",
	"ElementKind__Velocity_Y",
	"ElementKind__Velocity_Z",
	"ElementKind__Acceleration_X",
	"ElementKind__Acceleration_Y",
	"ElementKind__Acceleration_Z",

	"ElementKind__Time",
	"ElementKind__Start",
	"ElementKind__Stop",
	"ElementKind__Pause_Resume",
	"ElementKind__Duration",
	"ElementKind__Counter",
	"ElementKind__Version",
	"ElementKind__Begin_Log",
	"ElementKind__End_Log",
	"ElementKind__State",

	"ElementKind__Custom_1",
	"ElementKind__Custom_2",
	"ElementKind__Custom_3",
	"ElementKind__Custom_4",
	"ElementKind__Custom_5",
};

class Element_Type
{
public:
	Element_Type()
	{
		dataKind = DataKind__Count;
		valid = false;
		stationID = -1;
		elementID = ElementKind__Count;
		
		timespec currentTime;
		clock_gettime(CLOCK_REALTIME, &currentTime);
		seconds     = currentTime.tv_sec;
		nanoSeconds = currentTime.tv_nsec;

		memset(description,0,sizeof(description));

		unsigned64 = 0;

	}
  
  //Element_Type(const Element_Type& data)
	//{
		//dataKind = data.dataKind;
		//valid = data.valid;
		//stationID = data.stationID;
		//elementID = data.elementID;
		
		//seconds     = data.seconds;
		//nanoSeconds = data.nanoSeconds;

		//memcpy(description,data.description,sizeof(description));

		//unsigned64 = data.unsigned64;

	//}
  
	DataKind_Type dataKind :  8;
	bool valid             :  8;
	int stationID          :  8;
	int elementID          :  8;

	// Time that the data was generated.
	int nanoSeconds        : 32;
	long long int seconds  : 64;

	char description[40];

	union
	{
		long long int          signed64   : 64;
		long long unsigned int unsigned64 : 64;
		double                 double64;
		struct
		{
			long signed32_A : 32;
			long signed32_B : 32;
		};
		struct
		{
			unsigned long unsigned32_A : 32;
			unsigned long unsigned32_B : 32;
		};
		struct
		{
			float float32_A;
			float float32_B;
		};
	};

	void print()
	{
		std::cout << std::endl;
		if (dataKind >= 0 && dataKind < DataKind__Count) 
		{
			std::cout << "DataKind    " << DataKind_String[dataKind] << std::endl;
		}
		else
		{
			std::cout << "DataKind    " << "Unknown" << std::endl;
		}
		std::cout << "Valid       " << valid << std::endl;
		std::cout << "stationID   " << stationID << std::endl;

		if (elementID >= 0 && elementID < ElementKind__Count) 
		{
			std::cout << "elementID   " << ElementKind_String[elementID] << std::endl;
		}
		else
		{
			std::cout << "elementID   " << "Unknown" << std::endl;
		}

		std::cout << "nanoSeconds " << nanoSeconds << std::endl;
		std::cout << "seconds     " << seconds << std::endl;
		std::cout << "description " << description << std::endl;

		switch (dataKind)
		{
		case DataKind__Integer64:
		    std::cout << "signed64    " << signed64 << std::endl;
		    break;
		case DataKind__Unsigned64:
		    std::cout << "unsigned64  " << unsigned64 << std::endl;
		    break;
		case DataKind__Double:
		    std::cout << "double64    " << double64 << std::endl;
		    break;
		case DataKind__Other:
		    std::cout << "other       " << unsigned64 << std::endl;
		    break;
		}
	}
};

#endif
