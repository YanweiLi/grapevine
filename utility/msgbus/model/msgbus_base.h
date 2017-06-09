#ifndef SRC_MSGBUS_MODEL_MSGBUS_BASE_H_
#define SRC_MSGBUS_MODEL_MSGBUS_BASE_H_

#include "utility/msgbus/model/macrodef.h"
#include "utility/msgbus/model/msgbus_typedef.h"
#include "utility/msgbus/model/msgbus_interface.h"
#include <memory>

namespace msg
{
    //////////////////////////////////////////////////////////////////////////
    
    class MsgbusHandler : public std::enable_shared_from_this<MsgbusHandler>
    {
        typedef std::enable_shared_from_this<MsgbusHandler> _Base;
        
    public:
        typedef std::weak_ptr<MsgbusHandler> MHBWeakPtr;
        typedef std::shared_ptr<MsgbusHandler> MHBPtr;
        
        //////////////////////////////////////////////////////////////////////////
        
    public:
        MsgbusHandler(bool need_lock_res = true);
        ~MsgbusHandler();
        size_t size();
        
        bool on_message(tString const& reg_msg_id
                        , tString const& msg_id
                        , XParamPtr const& param
                        , MsgbusSendWeakPtr const& sender
                        , bool* filted);
        
        bool add_reg_msg(tString const& msg_id
                         , MSGBUSPROC_0 pfun
                         , HandlerEntryType type = eReceiver
                         , bool is_ui_thread = true
                         , bool is_sync_ui_thread = true );
        
        bool add_reg_msg(tString const& msg_id
                         , MSGBUSPROC_1 pfun
                         , HandlerEntryType type = eReceiver
                         , bool is_ui_thread = true
                         , bool is_sync_ui_thread = true );
        
        bool add_reg_msg(tString const& msg_id
                         , MSGBUSPROC_2 pfun
                         , HandlerEntryType type = eReceiver
                         , bool is_ui_thread = true
                         , bool is_sync_ui_thread = true );
        
        bool add_reg_msg(tString const& msg_id
                         , MSGBUSPROC_3 pfun
                         , HandlerEntryType type = eReceiver
                         , bool is_ui_thread = true
                         , bool is_sync_ui_thread = true );
        
        bool add_reg_msg(tString const& msg_id
                         , MSGBUSPROC_4 pfun
                         , HandlerEntryType type = eReceiver
                         , bool is_ui_thread = true
                         , bool is_sync_ui_thread = true );
        
        bool remove_reg_msg( tString const& msg_id);
        bool remove_all_msg();
        
        //////////////////////////////////////////////////////////////////////////
        
    private:
        bool add_reg_msg( tString const& msg_id
                         , HandlerEntryType type = eReceiver
                         , bool is_ui_thread = true
                         , bool is_sync_ui_thread = true );
        
        //////////////////////////////////////////////////////////////////////////
        
        // entry
        struct MessageSinkEntry_t
        {
            MessageSinkEntry_t();
            ~MessageSinkEntry_t();
            
            bool invoke(MHBWeakPtr const& weak_handler
                        , tString const& msg_id
                        , XParamPtr const& param
                        , MsgbusSendWeakPtr const& sender
                        , bool * filtered);
            
            /////////////////////////////////
            
            tString     msg_id_;
            int         msgbus_proc_type_;
            MSGBUSPROC  functor_;
        };
        
        typedef std::map<tString, MessageSinkEntry_t> MsgHandlerMap;
        
        //////////////////////////////////////////////////////////////////////////
        
        friend bool operator == ( MsgbusHandler::MHBWeakPtr const&
                                 , MsgbusHandler::MHBWeakPtr const&);
        
        //////////////////////////////////////////////////////////////////////////
        
    private:
        bool            need_lock_res_; // 是否需要使用统一的资源锁
        LockType        res_locker_;    // 资源锁
        LockType        reg_locker;     // 注册锁
        MsgHandlerMap   msg_handler_;
        tString         guid_;
    };
    
    //////////////////////////////////////////////////////////////////////////
    
    inline bool operator == (MsgbusHandler::MHBWeakPtr const& l
                             , MsgbusHandler::MHBWeakPtr const& r)
    {
        auto lref = l.lock();
        auto rref = r.lock();
        if (lref && rref)
        {
            return lref->guid_ == rref->guid_;
        }
        
        return false;
    }
    
    //////////////////////////////////////////////////////////////////////////
    
} // namespace msg

#endif // SRC_MSGBUS_MODEL_MSGBUS_BASE_H_
