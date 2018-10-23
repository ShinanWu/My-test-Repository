#include <atomic>
#include <pthread.h>

using namespace std;

template <class T, size_t size>
class EnhancedRingBuffer
{       
public:
    EnhancedRingBuffer();
    virtual ~EnhancedRingBuffer();
    bool tryPut(const T &value); //wait-free
    bool tryPop(T &value);
    bool put(const T &value);  //block-util-signal
    bool pop(T &value);
private:
    size_t nextPos(size_t curPos)
    {
        return (curPos + 1) % size;
    }
private:
    T __buf[size];
    pthread_mutex_t __mtx;
    pthread_cond_t __condNotFull, __condNotEmpty;
    atomic<size_t> __head, __tail;
};   

template<class T, size_t size>
EnhancedRingBuffer<T,size>::EnhancedRingBuffer():__head(0), __tail(0)
{
    pthread_mutex_init(&__mtx,NULL);
    pthread_cond_init(&__condNotEmpty,NULL);
    pthread_cond_init(&__condNotFull,NULL);
}
template<class T, size_t size>
EnhancedRingBuffer<T, size>::~EnhancedRingBuffer()
{
    pthread_mutex_destroy(&__mtx);
    pthread_cond_destroy(&__condNotEmpty);
    pthread_cond_destroy(&__condNotFull);
}
template<class T, size_t size>
bool EnhancedRingBuffer<T, size>::tryPut(const T &value)
{
    size_t head = __head.load(std::memory_order_relaxed);
    size_t next_head = nextPos(head);
    if (next_head == __tail.load(std::memory_order_acquire))
      return false;
    __buf[head] = value;
    __head.store(next_head, std::memory_order_release);
    return true;
}
template<class T, size_t size>
bool EnhancedRingBuffer<T, size>::tryPop(T &value)
{
    size_t tail = __tail.load(std::memory_order_relaxed);
    if (tail == __head.load(std::memory_order_acquire))
      return false;
    value = __buf[tail];
    __tail.store(nextPos(tail), std::memory_order_release);
    return true;
}
template<class T, size_t size>
bool EnhancedRingBuffer<T, size>::put(const T &value)
{
    while(!tryPut(value))
        pthread_cond_wait(&__condNotFull, &__mtx);
    pthread_cond_signal(&__condNotEmpty);
    return true;
}
template<class T, size_t size>
bool EnhancedRingBuffer<T, size>::pop(T &value)
{
    while(!tryPop(value))
        pthread_cond_wait(&__condNotEmpty, &__mtx);
    pthread_cond_signal(&__condNotFull);
    return true;
}

