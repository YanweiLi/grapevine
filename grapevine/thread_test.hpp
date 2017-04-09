//
//  ThreadTest.hpp
//  grapevine
//
//  Created by li_yanwei on 2017/4/9.
//  Copyright © 2017年 li_yanwei. All rights reserved.
//

#ifndef ThreadTest_hpp
#define ThreadTest_hpp
#include "thread.h"

class ClassThreadTest
{
public:
    int thread_func(int n , bool const& exit)
    {
        printf("thread start 1 ... \n");
        
        while (!exit) {
            printf("thread running 2... \n"); 
        }
        
        printf("thread stop 3 ... \n");
        return n*20;
    }
    
};


// 线程函数测试
void test_thread()
{
    ClassThreadTest test;
    typedef cpp0x::thread_0x<int, int> WorkThread;
    WorkThread work_thread;
    WorkThread::Functor1 func = std::bind(&ClassThreadTest::thread_func, &test , std::placeholders::_1 , std::placeholders::_2);
    work_thread.start(func , 10);
    work_thread.wait_for(200);
    work_thread.stop();
    int result = work_thread.get_result();
    printf("resutl = %d" , result);
}



#endif /* ThreadTest_hpp */
