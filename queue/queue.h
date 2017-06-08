//
//  queue.h
//  grapevine
//
//  Created by li_yanwei on 2016/10/26.
//  Copyright © 2016年 li_yanwei. All rights reserved.
//

#ifndef CPP_0X_QUEUE_H_
#define CPP_0X_QUEUE_H_
#include <list>
#include <mutex>
#include <condition_variable>

//////////////////////////////////////////////////////////////////////////
#define QUEUE_DATA_LOCKER(lock)                                          \
std::unique_lock<std::mutex> lock (mutex_)
//////////////////////////////////////////////////////////////////////////

namespace cpp0x
{
    template <class T>
    class queue
    {
    public:
        queue<T>()
        : exitwait_(false)
        , max_size_(0)
        {
            
        }
        
        ~queue<T>()
        {
            clear();
        }
        
    public:
        void init(int msg_cmt = 0)
        {
            QUEUE_DATA_LOCKER(lock);
            exitwait_ = false;
            max_size_  = msg_cmt;
            list_.clear();
        }
        
        void uninit()
        {
            QUEUE_DATA_LOCKER(lock);
            list_.clear();
            exitwait_ = true;
            cv_.notify_all();
        }
        
    public:
        void push(const T& x)
        {
            QUEUE_DATA_LOCKER(lock);
            if (max_size_ != 0
                &&  size() >= max_size_)
            {
                list_.clear();
            }
            
            list_.push_back(x);
            cv_.notify_one();
        }
        
        T pop()
        {
            QUEUE_DATA_LOCKER(lock);
            
            while (!shipment_available())
            {
                cv_.wait(lock , std::bind(&queue<T>::shipment_available , this));
            }
            
            T tmp;
            
            if (exitwait_)
            {
                exitwait_ = false;
                return tmp;
            }
            
            tmp = list_.front();
            list_.pop_front();
            return std::move(tmp);
        }
        
        T pop_back()
        {
            QUEUE_DATA_LOCKER(lock);
            
            while (!shipment_available())
            {
                cv_.wait(lock , std::bind(&queue<T>::shipment_available , this));
            }
            
            T tmp;
            if (exitwait_)
            {
                exitwait_ = false;
                return tmp;
            }
            
            tmp = list_.back();
            list_.pop_back();
            return std::move(tmp);
        }
        
        T at(size_t index)
        {
            QUEUE_DATA_LOCKER(lock);
            if(index < size() && index >= 0)
            {
                auto it = list_.begin();
                std::advance(it , index);
                return std::move(*it);
            }
            
            return std::move(T());
        }
        
    public:
        void clear()
        {
            QUEUE_DATA_LOCKER(lock);
            list_.clear();
        }
        
        size_t size()
        {
            return list_.size();
        }
        
        size_t delete_allof(const T& d)
        {
            QUEUE_DATA_LOCKER(lock);
            
            size_t before = size();
            if (before > 0) {
                list_.erase(std::remove(list_.begin()
                                        , list_.end()
                                        , d)
                            , list_.end());
            }
            
            return before - size();
        }
        
        template <class Functor>
        size_t delete_allof(Functor pred)
        {
            QUEUE_DATA_LOCKER(lock);
            
            size_t before = size();
            if (before > 0) {
                list_.erase(std::remove_if(list_.begin()
                                           , list_.end()
                                           , pred)
                            , list_.end());
                
            }
            
            return before - size();
        }
        
        
        bool exist(T& e)
        {
            for(T const& t : list_)
            {
                if (e == t)
                {
                    return true;
                }
            }
            
            return false;
        }
        
    private:
        bool shipment_available()
        {
            return size() != 0 || exitwait_;
        }
        
    protected:
        bool                        exitwait_;
        std::list<T>                list_;
        std::condition_variable     cv_;
        size_t                      max_size_;
        std::mutex                  mutex_;
    };
}



#endif /* CPP_0X_QUEUE_H_ */
