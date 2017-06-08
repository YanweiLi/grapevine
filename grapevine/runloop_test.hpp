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
    
//    cpp0x::SYNC_RUN_LOOP_DISPATCH(loop1 , [] (bool const& stoped){
//        int n = 0;
//        while (!stoped) {
//            printf("%d \n" , ++n);
//        }
//    });

    cpp0x::ASYNC_RUN_LOOP_DISPATCH(loop1 , [] (bool const& stoped){
        int n = 0;
        while (!stoped) {
            printf("%d \n" , ++n);
        }
    });
    
    THREAD_SLEEP(1000*2);
    
    bool stoped = cpp0x::DESTROY_RUN_LOOP(loop1);
    printf("stoped ? %s\n" , stoped ? "yes" : "no");
}

#endif /* runloop_test_hpp */
