//
//  main.cpp
//  grapevine
//
//  Created by li_yanwei on 2017/4/9.
//  Copyright © 2017年 li_yanwei. All rights reserved.
//

#include <iostream>
#include "thread_test.hpp"

// 线程函数测试
void test_thread()
{
    ClassThreadTest test;
    test.thread_start();
    test.thread_stop();
}

////////////////////////////////////////////////

int main(int argc, const char * argv[]) {
    
    std::cout << "test thread\n";
    test_thread();
    
    char x;
    std::cin >> x;
    return 0;
}
