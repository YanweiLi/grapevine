//
//  test_queue.h
//  grapevine
//
//  Created by li_yanwei on 2017/6/8.
//  Copyright © 2017年 li_yanwei. All rights reserved.
//

#ifndef test_queue_hpp
#define test_queue_hpp
#include "guid.h"
#include "queue/queue.h"

void test_queue()
{
    // 创建一个类型ThreadReturnValue ，数量为100个的队列
    cpp0x::queue<ThreadReturnValue> queReturn;
    queReturn.init(100);
    
    // 放进10个ThreadReturnValue对象 值为20的数据进队列
    ThreadReturnValue ret1;
    ret1.result_ = 20;
    
    for (int n = 0; n < 10; ++n) {
        
        queReturn.push(ret1);
    }
    
    // 放进10个ThreadReturnValue对象 值为10的数据进队列
    ThreadReturnValue ret2;
    ret2.result_ = 10;
    
    for (int n = 0; n < 10; ++n) {
        
        queReturn.push(ret2);
    }
    
    // 删除队列中值为11的数据
    ThreadReturnValue ret3;
    ret3.result_ = 11;
    
    size_t del1 = queReturn.delete_allof(ret3);
    printf("del1 delete %zu\n" , del1);
    
    // 删除队列中值为10的数据
    size_t del2 = queReturn.delete_allof([&ret2](ThreadReturnValue const& cont)
                           {
                               if (cont.result_ == 10) {
                                   return true;
                               }
                               
                               return false;
                           });
    
    printf("del2 delete %zu\n" , del2);

}

#endif /* test_queue_hpp */
