//
//  thread_0x.h
//  grapevine
//
//  Created by li_yanwei on 2016/10/21.
//  Copyright © 2016年 li_yanwei. All rights reserved.
//

#ifndef CPP_0X_THREAD_H_
#define CPP_0X_THREAD_H_
#include "utility/noncopyable.h"
#include "queue/queue.h"
#include <thread>
#include <future>

namespace cpp0x
{
    template <class ReturnType , class InputType>
    class thread_0x : public cpp0x::noncopyable
    {
    public:
        typedef std::function<ReturnType(bool const&)>              Functor0;
        typedef std::function<ReturnType(InputType , bool const&)>  Functor1;
        
    public:
        
        thread_0x<ReturnType , InputType>()
        :exit_flag_(false)
        {
            
        }
        
        ~thread_0x<ReturnType , InputType>()
        {
#ifdef DEBUG
            bool stoped = stop();
            printf("delete obj = %p : stoped ? %s" , this , stoped ? "yes" : "no");
#else
            stop();
#endif
        }
        
    public:
        
        bool start(Functor0 const& f)
        {
            if(!is_running())
            {
                exit_flag_ = false;
                std::packaged_task<ReturnType(bool const&)> pt(f);
                future_ = pt.get_future();
                
                auto tmp = std::thread(std::move(pt), std::cref(exit_flag_));
                thread_.swap(tmp);
            }
            
            return is_running();
        }
        
        bool start(Functor1 const& f , InputType input)
        {
            if(!is_running())
            {
                exit_flag_ = false;
                std::packaged_task<ReturnType(InputType , bool const&)> pt(f);
                future_ = pt.get_future();
                
                auto tmp = std::thread(std::move(pt) , input , std::cref(exit_flag_));
                thread_.swap(tmp);
            }
            
            return is_running();
        }
        
        bool stop()
        {
            if(thread_.joinable())
            {
                exit_flag_ = true;
                thread_.join();
                //future_.get();
                return true;
            }
            
            return false;
        }
        
        template<class T>
        bool stop_with_queue_exit(cpp0x::queue<T>& queue)
        {
            if(thread_.joinable())
            {
                exit_flag_ = true;
                queue.uninit();
                thread_.join();
                //future_.get();
                return true;
            }
            
            return false;
        }
        
    public:
        
        bool is_running()
        {
            return future_.valid();
        }
        
        bool wait()
        {
            if(is_running())
            {
                future_.wait();
            }
            
            return true;
        }
        
        bool wait_for(int milliseconds)
        {
            if(is_running())
            {
                std::chrono::milliseconds t(milliseconds);
                auto status = future_.wait_for(t);
                return status == std::future_status::ready;
            }
            
            // 程序结束，返回等待成功
            return true;
        }
        
        ReturnType get_result()
        {
            return future_.get();
        }
        
    private:
        std::thread                 thread_;
        std::future<ReturnType>     future_;
        mutable bool                exit_flag_;
    };
    
#define THREAD_SLEEP(t)     std::this_thread::sleep_for(std::chrono::milliseconds(t));
#define THIS_THREAD_ID()    std::this_thread::get_id()
}
#endif /* CPP_0X_THREAD_H_ */
