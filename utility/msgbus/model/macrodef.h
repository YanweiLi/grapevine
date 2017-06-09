#ifndef SRC_MSGBUS_MODEL_MARCODEF_H_
#define SRC_MSGBUS_MODEL_MARCODEF_H_

//////////////////////////////////////////////////////////////////////////
#define MSGBUS_EXPORTS

namespace msg
{
    //////////////////////////////////////////////////////////////////////////

#define BUSMSG_FUNC0(func)   static_cast<msg::MSGBUSPROC_0>(func)
#define BUSMSG_FUNC1(func)   static_cast<msg::MSGBUSPROC_1>(func)
#define BUSMSG_FUNC2(func)   static_cast<msg::MSGBUSPROC_2>(func)
#define BUSMSG_FUNC3(func)   static_cast<msg::MSGBUSPROC_3>(func)
#define BUSMSG_FUNC4(func)   static_cast<msg::MSGBUSPROC_4>(func)

    //////////////////////////////////////////////////////////////////////////

#define REG_BUS_MSG_0(handler_type, bUI, bSync, msid, fun)                 \
    add_reg_msg(msid,                                                      \
    BUSMSG_FUNC0(&__BUS_MSG_FUNCTOR_CLASS::fun), handler_type, bUI, bSync);

#define REG_BUS_MSG_1(handler_type, bUI, bSync, msid, fun)                 \
    add_reg_msg(msid,                                                      \
    BUSMSG_FUNC1(&__BUS_MSG_FUNCTOR_CLASS::fun), handler_type, bUI, bSync);

#define REG_BUS_MSG_2(handler_type, bUI, bSync, msid, fun)                 \
    add_reg_msg(msid,                                                      \
    BUSMSG_FUNC2(&__BUS_MSG_FUNCTOR_CLASS::fun), handler_type, bUI, bSync);

#define REG_BUS_MSG_3(handler_type, bUI, bSync, msid, fun)                 \
    add_reg_msg(msid,                                                      \
    BUSMSG_FUNC3(&__BUS_MSG_FUNCTOR_CLASS::fun), handler_type, bUI, bSync);

#define REG_BUS_MSG_4(handler_type, bUI, bSync, msid, fun)                 \
    add_reg_msg(msid,                                                      \
    BUSMSG_FUNC4(&__BUS_MSG_FUNCTOR_CLASS::fun), handler_type, bUI, bSync);

    //////////////////////////////////////////////////////////////////////////

#define BEGIN_BUS_MSG_MAP(__class)                                         \
    typedef __class __BUS_MSG_FUNCTOR_CLASS;                               \
public: void __RegistAll_In_Map() {

#define END_BUS_MSG_MAP()         }

    //////////////////////////////////////////////////////////////////////////

#define IF_FAIL_RETURN_FALSE(value)                                        \
    do                                                                     \
    {                                                                      \
    if (!(value))                                                          \
    {                                                                      \
    return false;                                                          \
}                                                                          \
} while (0)

    //////////////////////////////////////////////////////////////////////////
} // namespace msg

#endif // SRC_MSGBUS_MODEL_MARCODEF_H_
