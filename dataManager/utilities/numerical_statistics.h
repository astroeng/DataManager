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

#ifndef NUMERICAL_STATISTICS_H
#define NUMERICAL_STATISTICS_H

/*! \file numerical_statistics.h
    \brief Provides a templated class that can be used to collect basic statistics on a real time data set.
*/

/*! \class Numerical_Statistics
    \brief Class for statistical reporting on the recorded data.
    
 */
template <class DataType>
class Numerical_Statistics
{
public:
	Numerical_Statistics();
	~Numerical_Statistics();
	void reset();
	void includeValue(DataType value);
	DataType getMean();
	DataType getStdev();
	DataType getMax();
	DataType getMin();
	DataType getSetSize();
	
	bool isNewMax();
	bool isNewMin();
	
	void storeValue(DataType value);
	DataType getStoredValue();

private:
	DataType maxValue;
	DataType minValue;
	DataType meanValue;
	DataType valueStDev;
	long long valueCount;

	DataType storedValue;
	
  bool newMax;
	bool newMin;

};

#endif
