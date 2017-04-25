//
//  main.cpp
//  grapevine
//
//  Created by li_yanwei on 2017/4/9.
//  Copyright © 2017年 li_yanwei. All rights reserved.
//

#include <iostream>
#include "thread_test.hpp"
#include "guid.h"
#include "queue/queue.h"

////////////////////////////////////////////////

int main(int argc, const char * argv[]) {
    
    // thread
    std::cout << "test thread\n";
    test_thread();
    
    // uuid
    tString uuid = CGUID::create_uuid();
    
    // queue
    cpp0x::queue<ThreadReturnValue> queReturn;
    queReturn.init(100);
    
    ThreadReturnValue ret1;
    ret1.result_ = 20;
    
    for (int n = 0; n < 10; ++n) {
        
        queReturn.push(ret1);
    }
    
    ThreadReturnValue ret2;
    ret2.result_ = 10;
    
    for (int n = 0; n < 10; ++n) {
        
        queReturn.push(ret2);
    }
    
    queReturn.delete_allof(ret2);
    queReturn.delete_allof([&ret2](ThreadReturnValue const& cont)
    {
        if (ret2.result_ >= 20 && cont.result_ < 10) {
            return true;
        }
        
        return false;
    });
    
    char x;
    std::cin >> x;
    return 0;
}
