#include "utility/msgbus/model/msgbus_container.h"
#include "utility/msgbus/model/msgbus_typedef.h"
#include "thread/run_loop.h"
#include "utility/string/algorithm.h"
namespace msg
{
    //////////////////////////////////////////////////////////////////////////
    
    bool MsgbusContainer::start()
    {
        run_loop_id_ = cpp0x::CREATE_RUN_LOOP();
        return !run_loop_id_.empty();
    }
    
    bool MsgbusContainer::stop()
    {
        return cpp0x::DESTROY_RUN_LOOP(run_loop_id_);
    }
    
    void MsgbusContainer::push_task(tString const& msg_id
                                    , XParamPtr const& param
                                    , MsgbusSendWeakPtr const& sender)
    {
        MsgTaskItemPtr tmp = std::make_shared<MsgTaskItem>();
        tmp->msg_id_ = msg_id;
        tmp->param_ = param;
        tmp->sender_ = sender;
        
        // MsgbusContainer 单例，程序声明周期内都存在有效，因此直接使用this 指针。
        cpp0x::ASYNC_RUN_LOOP_DISPATCH(run_loop_id_, [this , tmp](bool const& stoped){
            if (!stoped) {
                process_msg_impl(tmp->msg_id_
                                 , tmp->param_
                                 , tmp->sender_
                                 , false);
            }
        });
    }
    
    //////////////////////////////////////////////////////////////////////////
    
    bool MsgbusContainer::send_msg(tString const& msg_id)
    {
        XParamPtr param;
        MsgbusSendWeakPtr sender;
        return send_msg(msg_id , param , sender);
    }
    
    bool MsgbusContainer::send_msg(tString const& msg_id
                                   , XParamPtr const& param)
    {
        MsgbusSendWeakPtr sender;
        return send_msg(msg_id , param , sender);
    }
    
    bool MsgbusContainer::send_msg(tString const& msg_id
                                   , XParamPtr const& param
                                   , MsgbusSendWeakPtr const& sender)
    {
        return process_msg_impl(msg_id , param , sender , true);
    }
    
    void MsgbusContainer::post_msg(tString const& msg_id)
    {
        XParamPtr param;
        MsgbusSendWeakPtr sender;
        post_msg(msg_id , param , sender);
    }
    
    void MsgbusContainer::post_msg(tString const& msg_id
                                   , MsgbusSendWeakPtr const& sender)
    {
        XParamPtr param;
        post_msg(msg_id, param , sender);
    }
    
    void MsgbusContainer::post_msg(tString const& msg_id
                                   , XParamPtr const& param)
    {
        MsgbusSendWeakPtr sender;
        post_msg(msg_id , param , sender);
    }
    
    void MsgbusContainer::post_msg(tString const& msg_id
                                   , XParamPtr const& param
                                   , MsgbusSendWeakPtr const& sender)
    {
        push_task(msg_id , param , sender);
    }
    
    bool MsgbusContainer::process_msg_impl(tString const& msg_id
                                           , XParamPtr const& param
                                           , MsgbusSendWeakPtr const& sender
                                           , bool is_sync_thread)
    {
        unsigned int filter_count = 0;
        unsigned int receiver_count = 0;
        
        // before msg handled
        MsgbusSenderPtr sender_ref = sender.lock();
        if (sender_ref && !is_sync_thread) {
            sender_ref->before_msg_process( msg_id
                                           , param
                                           , filter_count
                                           , receiver_count
                                           , false);
        }
        
        // handle
        bool filted = process_msg_impl(msg_id
                                       , param
                                       , sender
                                       , filter_count
                                       , receiver_count
                                       , is_sync_thread);
        
        // after msg handled
        //MsgbusSenderPtr sender_ref = sender.lock();
        if (sender_ref && !is_sync_thread)
        {
            sender_ref->after_msg_process(msg_id
                                          , param
                                          , filter_count
                                          , receiver_count
                                          , filted);
        }
        
        return filted;
    }
    
