#include "utility/msgbus/model/msgbus_base.h"
#include "utility/msgbus/model/msgbus_instance.h"
#include "utility/common/guid.h"

namespace msg
{
    MsgbusHandler::MsgbusHandler(bool need_lock_res /*= true*/)
    : need_lock_res_( need_lock_res )
    , guid_(CGUID::create_uuid())
    {
        
    }
    
    MsgbusHandler::~MsgbusHandler()
    {
        remove_all_msg();
    }
    
    size_t MsgbusHandler::size()
    {
        return msg_handler_.size();
    }
    
    bool MsgbusHandler::on_message(tString const& reg_msg_id
                                   , tString const& msg_id
                                   , XParamPtr const& param
                                   , MsgbusSendWeakPtr const& sender
                                   , bool* filtered)
    {
        bool handled = false;
        MessageSinkEntry_t entry;
        
        bool finded = false;
        {
            LockGuard locker(reg_locker);
            auto it = msg_handler_.find(reg_msg_id);
            if (it != msg_handler_.end())
            {
                entry = (it->second);
                finded = true;
            }
        }
        
        if (finded)
        {
            if (need_lock_res_)
            {
                LockGuard locker(res_locker_);
                handled = entry.invoke(_Base::shared_from_this()
                                       , msg_id
                                       , param
                                       , sender
                                       , filtered);
            }
            else
            {
                handled = entry.invoke(_Base::shared_from_this()
                                       , msg_id
                                       , param
                                       , sender
                                       , filtered);
            }
        }
        
        return handled;
    }
    
    bool MsgbusHandler::add_reg_msg(tString const& msg_id
                                    , HandlerEntryType type /*= eReceiver*/
                                    , bool is_ui_thread /*= true*/
                                    , bool is_sync_ui_thread /*= true */)
    {
        LockGuard guader(reg_locker);
        switch(type)
        {
                // 如果使用UIThread，
                // 则Filter必须是同步，否则起不到Filter作用。
            case eTopFilter:
            {
                IF_FAIL_RETURN_FALSE(instance().reg_top_filter(msg_id
                                                               , _Base::shared_from_this()
                                                               , is_ui_thread
                                                               , true));
                break;
            }
                
            case eFilter:
            {
                IF_FAIL_RETURN_FALSE(instance().reg_filter(msg_id
                                                           , _Base::shared_from_this()
                                                           , is_ui_thread
                                                           , true));
                break;
            }
                
                // 如果使用UIThread，
                // 则Filter必须是同步，否则起不到Filter的左右。
            case eReceiver:
            {
                IF_FAIL_RETURN_FALSE(instance().reg_receiver(msg_id
                                                             , _Base::shared_from_this()
                                                             , is_ui_thread
                                                             , is_sync_ui_thread));
                break;
            }
                
            case eTopReceiver:
            {
                IF_FAIL_RETURN_FALSE(instance().reg_top_receiver(msg_id
                                                                 , _Base::shared_from_this()
                                                                 , is_ui_thread
                                                                 , is_sync_ui_thread));
                break;
            }
                
            default: return false;
        }
        
        return true;
    }
    
    bool MsgbusHandler::add_reg_msg(tString const& msg_id
                                    , MSGBUSPROC_0 pfun
                                    , HandlerEntryType type /*= Receiver*/
                                    , bool is_ui_thread /*= true*/
                                    , bool is_aync_ui_thread /*= true*/ )
    {
        bool added = add_reg_msg(msg_id
                                 , type
                                 , is_ui_thread
                                 , is_aync_ui_thread);
        
        if(added)
        {
            MessageSinkEntry_t entry;
            entry.msg_id_ = msg_id;
            entry.functor_.MsgProc_0 = pfun;
            entry.msgbus_proc_type_ = eMsgBusProcType0;
            msg_handler_[entry.msg_id_] = entry;
        }
        
        return added;
    }
    
    bool MsgbusHandler::add_reg_msg(tString const&  msg_id
                                    , MSGBUSPROC_2 pfun
                                    , HandlerEntryType type /*= Receiver*/
                                    , bool is_ui_thread /*= true*/
                                    , bool is_sync_ui_thread /*= true*/ )
    {
        bool added = add_reg_msg(msg_id
                                 , type
                                 , is_ui_thread
                                 , is_sync_ui_thread);
        
        if(added)
        {
            MessageSinkEntry_t entry;
            entry.msg_id_ = msg_id;
            entry.functor_.MsgProc_2 = pfun;
            entry.msgbus_proc_type_ = eMsgBusProcType2;
            msg_handler_[entry.msg_id_] = entry;
        }
        
        return added;
    }
    
