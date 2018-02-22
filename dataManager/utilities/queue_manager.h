/* Derek Schacht
 *  2017 09 22
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and cite code that I find on the internet but I am not 
 *               perfect. If you find something that should be cited let me know
 *               and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right 
 *  to modify this in the future.
 */

#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include <queue>
#include <mutex>
#include <vector>
#include <iostream>

#include "data_queue.h"

template <class DataType> 
class Queue_Manager
{
public:
  void publish(DataType data, int value=0)
  {
    for (int i = 0; i < queueList.size(); i++)
    {
      queueList[i]->pushTail(data);
    }
  }
  
  std::string status()
  {
    int size = -1;
    std::string counts = "";
    for (int i = 0; i < queueList.size(); i++)
    {
      counts += to_string(queueList[i]->elementCount()) + ",";
    }
    return counts;
  }
  
  int connections()
  {
    return queueList.size();
  }
  
  void connect(Data_Queue<DataType>* newQueue)
  {
      queueList.push_back(newQueue);
  }

  void remove(Data_Queue<DataType>* queue)
  {
      // Do something in the future... for now this is not in the use 
      // case of the program.
  }

private:
  std::vector<Data_Queue<DataType>*> queueList;
};

#endif
