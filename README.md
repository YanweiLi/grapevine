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

队列：用来存放数据的一个数据结构，FIFO。所有的操作都线程安全。

``` C++

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
```

# 3.RunLoop

实际是一个while循环，用来处理closure对象，把要做的task 丢进一个runloop。这个task 就会自动被执行。

```C++
    // 创建一个RUN LOOP
    inline tString CREATE_RUN_LOOP()
    {
        return RunLoopManager::instance().create_dispatch_loop();
    }
    
    inline bool CREATE_RUN_LOOP(tString const& loop_id)
    {
        return RunLoopManager::instance().create_dispatch_loop(loop_id);
    }
    
    // 销毁一个RUN LOOP
    inline bool DESTROY_RUN_LOOP(tString const& t)
    {
        return RunLoopManager::instance().destroy_dispatch_loop(t);
    }
    
    // 异步分发一个 task 给RUN LOOP
    inline bool ASYNC_RUN_LOOP_DISPATCH(tString const& t , LoopTaskType const& o)
    {
        return RunLoopManager::instance().push(t , o , false);
    }
    
    // 同步分发一个 task 给RUN LOOP
    inline bool SYNC_RUN_LOOP_DISPATCH(tString const& t , LoopTaskType const& o)
    {
        return RunLoopManager::instance().push(t , o , true);
    }


// 例子
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
```

# 4.xParam
xParam ： 由于C++没有统一的基类，因此传递参数的时候，需要传入很多的参数。想要做一个统一的接口变的比较困难。xParam 由此而生，把所有的参数都序列化成一个xParam中。这样接口就比较统一了。
例子见5 Msgbus

# 5.Msgbus
Msgbus: 一个观察者模型，类似于MFC 中的message map。提供消息的注册，及其响应。


```C++

//
//  msg_bus_test.h
//  grapevine
//
//  Created by li_yanwei on 2017/6/9.
//  Copyright © 2017年 li_yanwei. All rights reserved.
//

#ifndef test_msg_bus_h
#define test_msg_bus_h
#include "utility/msgbus/include/msgbus.h"

//////////////////////////////////////////////////////////////////////////
// msgbus œ˚œ¢Õ®÷™


#define TEST_MSGBUS_ID0 _T("test.msgbus.receiver.id0")
#define TEST_MSGBUS_ID1 _T("test.msgbus.receiver.id1")
#define TEST_MSGBUS_ID2 _T("test.msgbus.receiver.id2")
#define TEST_MSGBUS_ID3 _T("test.msgbus.receiver.id3")
#define TEST_MSGBUS_ID4 _T("test.msgbus.filter.id0")


template<class T>
class Observer_T
: public msg::MsgbusHandler
, public msg::MsgbusSender
{
    BEGIN_BUS_MSG_MAP(Observer_T)
    // 0 param
    REG_BUS_MSG_0(msg::eReceiver , false , false
                  , TEST_MSGBUS_ID0 , on_test_msgbus_id0)
    // 1 param
    REG_BUS_MSG_1(msg::eReceiver , false , false
                  , TEST_MSGBUS_ID1 , on_test_msgbus_id1)
    // 2 param
    REG_BUS_MSG_2(msg::eTopReceiver , false , false
                  , TEST_MSGBUS_ID2 , on_test_msgbus_id2)
    
    // filer
    REG_BUS_MSG_2(msg::eFilter , false , false
                  , TEST_MSGBUS_ID4 , on_test_msgbus_id2)
    
    END_BUS_MSG_MAP()
    
public:
    Observer_T(T t) : t_(t){}
    ~Observer_T(){}
    
    void on_test_msgbus_id0()
    {
        printf("0x%p : on_test_msgbus_id1 \r\n" , &t_);
    }
    
    void on_test_msgbus_id1(tString const& msg_id)
    {
        printf("0x%p : msg id = %s \r\n" , &t_ , msg_id.c_str());
    }
    
    void on_test_msgbus_id2(tString const& msg_id
                            , xparam::XParamPtr const& param)
    {
        printf("0x%p : msg id = %s \r\n" , &t_ , msg_id.c_str());
        
        std::wstring para_str;
        bool changed = XSL_INSTANCE.from_xparam(param , para_str);
        if (changed)
        {
            printf("0x%p : param = %ls \r\n" , &t_ , para_str.c_str());
        }
    }
    
    bool before_msg_process(tString const& msg_id
                            , xparam::XParamPtr const& param
                            , unsigned int filter_count
                            , unsigned int receiver_count
                            , bool filterd)
    {
        return true;
    }
    
    bool after_msg_process(tString const& msg_id
                           , xparam::XParamPtr const& param
                           , unsigned int filter_count
                           , unsigned int receiver_count
                           , bool filterd)
    {
        return true;
    }
    
private:
    T t_;
};

typedef Observer_T<int> ObserverInt;
typedef Observer_T<double> ObserverDouble;

//////////////////////////////////////////////////////////////////////////

void test_msgbus_1()
{
    msg::init_msgbus();
    
    msg::MsgbusPtr<ObserverInt> obs_int(new ObserverInt(1));
    msg::MsgbusPtr<ObserverDouble> obs_double(new ObserverDouble(2.0));
    
    //////////////////////////////////////////////////////////////////////////
    // send message
    {
        // 0 param
        bool handled0 = msg::instance().send_msg(TEST_MSGBUS_ID0);
        printf("handled0 = %s\n" , handled0 ? "yes" : "no");
        
        // 1 param
        bool handled1 = msg::instance().send_msg(TEST_MSGBUS_ID1);
        printf("handled1 = %s\n" , handled1 ? "yes" : "no");
        
        // 2 param
        tString cmd(_T("this is test param"));
        xparam::XParamPtr xpa = XSL_INSTANCE.to_xparam(_T("msgbus") , cmd);
        bool handled2 = msg::instance().send_msg(TEST_MSGBUS_ID2 , xpa);
        printf("handled2 = %s\n" , handled2 ? "yes" : "no");
    }
    
    printf(" ------------------------- \r\n");
    
    ////////////////////////////////////////////////////////////////////////////
    // post message
    
    {
        // 0 param
        msg::instance().post_msg(TEST_MSGBUS_ID0);
        
        // 1 param
        msg::instance().post_msg(TEST_MSGBUS_ID1 , obs_int.weak_this());
        
        // 2 param
        tString cmd(_T("this is test param"));
        xparam::XParamPtr xpa = XSL_INSTANCE.to_xparam(_T("msgbus") , cmd);
        msg::instance().post_msg(TEST_MSGBUS_ID2 , xpa , obs_int.weak_this());
        
        // 3 param
    }
    
    //////////////////////////////////////////////////////////////////////////
    // filter
    // 2 param
    tString cmd(_T("this is test param"));
    xparam::XParamPtr xpa = XSL_INSTANCE.to_xparam(_T("msgbus") , cmd);
    msg::instance().post_msg(TEST_MSGBUS_ID4 , xpa , obs_int.weak_this());
    
    char x;
    std::cin >> x;
}

#endif /* test_msg_bus_h */

```




