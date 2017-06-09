//
//  main.cpp
//  grapevine
//
//  Created by li_yanwei on 2017/4/9.
//  Copyright © 2017年 li_yanwei. All rights reserved.
//

#include <iostream>
#include "grapevine/thread_test.hpp"
#include "grapevine/queue_test.hpp"
#include "grapevine/runloop_test.hpp"
#include "grapevine/msg_bus_test.h"

////////////////////////////////////////////////

int main(int argc, const char * argv[]) {
    
    // thread
    //std::cout << "test thread\n";
    //test_thread();
    
    // queue
    //std::cout << "test queue\n";
    //test_queue();
    
    // runloop
    //std::cout << "test runloop\n";
    //test_runloop();

    // msgbus
    std::cout << "test msgbus\n";
    test_msgbus_1();
    
    
    char x;
    std::cin >> x;
    return 0;
}
