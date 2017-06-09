#ifndef SRC_XPARAM_MODEL_UTILS_H_
#define SRC_XPARAM_MODEL_UTILS_H_

#include "utility/xparam/xml/xml.h"
#include "utility/xparam/model/typedef.h"
#include "utility/xparam/model/xinres_interface.h"


namespace xparam
{
    //////////////////////////////////////////////////////////////////////////
    
    class ref_counter
    {
    public:
        ref_counter() : m_counter(1) {}
        long addref() const
        {
            return ++m_counter;
        }
        
        long release() const
        {
            return --m_counter;
        }
        
    private:
        mutable volatile std::atomic_long m_counter;
    };
    
    template <typename BaseClass, typename ThisClass>
    class ref_counter_base : public BaseClass
    {
    public:
        void addref() const
        {
            m_counter.addref();
        }
        void release() const
        {
            if (0 == m_counter.release())
            {
                delete static_cast<ThisClass const*>(this);
            }
        }
    private:
        ref_counter m_counter;
    };
    
    //////////////////////////////////////////////////////////////////////////
    
    template <typename T>
    class ArrayInRes : public ref_counter_base< XInRes, ArrayInRes<T> >
    {
    public:
        ArrayInRes(T* t) : m_array(t) {}
        virtual ~ArrayInRes() {delete[] m_array;}
        
    private:
        T* m_array;
        ArrayInRes(ArrayInRes const&);
        ArrayInRes& operator=(ArrayInRes const&);
    };
    
    template <typename T>
    void assign_array_inres(XInResPtr& inres, T* t)
    {
        inres = XInResPtr(new ArrayInRes<T>(t));
    }
    
    //////////////////////////////////////////////////////////////////////////
    
    inline static bool wstring2mbs( xstring<wchar_t> const& wstr
                                   , char const*& ms
                                   , size_t& mlen
                                   , unsigned int codepage
                                   , XInResPtr& inres)
    {
        std::string tmp = String::Convert<wchar_t, char>::to_string(wstr.c_str());
        
        if (!tmp.empty())
        {
            char* buffer = new char[tmp.length() + 1];
            memcpy(buffer, tmp.data(), tmp.length());
            mlen = tmp.length();
            ms = buffer;
            
            assign_array_inres(inres, buffer);
            return true;
        }
        
        return false;
    }
    
    inline static bool mbs2wstring( xstring<wchar_t>& wstr
                                   , char const* ms
                                   , size_t mlen,
                                   unsigned int codepage)
    {
        xstring<char>::normalize_size(ms, mlen);
        
        std::wstring tmp = String::Convert<char, wchar_t>::to_wstring(ms);
        if (!tmp.empty())
        {
            wstr.assign(tmp.data(), tmp.length());
            return true;
        }
        
        return false;
    }
    
    template <typename T, typename CharType>
    inline static void get_integer_string(T t
                                          , CharType const*& s
                                          , size_t& len
                                          , XInResPtr& inres)
    {
        size_t newlen = sizeof(T) * 4 < 8 ? 8 : sizeof(T) * 4;
        CharType* p = new CharType[newlen];
        len = tcfl::easy_itoa(t, p, newlen, 10);
        p[len < newlen ? len : newlen - 1] = 0;
        assign_array_inres(inres, p);
        s = p;
    }
    
    template <typename T, typename CharType>
    inline static void get_float_string(T t
                                        , CharType const*& s
                                        , size_t& len
                                        , XInResPtr& inres)
    {
        size_t newlen = 32;
        CharType* p = new CharType[newlen];
        len = tcfl::easy_ftoa(t, p, newlen);
        p[len < newlen ? len : newlen - 1] = 0;
        assign_array_inres(inres, p);
        s = p;
    }
    
    template <typename CharType>
    inline static void get_bool_string(bool t
                                       , CharType const*& s
                                       , size_t& len
                                       , XInResPtr&)
    {
        static const CharType true_str[] = {49, 0};
        static const CharType false_str[] = {48, 0};
        s = t ? true_str : false_str;
        len = 1;
    }
    
    template <typename CharType>
    inline static void get_pvoid_string(void const* t
                                        , CharType const*& s
                                        , size_t& len
                                        , XInResPtr& inres)
    {
        size_t newlen = sizeof(void const*) * 2 + 2;
        CharType* p = new CharType[newlen + 1];
        tcfl::easy_itoa(ptr_int::cast(t), p + 2, newlen - 2, 16);
        p[0] = 48; // '0'
        p[1] = 120; // 'x'
        p[newlen] = 0;
        assign_array_inres(inres, p);
        s = p;
        len = newlen;
    }
    
    template <typename CharType>
    inline static void get_blob_string(xstring<unsigned char> const& t
                                       , CharType const*& s
                                       , size_t& len
                                       , XInResPtr& inres)
    {
        if (t.size() == 0)
        {
            static const CharType empty_str[] = {0};
            s = empty_str;
            len = 0;
        }
        else
        {
            size_t newlen = base64_encode_len(t.size());
            CharType* p = new CharType[newlen + 1];
            Base64Encode(t.data(), t.size(), p, newlen);
            p[newlen] = 0;
            assign_array_inres(inres, p);
            s = p;
            len = newlen;
        }
    }
} // namespace xparam

#endif  // SRC_XPARAM_MODEL_UTILS_H_
