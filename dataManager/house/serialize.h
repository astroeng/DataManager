
#include <cstring>

#include "build_date.h"
#include "element.h"

#include <iostream>

#define MAJOR_VERSION 0
#define MINOR_VERSION 1


class Serialize
{

public:

	Serialize(int elements)
	{
		size         = elements * sizeof(Element_Type);
		elementCount = elements;
		elementArray = new Element_Type[elements];

		nextFreeElement = 0;
		memset((unsigned char*)elementTable,-1,sizeof(elementTable));
	}

	Serialize(unsigned char* data, int dataSize)
	{
		size         = dataSize;
		elementCount = dataSize/sizeof(Element_Type);
		
		elementArray = new Element_Type[elementCount];

		memset((unsigned char*)elementTable, -1, sizeof(elementTable));
		memcpy((unsigned char*)elementArray, data, dataSize);

		nextFreeElement = 0;
		for (int i = 0; i < elementCount; i++)
		{
			if (elementTable[elementArray[i].elementID] == -1)
			{
				elementTable[elementArray[i].elementID] = nextFreeElement++;
			}
		}
	}

	Serialize(const Serialize& data)
	{

    elementArray = new Element_Type[data.elementCount];

    nextFreeElement = data.nextFreeElement;
    size            = data.size;
    elementCount    = data.elementCount;

    memcpy((unsigned char*)elementArray, (unsigned char*)data.elementArray, size);
    memcpy((unsigned char*)elementTable, (unsigned char*)&data.elementTable, sizeof(elementTable));

	}

	~Serialize()
	{
		delete elementArray;
	}

	Element_Type getElement(ElementKind_Type elementId)
	{
		if ((elementTable[elementId] < elementCount) && (elementTable[elementId] >= 0) && elementTable[elementId] >= 0)
		{
			return elementArray[elementTable[elementId]];
		}
		
		Element_Type returnDefault;
		return returnDefault;
	}

	void setElement(Element_Type element)
	{
		if (elementTable[element.elementID] == -1)
		{
			elementTable[element.elementID] = nextFreeElement++;
		}
		
    if ((elementTable[element.elementID] < elementCount) && (elementTable[element.elementID] >= 0))
		{
			elementArray[elementTable[element.elementID]] = element;
		}
	}

	int structureSize()
	{
		return size;
	}
	
	int elements()
	{
		return elementCount;
	}
	
	int returnBuffer(unsigned char* destination, int dataSize)
	{
		if (dataSize >= size)
		{
			memset(destination, 0, dataSize);
			memcpy(destination, (unsigned char*)elementArray, size);
		
			return size;
		}
		return 0;
	}

	void print()
	{
    
    std::cout << (void*)elementArray << " : ";
    
		for (int i = 0; i < ElementKind__Count; i++)
		{
			std::cout << elementTable[i] << ",";
		}
		std::cout << std::endl;
	}
	
private:

	Element_Type* elementArray;
	int size;
	int elementCount;
	int elementTable[ElementKind__Count];
	int nextFreeElement;

};
