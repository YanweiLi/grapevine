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
