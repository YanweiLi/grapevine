#ifndef SRC_MSGBUS_MODEL_MSGBUS_INSTANCE_H_
#define SRC_MSGBUS_MODEL_MSGBUS_INSTANCE_H_

#include "utility/msgbus/model/macrodef.h"
#include "utility/msgbus/model/msgbus_container.h"

namespace msg
{
    MSGBUS_EXPORTS bool init_msgbus();
    MSGBUS_EXPORTS bool uninit_msgbus();
    MSGBUS_EXPORTS MsgbusContainer& instance();
} // namespace msg

#endif  //SRC_MSGBUS_MODEL_MSGBUS_INSTANCE_H_
