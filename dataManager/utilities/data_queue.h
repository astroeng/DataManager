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

#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "system_utilities.h"

/*! \file data_queue.h
    \brief Provides an implementation of a templated blocking queue.
*/

template <class DataType>
class Data_Queue
{
public:
  
  int elementCount()
  {
    return queueList.size();
  }
  
  DataType peekHead()
  {
    std::unique_lock<std::mutex> lock(queueMutex);
    while(queueList.empty()) {block.wait(lock);}
    return queueList.front();
  }
  
  bool pushTail(DataType element)
  {
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      queueList.push(element);
    }
    block.notify_one();
    return true;
  }
  
  DataType popHead()
  {
    std::unique_lock<std::mutex> lock(queueMutex);
    while(queueList.empty()) {block.wait(lock);}
    DataType data = queueList.front();
    queueList.pop();
    return data;
  }

private:

  std::queue<DataType> queueList;
  std::mutex queueMutex;
  std::condition_variable block;

};

#endif
