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
#include <iostream>

class ClassThreadTest
{
public:
    int thread_func(int n)
    {
        std::cout << "thread_func \n";
        return n;
    }
    
    bool thread_start()
    {
        auto func = std::bind(&ClassThreadTest::thread_func, this , 10);
        return working_thread_.start(func);
    }
    
    bool thread_stop()
    {
        return working_thread_.stop();
    }
    
private:
    cpp0x::thread_0x<int, int> working_thread_;
};


#endif /* ThreadTest_hpp */
