//
//  ThreadTest.hpp
//  grapevine
//
//  Created by li_yanwei on 2017/4/9.
//  Copyright © 2017年 li_yanwei. All rights reserved.
//

#ifndef ThreadTest_hpp
#define ThreadTest_hpp
#include "thread/thread.h"

class ThreadReturnValue
{
public:
    int result_;
};

bool operator == (ThreadReturnValue const& l , ThreadReturnValue const& r)
{
    return l.result_ == r.result_;
}

class ThreadInputValue
{
public:
    int input_;
};

class ClassThreadTest
{
public:
    ThreadReturnValue thread_func(ThreadInputValue const& input , bool const& exit)
    {
        printf("thread start 1 ... \n");
        
        while (!exit) {
            printf("thread running 2... \n"); 
        }
        
        printf("thread stop 3 ... \n");
        
        ThreadReturnValue ret;
        ret.result_ = input.input_*10;
        return ret;
    }
    
    ThreadReturnValue thread_func0(bool const& exit)
    {
        printf("thread start 1 ... \n");
        
        while (!exit) {
            printf("thread running 2... \n");
        }
        
        printf("thread stop 3 ... \n");
        
        ThreadReturnValue ret;
        ret.result_ = 10;
        return ret;
    }
};


// 线程函数测试
void test_thread()
{
    ClassThreadTest test;
    typedef cpp0x::thread_0x<ThreadReturnValue, ThreadInputValue> WorkThread;
    WorkThread work_thread;
    WorkThread::Functor1 func1 = std::bind(&ClassThreadTest::thread_func
                                          , &test
                                          , std::placeholders::_1
                                          , std::placeholders::_2);
    
    
    WorkThread::Functor0 func0 = std::bind(&ClassThreadTest::thread_func0
                                          , &test
                                          , std::placeholders::_1);
    
    ThreadInputValue input;
    input.input_ = 10;
    work_thread.start(func1 , input);
    work_thread.wait_for(200);
    work_thread.stop();
    ThreadReturnValue result = work_thread.get_result();
    printf("resutl = %d" , result.result_);
}


/////////////////////////////////////////////////////////////////////////////////////



#endif /* ThreadTest_hpp */
