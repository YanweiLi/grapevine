#ifndef SRC_MSGBUS_MODEL_MSGBUS_TYPEDEF_H_
#define SRC_MSGBUS_MODEL_MSGBUS_TYPEDEF_H_

#include <map>

#include "utility/xparam/include/xparam.h"
#include "utility/msgbus/model/msgbus_interface.h"

#ifdef WIN32
#include <hash_map>
namespace cxx = stdext;
#else
#include <ext/hash_map>
namespace cxx = __gnu_cxx;
#endif

namespace msg
{
    //////////////////////////////////////////////////////////////////////////
    
    using namespace xparam;
    class MsgbusHandler;
    
    //////////////////////////////////////////////////////////////////////////
    
    // std
    typedef void (MsgbusHandler:: * MSGBUSPROC_0)();
    typedef void (MsgbusHandler:: * MSGBUSPROC_1)(const tString &);
    
    typedef void (MsgbusHandler:: * MSGBUSPROC_2)(const tString &
                                                  , XParamPtr const&);
    
    typedef void (MsgbusHandler:: * MSGBUSPROC_3)(const tString &
                                                  , XParamPtr const&
                                                  , MsgbusSendWeakPtr const&);
    
    typedef void (MsgbusHandler:: * MSGBUSPROC_4)(const tString &
                                                  , XParamPtr const&
                                                  , MsgbusSendWeakPtr const&
                                                  , bool*);
    
    //////////////////////////////////////////////////////////////////////////
    
    enum HandlerEntryType
    {
        eTopFilter
        , eFilter
        , eTopReceiver
        , eReceiver
    };
    
    typedef enum
    {
        eMsgBusProcTypeError = -1
        , eMsgBusProcTypeBegin = 0
        , eMsgBusProcType0 = 0
        , eMsgBusProcType1
        , eMsgBusProcType2
        , eMsgBusProcType3
        , eMsgBusProcType4
        , eMsgBusProcTypeAll
        
    }MSGBUSPROCTYPE;
    
    //////////////////////////////////////////////////////////////////////////
    
    typedef union _MsgBusProc
    {
        MSGBUSPROC_0 MsgProc_0;
        MSGBUSPROC_1 MsgProc_1;
        MSGBUSPROC_2 MsgProc_2;
        MSGBUSPROC_3 MsgProc_3;
        MSGBUSPROC_4 MsgProc_4;
    }MSGBUSPROC;
    
    //////////////////////////////////////////////////////////////////////////
    
    typedef std::mutex LockType;
    typedef std::lock_guard<LockType> LockGuard;
    
    //////////////////////////////////////////////////////////////////////////
    
    class HashMapString : public tString
    {
    public:
        operator size_t() const
        {
            size_t value = 0;
            int size  = (int)length();
            if (size > 0)
            {
                const tString::value_type* pInfo = this->data();
                for (int idx = 0; idx < size; idx += 2)
                {
                    value = 13 * value + *(pInfo + idx);
                }
            }
            
            return value;
        }
    };
    
#define tString2HashMapString(data)\
*(reinterpret_cast<HashMapString*>(const_cast<tString*>(&data)))
    
    //////////////////////////////////////////////////////////////////////////
    
    struct MsgbusLess
    {
        // functor for operator<
        bool operator()(const HashMapString& left
                        , const HashMapString& right) const
        {
            // apply operator< to operands
            // return (_Left < _Right);
            size_t len1 = left.length();
            size_t len2 = right.length();
            if (len1 == len2)
            {
                lptchar p1 = (lptchar)left.data() + len1;
                lptchar p2 = (lptchar)right.data() + len2;
                
                while (len1--)
                {
                    if (*--p1 != *--p2)
                    {
                        return true;
                    }
                }
                
                return false;
            }
            
            return true;
        }
    };
    
    //////////////////////////////////////////////////////////////////////////
    
} // namespace msg

#endif // SRC_MSGBUS_MODEL_MSGBUS_TYPEDEF_H_
