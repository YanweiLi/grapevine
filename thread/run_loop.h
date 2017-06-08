//
//  run_loop.h
//  grapevine
//
//  Created by li_yanwei on 2016/10/18.
//  Copyright © 2016年 li_yanwei. All rights reserved.
//

#ifndef SRC_UTILITY_THREAD_RUN_LOOP_H_
#define SRC_UTILITY_THREAD_RUN_LOOP_H_
#include <map>
#include "queue/queue.h"
#include "thread/thread.h"
#include "utility/guid.h"
#include "utility/singleton.h"

namespace cpp0x {
    //
    // 同步需要添加一些信息
    //
    typedef std::function<void(bool const& stoped)> LoopTaskType;
    
    class RunLoopSyncInfo
    {
    public:
        RunLoopSyncInfo(LoopTaskType const& task
                        , bool is_sync)
        {
            loop_task           = task;
            is_sync_runloop_    = is_sync;
            wait_here_          = true;
        }
        
        const LoopTaskType& get_task()
        {
            return loop_task;
        }
        
        bool wait_here()
        {
            if(is_sync_runloop_)
            {
                std::unique_lock<std::mutex> lock (mutex_);
                
                while (wait_here_) {
                    sync_.wait(lock);
                }
            }
            
            return is_sync_runloop_;
        }
        
        void wake_up()
        {
            if(is_sync_runloop_)
            {
                std::unique_lock<std::mutex> lock (mutex_);
                
                wait_here_ = false;
                sync_.notify_all();
            }
        }
        
    private:
        // 执行block
        LoopTaskType                loop_task;
        // 同步锁
        std::condition_variable     sync_;
        std::mutex                  mutex_;
        bool                        wait_here_;
        // 线程id
        std::string                 thread_id_;
        // 同步
        bool                        is_sync_runloop_;
    };
    
    //////////////////////////////////////////////////////////////////////////////////////////
    
    typedef std::shared_ptr<RunLoopSyncInfo>    RunLoopSyncInfoPtr;
    typedef cpp0x::queue<RunLoopSyncInfoPtr>    RunLoopQueue;
    typedef cpp0x::thread_0x<void, void*>       Worker;
    
    class RunLoop
    {
    public:
        RunLoop()
        {
            queue_.init();
            guid_ = CGUID::create_uuid();
        }
        
        RunLoop(tString const& loop_id)
        {
            queue_.init();
            guid_ = loop_id;
        }
        
        ~RunLoop()
        {
            thread_.stop();
            queue_.uninit();
        }
        
    public:
        bool start()
        {
            Worker::Functor0 f = std::bind(&RunLoop::thread_func
                                           , this
                                           , std::placeholders::_1);
            return thread_.start(f);
        }
        
        bool stop()
        {
            return thread_.stop();
        }
        
        void push(RunLoopSyncInfoPtr const& param)
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
                RunLoopSyncInfoPtr p = queue_.pop();
                if (p)
                {
                    LoopTaskType const& loop_task = p->get_task();
                    if(loop_task)
                    {
                        loop_task(stoped);
                    }
                    
                    p->wake_up();
                }
            }
        }
        
    private:
        Worker          thread_;
        tString         guid_;
        RunLoopQueue    queue_;
    };
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // 工程类 : 构建一个
    
    class RunLoopManager : public Singleton<RunLoopManager>
    {
        DECLARE_SINGLETON_CLASS(RunLoopManager);
        
        typedef std::shared_ptr<RunLoop>            RunLoopSPtr;
        typedef std::map<tString , RunLoopSPtr>     RunLoopObjs;
        
    public:
        tString create_dispatch_loop()
        {
            RunLoopSPtr obj = std::make_shared<RunLoop>();
            tString loop_id = obj->get_id();
            run_loop_objs_.insert(std::make_pair(loop_id, obj));
            
            return obj->start() ? loop_id : "";
        }
        
        bool create_dispatch_loop(tString const& loop_id)
        {
            if (run_loop_objs_.find(loop_id) != run_loop_objs_.end()) {
                RunLoopSPtr obj = std::make_shared<RunLoop>(loop_id);
                run_loop_objs_.insert(std::make_pair(loop_id, obj));
                obj->start();
                return true;
            }
            
            return false;
        }
        
        bool destroy_dispatch_loop(tString const& loop_id)
        {
            auto size = run_loop_objs_.size();
            auto it = run_loop_objs_.find(loop_id);
            if (it != run_loop_objs_.end()) {
                RunLoopSPtr ptr = it->second;
                if (ptr->stop())
                {
                    run_loop_objs_.erase(loop_id);
                }
            }
            
            return size == run_loop_objs_.size() + 1;
        }
        
        bool push(tString const& loop_id , LoopTaskType const& loop_obj , bool is_sync)
        {
            auto it = run_loop_objs_.find(loop_id);
            if(it != run_loop_objs_.end())
            {
                RunLoopSPtr tmp = it->second;
                if(tmp)
                {
                    RunLoopSyncInfoPtr task = std::make_shared<RunLoopSyncInfo>(loop_obj , is_sync);
                    tmp->push(task);
                    task->wait_here();
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
    
    inline bool CREATE_RUN_LOOP(tString const& loop_id)
    {
        return RunLoopManager::instance().create_dispatch_loop(loop_id);
    }
    
    inline bool DESTROY_RUN_LOOP(tString const& t)
    {
        return RunLoopManager::instance().destroy_dispatch_loop(t);
    }
    
    inline bool ASYNC_RUN_LOOP_DISPATCH(tString const& t , LoopTaskType const& o)
    {
        return RunLoopManager::instance().push(t , o , false);
    }
    
    inline bool SYNC_RUN_LOOP_DISPATCH(tString const& t , LoopTaskType const& o)
    {
        return RunLoopManager::instance().push(t , o , true);
    }
}
#endif /* SRC_UTILITY_THREAD_RUN_LOOP_H_ */
