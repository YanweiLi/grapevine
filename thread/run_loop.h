//
//  run_loop.h
//  grapevine
//
//  Created by li_yanwei on 2016/10/27.
//  Copyright © 2016年 li_yanwei. All rights reserved.
//

#ifndef CPP_0X_THREAD_RUN_LOOP_H_
#define CPP_0X_THREAD_RUN_LOOP_H_

/**
 新理解：实际上分发队列就是采用队列的方式，解决资源加锁的问题。
 这种解决方法在 google chrome 开源库中很常见
 
 因此 构建一个lamda表达式来解决枷锁问题，同步问题 转化成 异步问题。
 // 异步回调结果？？？？
 */

#include <functional>
#include <map>

#include "utility/guid.h"
#include "queue/queue.h"
#include "thread/thread.h"
#include "utility/singleton.h"
#include "utility/string_def.h"

namespace cpp0x {
    
    typedef cpp0x::thread_0x<void, void*> CppWorker;
    typedef std::function<void(void)> LoopTaskType;
    
    class RunLoop
    {
    public:
        RunLoop()
        {
            queue_.init();
            guid_ = CGUID::create_uuid();
        }
        
        ~RunLoop()
        {
            thread_.stop();
            queue_.uninit();
        }
        
    public:
        bool start()
        {
            CppWorker::Functor0 f = std::bind(&RunLoop::thread_func
                                              , this
                                              , std::placeholders::_1);
            return thread_.start(f);
            
        }
        
        bool stop()
        {
            return thread_.stop();
        }
        
        void push(LoopTaskType const& param)
        {
            queue_.push(param);
        }
        
        tString get_id()
        {
            return guid_;
        }
        
    private:
        void thread_func(bool const& stoped)
        {
            while(!stoped)
            {
                LoopTaskType p = queue_.pop();
                if (p)
                {
                    p();
                }
            }
        }
        
    private:
        CppWorker thread_;
        cpp0x::queue<LoopTaskType> queue_;
        tString guid_;
    };
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // 工程类 : 构建一个
    
    class RunLoopManager : public Singleton<RunLoopManager>
    {
        DECLARE_SINGLETON_CLASS(RunLoopManager);
        
        typedef std::shared_ptr<RunLoop>          RunLoopSPtr;
        typedef std::map<tString , RunLoopSPtr>   RunLoopObjs;
        
    public:
        tString create_dispatch_loop()
        {
            RunLoopSPtr obj = std::make_shared<RunLoop>();
            tString loop_id = obj->get_id();
            run_loop_objs_.insert(std::make_pair(loop_id, obj));
            
            obj->start();
            return loop_id;
        }
        
        bool destroy_dispatch_loop(tString const& loop_id)
        {
            auto size = run_loop_objs_.size();
            auto it = run_loop_objs_.erase(loop_id);
            return size == it + 1;
        }
        
        bool push_async(tString const& loop_id , LoopTaskType const& loop_obj)
        {
            auto it = run_loop_objs_.find(loop_id);
            if(it != run_loop_objs_.end())
            {
                RunLoopSPtr tmp = it->second;
                if(tmp)
                {
                    tmp->push(loop_obj);
                }
                
                return true;
            }
            
            return false;
        }
        
        bool push_sync(tString const& loop_id , LoopTaskType const& loop_obj)
        {
            auto it = run_loop_objs_.find(loop_id);
            if(it != run_loop_objs_.end())
            {
                RunLoopSPtr tmp = it->second;
                if(tmp)
                {
                    tmp->push(loop_obj);
                }
                
                return true;
            }
            
            return false;
        }
        
    private:
        RunLoopObjs run_loop_objs_;
    };
    
    //////////////////////////////////////////////////////////////////////////////////////////
    
    inline tString CREATE_RUN_LOOP()
    {
        return RunLoopManager::instance().create_dispatch_loop();
    }
    
    inline bool DESTROY_RUN_LOOP(tString const& t)
    {
        return RunLoopManager::instance().destroy_dispatch_loop(t);
    }
    
    inline bool RUN_LOOP_DISPATCH_ASYNC(tString const& t , LoopTaskType const& o)
    {
        return RunLoopManager::instance().push_async(t , o);
    }
    
    inline bool RUN_LOOP_DISPATCH(tString const& t , LoopTaskType const& o)
    {
        return RunLoopManager::instance().push_sync(t , o);
    }
}
#endif /* CPP_0X_THREAD_RUN_LOOP_H_ */