    bool MsgbusContainer::process_msg_impl(tString const& msg_id
                                           , XParamPtr const& param
                                           , MsgbusSendWeakPtr const& sender
                                           , unsigned int& filer_cout
                                           , unsigned int& receiver_count
                                           , bool is_sync_thread)
    {
        // get handlers
        MsgHandlerPair handler_pair = get_msg_handler(msg_id);
        if (!handler_pair.first
            && handler_pair.second.empty())
        {
            return false;
        }
        
        //////////////////////////////////////////////////////////////////////////
        // delete nullptr
        
        if (handler_pair.first)
        {
            MsgHandlersPtr const& handler = handler_pair.first;
            delete_invalid_handlers(handler->filters_
                                    , handler->locker_);
            
            // execute handle list
            if (execute_handler_list(msg_id
                                     , msg_id
                                     , param
                                     , sender
                                     , handler->filters_
                                     , filer_cout
                                     , is_sync_thread))
            {
                return true;
            }
        }
        
        MsgHandlersPtrList& handler_list = handler_pair.second;
        for(MsgHandlersPtr const& tmp : handler_list)
        {
            delete_invalid_handlers(tmp->filters_
                                    , tmp->locker_);
            if (execute_handler_list(msg_id
                                     , tmp->reg_msg_id_
                                     , param
                                     , sender
                                     , tmp->filters_
                                     , filer_cout
                                     , is_sync_thread))
            {
                return  true;
            }
        }
        
        //////////////////////////////////////////////////////////////////////////
        
        if (handler_pair.first)
        {
            MsgHandlersPtr const& handler = handler_pair.first;
            delete_invalid_handlers(handler->receivers_
                                    , handler->locker_);
            
            if (execute_handler_list(msg_id
                                     , msg_id
                                     , param
                                     , sender
                                     , handler->receivers_
                                     , filer_cout
                                     , is_sync_thread))
            {
                return true;
            }
        }
        
        for(MsgHandlersPtr const& tmp : handler_list)
        {
            delete_invalid_handlers(tmp->receivers_
                                    , tmp->locker_);
            if (execute_handler_list(msg_id
                                     , tmp->reg_msg_id_
                                     , param
                                     , sender
                                     , tmp->receivers_
                                     , filer_cout
                                     , is_sync_thread))
            {
                return true;
            }
        }
        
        //////////////////////////////////////////////////////////////////////////
        
        return false;
    }
    
    MsgHandlerPair MsgbusContainer::get_msg_handler( tString const& msg_id )
    {
        MsgHandlerPair tmp;
        
        //////////////////////////////////////////////////////////////////////////
        
        std::vector<tString> msg_ids = calc_msg_ids(msg_id);
        
        //////////////////////////////////////////////////////////////////////////
        {
            LockGuard guader(locker_);
            
            // find MsgHandlersPtr
            HashMapString tmp_msg_id = tString2HashMapString(msg_id);
            auto found = msg_handlers_.find(tmp_msg_id);
            if (found != msg_handlers_.end())
            {
                tmp.first = found->second;
            }
            
            //////////////////////////////////////////////////////////////////////////
            
            for(tString const& id : msg_ids)
            {
                HashMapString tmp_id
                = tString2HashMapString(id);
                auto found = msg_handlers_.find(tmp_id);
                if (found != msg_handlers_.end())
                {
                    tmp.second.push_back(found->second);
                }
            }
        }
        
        //////////////////////////////////////////////////////////////////////////
        
        return tmp;
    }
    
    std::vector<tString> MsgbusContainer::calc_msg_ids(tString const& msg_id)
    {
        static const tString::value_type
        TOKEN[] = {_T('.') , _T('\0')};
        
        std::vector<tString> tmp = String::split(msg_id , TOKEN);
        return tmp;
    }
    
