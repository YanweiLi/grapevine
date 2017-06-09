#ifndef SRC_MSGBUS_MODEL_MSGBUS_H_
#define SRC_MSGBUS_MODEL_MSGBUS_H_

#include "utility/msgbus/model/msgbus_base.h"
#include <assert.h>

namespace msg
{
    //////////////////////////////////////////////////////////////////////////
    
    // 注册回调函数__RegistAll_In_Map
    template< bool InitReg = true>
    struct InitRegPolicy
    {
        template< typename T >
        static void act( T & t )
        {
            t.__RegistAll_In_Map();
        }
    };
    
    template<>
    struct InitRegPolicy<false>
    {
        template< typename T >
        static void act( T & t )
        {}
    };
    
    //////////////////////////////////////////////////////////////////////////
    
    template<class T , bool reg_all = true>
    class MsgbusPtr
    {
    public:
        MsgbusPtr(T* t = 0)
        : impl_(t)
        {
            if (t)
            {
                InitRegPolicy<reg_all>::act(*t);
            }
        }
        
        MsgbusPtr(MsgbusPtr const& r)
        : impl_(r.impl_)
        {
        }
        
        MsgbusPtr& operator = (MsgbusPtr const& r)
        {
            impl_ = r.impl_;
            return *this;
        }
        
        MsgbusPtr& operator = (T* p)
        {
            this->reset(p);
            return *this;
        }
        
        void reset(T* p = 0)
        {
            assert(p == 0 || p != impl_.get());
            
            impl_.reset(p);
            if(impl_)
            {
                InitRegPolicy<reg_all>::act(*p);
            }
        }
        
        T& operator *() const
        {
            return impl_.operator *();
        }
        
        T* operator ->() const
        {
            return impl_.operator ->();
        }
        
        T* get() const
        {
            return impl_.get();
        }
        
        long use_count() const
        {
            return impl_.use_count();
        }
        
        bool unique() const
        {
            return impl_.unique();
        }
        
        void swap(MsgbusPtr& other)
        {
            impl_.swap(other.impl_);
        }
        
        std::weak_ptr<T> weak_this()
        {
            return impl_;
        }
        
    private:
        std::shared_ptr<T> impl_;
    };
    
    //////////////////////////////////////////////////////////////////////////
} // namespace msg

#endif // SRC_MSGBUS_MODEL_MSGBUS_H_
