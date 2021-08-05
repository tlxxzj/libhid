//
//  ThreadSafeQueue.hpp
//  test2
//
//  Created by Lang Tian on 2021/8/3.
//

#ifndef ThreadSafeQueue_hpp
#define ThreadSafeQueue_hpp

#include <queue>
#include <mutex>

template <typename T>
class ThreadSafeQueue
{
protected:
    std::mutex mutex_;
    std::queue<T>queue_;
    
public:
    ThreadSafeQueue() = default;
    
    bool empty()
    {
        return queue_.empty();
    }
    
    size_t size()
    {
        return queue_.size();
    }
    
    void push(const T & value)
    {
        queue_.push(value);
    }
    
    void push(T && value)
    {
        queue_.push(std::move(value));
    }
    
    T pop()
    {
        
    }
    
};

#endif /* ThreadSafeQueue_hpp */
