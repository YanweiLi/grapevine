#ifndef SRC_MSGBUS_MODEL_MSGBUS_HANDLER_H_
#define SRC_MSGBUS_MODEL_MSGBUS_HANDLER_H_

#include "utility/msgbus/model/msgbus_typedef.h"
#include "utility/msgbus/model/msgbus_interface.h"
#include "utility/msgbus/model/msgbus_base.h"

namespace msg
{
    //////////////////////////////////////////////////////////////////////////
    
    struct MsgHandlerWrap
    {
        MsgbusHandler::MHBWeakPtr handler_;
        bool is_enable;
        bool is_sync_ui_thread_;
    };
    
    inline bool operator == (MsgHandlerWrap const& l
                             , MsgHandlerWrap const& r)
    {
        return l.handler_ == r.handler_;
    }
    
    typedef std::list<MsgHandlerWrap> MsgHandlerList;
    
    //////////////////////////////////////////////////////////////////////////
    // gxx 中没有 hash_compare ， 自己拷贝过来
    // traits class for hash containers
    
    // TEMPLATE FUNCTION hash_value
    
#define _HASH_SEED	(size_t)0xdeadbeef
    // hash _Keyval to size_t value one-to-one
    
    template<class _Kty> inline
    size_t hash_value(const _Kty& _Keyval)
    {
        return ((size_t)_Keyval ^ _HASH_SEED);
    }
    
    // hash range of elements
    template <class _InIt>
    inline size_t _Hash_value(_InIt _Begin, _InIt _End)
    {
        size_t _Val = 2166136261U;
        
        while (_Begin != _End)
        {
            _Val = 16777619U * _Val ^ (size_t)*_Begin++;
        }
        
        return (_Val);
    }
    
    // hash string to size_t value
    template<class _Elem
    , class _Traits
    , class _Alloc> inline
    size_t hash_value(const std::basic_string<_Elem
                      , _Traits
                      , _Alloc>& _Str)
    {
        const _Elem *_Ptr = _Str.c_str();
        return (_Hash_value(_Ptr, _Ptr + _Str.size()));
    }
    
    // hash NTBS to size_t value
    inline size_t hash_value(const char *_Str)
    {
        return (_Hash_value(_Str, _Str + std::strlen(_Str)));
    }
    
    // hash NTWCS to size_t value
    inline size_t hash_value(const wchar_t *_Str)
    {
        return (_Hash_value(_Str, _Str + std::wcslen(_Str)));
    }
    
#ifdef _NATIVE_WCHAR_T_DEFINED
    
    // hash NTWCS to size_t value
    inline size_t hash_value(const unsigned short *_Str)
    {
        const unsigned short *_End = _Str;
        
        while (*_End != 0)
        {
            ++_End;
        }
        
        return (_Hash_value(_Str, _End));
    }
    
#endif /* _NATIVE_WCHAR_T_DEFINED */
    
    template<class _Kty, class _Pr = cxx::less<_Kty> >
    class hash_compare
    {
    public:
        
        // parameters for hash table
        // 0 < bucket_size
        
        enum { bucket_size = 1 };
        
        // construct with default comparator
        hash_compare() : comp(){}
        
        // construct with _Pred comparator
        hash_compare(_Pr _Pred): comp(_Pred){}
        
        // hash _Keyval to size_t value by pseudorandomizing transform
        size_t operator()(const _Kty& _Keyval) const
        {
            long _Quot = (long)(hash_value(_Keyval) & LONG_MAX);
            //ldiv_t _Qrem = std::ldiv(_Quot, 127773);
            ldiv_t _Qrem = ldiv(_Quot, 127773);
            _Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
            if (_Qrem.rem < 0)
            {
                _Qrem.rem += LONG_MAX;
            }
            
            return ((size_t)_Qrem.rem);
        }
        
        // test if _Keyval1 ordered before _Keyval2
        bool operator()(const _Kty& _Keyval1, const _Kty& _Keyval2) const
        {
            return (comp(_Keyval1, _Keyval2));
        }
        
    protected:
        // the comparator object
        _Pr comp;
    };
    
    //////////////////////////////////////////////////////////////////////////
    
    struct MsgHandlers
    {
        LockType locker_;
        tString reg_msg_id_;
        MsgHandlerList filters_;
        MsgHandlerList receivers_;
    };
    
    typedef std::shared_ptr<MsgHandlers> MsgHandlersPtr;
    typedef std::vector<MsgHandlersPtr> MsgHandlersPtrList;
    typedef std::pair<MsgHandlersPtr , MsgHandlersPtrList> MsgHandlerPair;
    
    // 直接上hash map 数据超过1000效果明显
    typedef cxx::hash_map<HashMapString
    , MsgHandlersPtr
    , hash_compare<HashMapString , MsgbusLess> > MsgHandlerMap;
    
    //////////////////////////////////////////////////////////////////////////
} // namespace msg

#endif  //SRC_MSGBUS_MODEL_MSGBUS_HANDLER_H_
