#ifndef SRC_MSGBUS_MODEL_MSGBUS_INTERFACE_H_
#define SRC_MSGBUS_MODEL_MSGBUS_INTERFACE_H_

#include "utility/xparam/include/xparam.h"

namespace msg
{
    struct MsgbusSender
    {
        virtual bool before_msg_process(tString const& msg_id
                                        , xparam::XParamPtr const& param
                                        , unsigned int filter_count
                                        , unsigned int receiver_count
                                        , bool filterd) = 0;
        
        virtual bool after_msg_process(tString const& msg_id
                                       , xparam::XParamPtr const& param
                                       , unsigned int filter_count
                                       , unsigned int receiver_count
                                       , bool filterd) = 0;
    };
    
    typedef std::weak_ptr<MsgbusSender> MsgbusSendWeakPtr;
    typedef std::shared_ptr<MsgbusSender> MsgbusSenderPtr;
    
    //////////////////////////////////////////////////////////////////////////
} // namespace msg

#endif  //SRC_MSGBUS_MODEL_MSGBUS_INTERFACE_H_
