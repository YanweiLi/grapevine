//
//  runloop_test.h
//  grapevine
//
//  Created by li_yanwei on 2017/6/8.
//  Copyright © 2017年 li_yanwei. All rights reserved.
//

#ifndef runloop_test_hpp
#define runloop_test_hpp
#include "thread/run_loop.h"

void test_runloop()
{
    tString loop1 = cpp0x::CREATE_RUN_LOOP();
    cpp0x::RUN_LOOP_DISPATCH_ASYNC(loop1 , []{
        printf("heheh \n");
    });
    
    cpp0x::RUN_LOOP_DISPATCH(loop1 , [] {
        printf("sss \n");
    });
    
    cpp0x::DESTROY_RUN_LOOP(loop1);
}

#endif /* runloop_test_hpp */