    bool MsgbusContainer::reg_msg_impl(tString const& msg_id
                                       , MsgbusHandler::MHBWeakPtr const& base_weakptr
                                       , bool is_ui_thread
                                       , bool is_sync_ui_thread
                                       , bool is_top
                                       , bool filtered)
    {
        //////////////////////////////////////////////////////////////////////////
        // find msg handler
        MsgHandlersPtr msg_handler;
        {
            LockGuard guard(locker_);
            HashMapString hash_id = tString2HashMapString(msg_id);
            MsgHandlersPtr& hp = msg_handlers_[hash_id];
            if (!hp)
            {
                hp.reset(new MsgHandlers);
                hp->reg_msg_id_ = msg_id;
            }
            
            msg_handler = hp;
        }
        
        //////////////////////////////////////////////////////////////////////////
        // find handler
        
        for(auto const& wrap : msg_handler->filters_)
        {
            if (wrap.handler_ == base_weakptr)
            {
                return false;
            }
        }
        
        for(auto const& wrap : msg_handler->receivers_)
        {
            if (wrap.handler_ == base_weakptr)
            {
                return false;
            }
        }
        
        //////////////////////////////////////////////////////////////////////////
        // reg msg
        {
            LockGuard guard(msg_handler->locker_);
            
            MsgHandlerList& msg_list = filtered
            ? msg_handler->filters_
            : msg_handler->receivers_;
            
            MsgHandlerWrap wrap =
            {
                base_weakptr
                , true
                , is_sync_ui_thread
            };
            
            is_top
            ? msg_list.push_front(wrap)
            : msg_list.push_back(wrap);
        }
        
        return true;
    }
    
    bool MsgbusContainer::execute_handler_list(tString const& msg_id
                                               , tString const& reg_msg_id
                                               , XParamPtr const& param
                                               , MsgbusSendWeakPtr const& sender
                                               , MsgHandlerList const& handler
                                               , unsigned int& exec_count
                                               , bool is_sync_thread)
    {
        bool handled = false;
        
        // filter
        for(MsgHandlerWrap const& wrap : handler)
        {
            handled = handle_sync_msg(wrap
                                      , msg_id
                                      , reg_msg_id
                                      , param
                                      , sender);
            
            if (handled)
            {
                break;
            }
        }
        
        return handled;
    }
    
    void MsgbusContainer::delete_invalid_handlers
    (MsgHandlerList& handler_list
     , LockType& locker)
    {
        LockGuard guarder(locker);
        auto del = std::remove_if(handler_list.begin()
                                  ,handler_list.end()
                                  , [](MsgHandlerWrap const& wrap)->bool
                                  {
                                      MsgbusHandler::MHBPtr ref = wrap.handler_.lock();
                                      return !ref ? true : false;
                                  });
        
        handler_list.erase(del , handler_list.end());
    }
    
    bool MsgbusContainer::handle_sync_msg(MsgHandlerWrap const& wrap
                                          , tString const& msg_id
                                          , tString const& reg_msg_id
                                          , XParamPtr const& param
                                          , MsgbusSendWeakPtr const& sender)
    {
        MsgbusHandler::MHBPtr ref = wrap.handler_.lock();
        if (!ref){ return false; }
        
        bool filted = false;
        return ref->on_message(reg_msg_id
                               , msg_id
                               , param
                               , sender
                               , &filted);
    }
    
    bool MsgbusContainer::reg_filter(tString const& msg_id
                                     , MsgbusHandler::MHBWeakPtr const& handler
                                     , bool is_ui_thread
                                     , bool is_sync_ui_thread)
    {
        return reg_msg_impl(msg_id
                            , handler
                            , is_ui_thread
                            , is_sync_ui_thread
                            , false
                            , true);
    }
    
    bool MsgbusContainer::reg_top_filter(tString const& msg_id
                                         , MsgbusHandler::MHBWeakPtr const& handler
                                         , bool is_ui_thread
                                         , bool is_sync_ui_thread)
    {
        return reg_msg_impl(msg_id
                            , handler
                            , is_ui_thread
                            , is_sync_ui_thread
                            , true
                            , true);
    }
    
    bool MsgbusContainer::reg_receiver(tString const& msg_id
                                       , MsgbusHandler::MHBWeakPtr const& handler
                                       , bool is_ui_thread
                                       , bool is_sync_ui_thread)
    {
        return reg_msg_impl(msg_id
                            , handler
                            , is_ui_thread
                            , is_sync_ui_thread
                            , false
                            , false);
    }
    
    bool MsgbusContainer::reg_top_receiver(tString const& msg_id
                                           , MsgbusHandler::MHBWeakPtr const& handler
                                           , bool is_ui_thread
                                           , bool is_sync_ui_thread)
    {
        return reg_msg_impl(msg_id
                            , handler
                            , is_ui_thread
                            , is_sync_ui_thread
                            , true
                            , false);
    }
    
    //////////////////////////////////////////////////////////////////////////
} // namespace msg
