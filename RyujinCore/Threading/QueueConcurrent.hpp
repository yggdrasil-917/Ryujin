#pragma once


#include "Thread.hpp"
#include "../Containers/List.hpp"


namespace Ryujin
{
    template<typename T>
    class QueueConcurrent
    {
    private:
        List<T> queue;
        Mutex mutex;
        ConditionVariable cv;
        
    public:
        QueueConcurrent()
        {
            
        }
        
        ~QueueConcurrent()
        {
            
        }
        
        void Add(T item)
        {
            ScopedLock lock(mutex);
            queue.Add(item);
            cv.UnblockOneThread();
        }
        
        T Remove()
        {
            ScopedLock lock(mutex);
            while (queue.Size() == 0)
            {
                cv.Wait(lock);
            }
            T item;
            if (queue.GetFirst(item))
            {
                queue.RemoveFront();
            }
            
            return item;
        }
        
        int32 Size()
        {
            ScopedLock lock(mutex);
            return queue.Size();
        }
    };
}
