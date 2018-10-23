#include "EnhancedRingBuffer.h"
#include <iostream>
#include <thread>

using namespace std;

void consumer(weak_ptr<EnhancedRingBuffer<int, 5 > > wpRbuff) 
{
    while(true)
    {
        shared_ptr<EnhancedRingBuffer<int, 5> > spRbuff = wpRbuff.lock();
        if(spRbuff)
        {
            int getNum;
            spRbuff->pop(getNum);
            cout << "num is " << getNum << endl;
        }
        else
        {
            cout << "life over!" << endl;
        }
    }
}

void producer(shared_ptr<EnhancedRingBuffer<int, 5> > spRbuff)
{
  //  spRbuff.reset(new EnhancedRingBuffer<int, 5>);
    int inNum = 0;
    while(inNum != 9)
    {
        cin >> inNum;
        spRbuff->put(inNum);
    }
    spRbuff.reset();
}


int main() 
{
  shared_ptr<EnhancedRingBuffer<int, 5> > spRbuff(new EnhancedRingBuffer<int, 5>());
  std::thread first (consumer,spRbuff);     // spawn new thread that calls foo()
  std::thread second (producer,spRbuff);  // spawn new thread that calls bar(0)

  std::cout << "main, foo and bar now execute concurrently...\n";

  // synchronize threads:
   first.join();                // pauses until first finishes
     second.join();               // pauses until second finishes

       std::cout << "foo and bar completed.\n";
         return 0;
         }
