#include "utility/msgbus/model/msgbus_instance.h"
#include "utility/msgbus/model/msgbus_container.h"

namespace msg
{
    msg::MsgbusContainer& instance()
    {
        return msg::MsgbusContainer::instance();
    }
    
    bool init_msgbus()
    {
        return instance().start();
    }
    
    bool uninit_msgbus()
    {
        bool stoped = instance().stop();
        return stoped;
    }
} // namespace msg