    bool MsgbusHandler::add_reg_msg(tString const&  msg_id
                                    , MSGBUSPROC_1 pfun
                                    , HandlerEntryType type /*= Receiver*/
                                    , bool is_ui_thread /*= true*/
                                    , bool is_sync_ui_thread /*= true*/ )
    {
        bool hr = add_reg_msg(msg_id
                              , type
                              , is_ui_thread
                              , is_sync_ui_thread);
        
        if(hr)
        {
            MessageSinkEntry_t entry;
            entry.msg_id_ = msg_id;
            entry.functor_.MsgProc_1 = pfun;
            entry.msgbus_proc_type_ = eMsgBusProcType1;
            msg_handler_[entry.msg_id_] = entry;
        }
        
        return hr;
    }
    
    bool MsgbusHandler::add_reg_msg(tString const& msg_id
                                    , MSGBUSPROC_3 pfun
                                    , HandlerEntryType type /*= Receiver*/
                                    , bool is_ui_thread /*= true*/
                                    , bool is_sync_ui_thread /*= true*/ )
    {
        bool hr = add_reg_msg(msg_id
                              , type
                              , is_ui_thread
                              , is_sync_ui_thread);
        
        if(hr)
        {
            MessageSinkEntry_t entry;
            entry.msg_id_ = msg_id;
            entry.functor_.MsgProc_3 = pfun;
            entry.msgbus_proc_type_ = eMsgBusProcType3;
            msg_handler_[entry.msg_id_] = entry;
        }
        
        return hr;
    }
    
    bool MsgbusHandler::add_reg_msg(tString const& msg_id
                                    , MSGBUSPROC_4 pfun
                                    , HandlerEntryType type /*= eReceiver*/
                                    , bool is_ui_thread /*= true*/
                                    , bool is_sync_ui_thread /*= true*/ )
    {
        bool hr = add_reg_msg(msg_id
                              , type
                              , is_ui_thread
                              , is_sync_ui_thread);
        
        if(hr)
        {
            MessageSinkEntry_t entry;
            entry.msg_id_ = msg_id;
            entry.functor_.MsgProc_4 = pfun;
            entry.msgbus_proc_type_ = eMsgBusProcType4;
            msg_handler_[entry.msg_id_] = entry;
        }
        
        return hr;
    }
    
    bool MsgbusHandler::remove_reg_msg(tString const& msg_id)
    {
        return false;
    }
    
    bool MsgbusHandler::remove_all_msg()
    {
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    
    MsgbusHandler::MessageSinkEntry_t::MessageSinkEntry_t()
    {
        msgbus_proc_type_ = eMsgBusProcTypeError;
    }
    
    MsgbusHandler::MessageSinkEntry_t::~MessageSinkEntry_t()
    {
    }
    
    bool MsgbusHandler::MessageSinkEntry_t::invoke
    ( MHBWeakPtr const& weak_handler
     , tString const& msg_id
     , XParamPtr const& param
     , MsgbusSendWeakPtr const& sender
     , bool * filtered)
    {
        MHBPtr handler = weak_handler.lock();
        if(!handler) {return false;}
        
        if(msgbus_proc_type_ < eMsgBusProcTypeBegin
           || msgbus_proc_type_ >= eMsgBusProcTypeAll)
        {
            return false;
        }
        
        // union , just check MsgProc_0
        if(functor_.MsgProc_0 == nullptr)
        {
            return false;
        }
        
        switch(msgbus_proc_type_)
        {
            case eMsgBusProcType0:
            {
                (handler.get() ->* functor_.MsgProc_0)();
                break;
            }
                
            case eMsgBusProcType1:
            {
                (handler.get() ->* functor_.MsgProc_1)(msg_id);
                break;
            }
                
            case eMsgBusProcType2:
            {
                (handler.get() ->* functor_.MsgProc_2)(msg_id, param);
                break;
            }
                
            case eMsgBusProcType3:
            {
                (handler.get() ->* functor_.MsgProc_3)(msg_id
                                                       , param
                                                       , sender);
                break;
            }
                
            case eMsgBusProcType4:
            {
                (handler.get() ->* functor_.MsgProc_4)(msg_id
                                                       , param
                                                       , sender
                                                       , filtered);
                break;
            }
                
            default: return false;
        }
        
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}// namespace msg

