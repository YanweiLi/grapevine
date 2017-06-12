# grapevine
grapevine 是一个C++库，包含线程，队列，消息中心，序列化与反序列化等等，后续还会加入更多的功能。其目的是为了摆脱很多程序员对于框架的束缚，我们是创造者，而不是框架下的搬砖的机器。

### 1.thread
之前一直使用boost::thread 。C++11 出来以后看了一下，缺少interrupt方法。因此一直在思考如何更好的使用C++11中的线程类。

1）通常线程函数是无出入参数，返回参数的，即void(void)类型的函数。但是我们现在的thread库可以赋予线程函数输入参数，返回参数的能力。

2）线程函数在运行的时候，boost::thread可以发送interrupt来打断，但是C++11中没有赋予这样的能力。因此我们在线程函数中传入 bool const& stop 这样的参数，赋予线程函数打断的能力。

``` C++
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


```


# 2.queue

# 3.RunLoop

# 4.xParam

# 5.Msgbus

# 6.Srlz



