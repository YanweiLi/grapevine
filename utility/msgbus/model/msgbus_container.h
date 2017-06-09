#ifndef SRC_MSGBUS_MODEL_MSGBUS_CONTAINER_H_
#define SRC_MSGBUS_MODEL_MSGBUS_CONTAINER_H_

#include "utility/msgbus/model/msgbus_interface.h"
#include "utility/msgbus/model/msgbus_handler.h"
#include "thread/thread.h"
#include "utility/common/singleton.h"

namespace msg
{
    //////////////////////////////////////////////////////////////////////////
    
    using namespace xparam;
    
    //////////////////////////////////////////////////////////////////////////
    
    class MsgbusContainer : public Singleton<MsgbusContainer>
    {
        DECLARE_SINGLETON_CLASS(MsgbusContainer);
        typedef std::weak_ptr<MsgbusContainer> MsgbusContainerPtr;
        
        //////////////////////////////////////////////////////////////////////////
        
        struct MsgTaskItem
        {
            tString msg_id_;
            XParamPtr param_;
            MsgbusSendWeakPtr sender_;
        };
        
        typedef std::shared_ptr<MsgTaskItem> MsgTaskItemPtr;
        
        //////////////////////////////////////////////////////////////////////////
    private:
        MsgbusContainer(){}
    public:
        ~MsgbusContainer(){}
        
    public:
        bool start();
        bool stop();
        
        bool send_msg(tString const& msg_id);
        
        bool send_msg(tString const& msg_id
                      , XParamPtr const& param);
        
        bool send_msg(tString const& msg_id
                      , XParamPtr const& param
                      , MsgbusSendWeakPtr const& sender);
        
        void post_msg(tString const& msg_id);
        
        void post_msg(tString const& msg_id
                      , MsgbusSendWeakPtr const& sender);
        
        void post_msg(tString const& msg_id
                      , XParamPtr const& param);
        
        void post_msg(tString const& msg_id
                      , XParamPtr const& param
                      , MsgbusSendWeakPtr const& sender);
        
        //////////////////////////////////////////////////////////////////////////
        
        bool reg_filter(tString const& msg_id
                        , MsgbusHandler::MHBWeakPtr const& handler
                        , bool is_ui_thread
                        , bool is_sync_ui_thread);
        
        bool reg_top_filter(tString const& msg_id
                            , MsgbusHandler::MHBWeakPtr const& handler
                            , bool is_ui_thread
                            , bool is_sync_ui_thread);
        
        bool reg_receiver(tString const& msg_id
                          , MsgbusHandler::MHBWeakPtr const& handler
                          , bool is_ui_thread
                          , bool is_sync_ui_thread);
        
        bool reg_top_receiver(tString const& msg_id
                              , MsgbusHandler::MHBWeakPtr const& handler
                              , bool is_ui_thread
                              , bool is_sync_ui_thread);
        //////////////////////////////////////////////////////////////////////////
        
    private:
        void work_guard(MsgTaskItemPtr const& elem);
        void push_task(tString const& msg_id
                       , XParamPtr const& param
                       , MsgbusSendWeakPtr const& sender);
        
        //////////////////////////////////////////////////////////////////////////
        
    private:
        MsgHandlerPair get_msg_handler(tString const& msg_id);
        std::vector<tString> calc_msg_ids(tString const& msg_id);
        
        bool reg_msg_impl(tString const& msg_id
                          , MsgbusHandler::MHBWeakPtr const& handler
                          , bool is_ui_thread
                          , bool is_sync_ui_thread
                          , bool is_top
                          , bool filtered);
        
        bool process_msg_impl(tString const& msg_id
                              , XParamPtr const& param
                              , MsgbusSendWeakPtr const& sender
                              , bool is_sync_thread);
        
        bool process_msg_impl(tString const& msg_id
                              , XParamPtr const& param
                              , MsgbusSendWeakPtr const& sender
                              , unsigned int& filer_cout
                              , unsigned int& receiver_count
                              , bool is_sync_thread);
        
        void delete_invalid_handlers(MsgHandlerList& handler_list
                                     , LockType& locker);
        
        bool execute_handler_list(tString const& msg_id
                                  , tString const& reg_msg_id
                                  , XParamPtr const& param
                                  , MsgbusSendWeakPtr const& sender
                                  , MsgHandlerList const& handler
                                  , unsigned int& exec_count
                                  , bool is_sync_thread);
        
        bool handle_sync_msg(MsgHandlerWrap const& wrap
                             , tString const& msg_id
                             , tString const& reg_msg_id
                             , XParamPtr const& param
                             , MsgbusSendWeakPtr const& sender);
    private:
        LockType locker_;
        MsgHandlerMap msg_handlers_;
        
        // 这里有个局限，只能post / send 到本模块中，如果某个任务占用CPU比较长，会阻塞线程。
        // 因此我们还可以学习 iOS中 GCD 的写法。直接写一个分发队列。
        //std::unique_ptr<cpp0x::queue<MsgTaskItemPtr>> task_queue_;
        tString run_loop_id_;
    };
    
    //////////////////////////////////////////////////////////////////////////
} // namespace msg

#endif // SRC_MSGBUS_MODEL_MSGBUS_CONTAINER_H_
