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

using namespace std;

#include "numerical_statistics.h"

/*! \file numerical_statistics.cpp
    \brief Provides a templated class that can be used to collect basic statistics on a real time data set.
*/

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

template <class DataType> Numerical_Statistics<DataType>::Numerical_Statistics()
{
    /* mean is not reset because it leads to a potentially large
     * jump that makes the stdev compuation less valid.
     */

    meanValue = 0;
    reset();
}

template <class DataType> Numerical_Statistics<DataType>::~Numerical_Statistics()
{

}

template <class DataType> void Numerical_Statistics<DataType>::reset()
{
    minValue = 100000000;
    maxValue = -100000000;
    valueStDev = 0;
    valueCount = 0;
}
	
template <class DataType> void Numerical_Statistics<DataType>::includeValue(DataType value)
{
    /* The computation of the mean and the standard deviation was influenced by
     * this stackexchange discussion.
     * http://dsp.stackexchange.com/questions/811/determining-the-mean-and-standard-deviation-in-real-time
     * which itself references
     * Knuth's "Art of Computer Programming" vol 2
     */
     
    /* Initially this file only used the integer representation values and was
     * used on an Arduino. With the reimplementation on a rasPi and the use
     * of float representations the processing could probably also use an update.
     */
    DataType previousMean = meanValue;

    valueCount++;

    meanValue += ((value - meanValue) / (DataType)valueCount);
    valueStDev += ((value - meanValue) * (value - previousMean));

    /* End of code infuenced by said discussion. */

    newMax = (value > maxValue);
    newMin = (value < minValue);

    maxValue = max(value, maxValue);
    minValue = min(value, minValue);

}
template <class DataType> DataType Numerical_Statistics<DataType>::getMean()
{
    return meanValue;
}
template <class DataType> DataType Numerical_Statistics<DataType>::getStdev()
{
    return valueStDev / max(valueCount,1);
}
template <class DataType> DataType Numerical_Statistics<DataType>::getMax()
{
    return maxValue;
}
template <class DataType> DataType Numerical_Statistics<DataType>::getMin()
{
    return minValue;
}
template <class DataType> DataType Numerical_Statistics<DataType>::getSetSize()
{
    return valueCount;
}
template <class DataType> bool Numerical_Statistics<DataType>::isNewMax()
{
    return newMax;
}
template <class DataType> bool Numerical_Statistics<DataType>::isNewMin()
{
    return newMin;
}

template <class DataType> void Numerical_Statistics<DataType>::storeValue(DataType value)
{
    storedValue = value;
    
    includeValue(value);
}

template <class DataType> DataType Numerical_Statistics<DataType>::getStoredValue()
{
    return storedValue;
}

/* Hints for the compiler */

template class Numerical_Statistics<long long>;
template class Numerical_Statistics<double>;

