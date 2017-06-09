#ifndef SRC_XPARAM_MODEL_XPARAM_IMPL_H_
#define SRC_XPARAM_MODEL_XPARAM_IMPL_H_

#include "utility/xparam/xml/xml.h"
#include "utility/xparam/model/xparam_interface.h"
#include "utility/common/noncopyable.h"
#include <memory>

namespace xparam
{
    //////////////////////////////////////////////////////////////////////////

#define M_STRING (*(xstring<wchar_t>*)string_)
#define M_BLOB (*(xstring<unsigned char>*)blob_)
#define M_SP (*(std::shared_ptr<void>*)sp_)

    //////////////////////////////////////////////////////////////////////////

    class XParamImpl 
        : public XParam
        , public cpp0x::noncopyable
        , public std::enable_shared_from_this<XParamImpl>
    {
    public:
        XParamImpl();
        virtual ~XParamImpl();

        struct
        {
            X_VALUE_TYPE vtype_ : 8;
            X_VALUE_NODE vnode_ : 8;
            unsigned int flags_ : 16;
        };

        static XParamPtr create();
        virtual XParamPtr clone() const;
        virtual XParamPtr create_xparam() const;

        // value type methods
        virtual X_VALUE_TYPE get_value_type() const;
        virtual bool is_empty_type() const;
        virtual bool is_basic_type() const;
        virtual bool is_string_compatible_type() const;
        virtual bool is_blob_type() const;
        virtual bool is_pointer_type() const;

        // value implementation methods
        virtual X_VALUE_NODE value_node() const;
        virtual void value_node(X_VALUE_NODE xvn);

        virtual bool get_name(wchar_t const*& s
            , size_t& len
            , XInResPtr&) const;

        virtual bool get_name(char const*& s
            , size_t& len
            , unsigned int codepage
            , XInResPtr& inres) const;

        virtual void set_name(wchar_t const* s
            , size_t len);
        virtual void set_name(char const* s
            , size_t len
            , unsigned int codepage);

        // get basic value methods
        virtual bool get_value(char& val) const;
        virtual bool get_value(unsigned char& val) const;
        virtual bool get_value(short& val) const;
        virtual bool get_value(unsigned short& val) const;
        virtual bool get_value(long& val) const;
        virtual bool get_value(unsigned long& val) const;
        virtual bool get_value(long long& val) const;
        virtual bool get_value(unsigned long long& val) const;
        virtual bool get_value(int& val) const;
        virtual bool get_value(unsigned int& val) const;
        virtual bool get_value(float& val) const;
        virtual bool get_value(double& val) const;
        virtual bool get_value(long double& val) const;
        virtual bool get_value(bool& val) const;

        // get pointer value methods
        virtual bool get_value(void*& val) const;
        virtual bool get_value(std::shared_ptr<void>& val) const;
        
#ifdef WIN32
        virtual bool get_value(IUnknown*& val) const;
        virtual bool get_value(IDispatch*& val) const;
#endif
        
        // get string value methods
        virtual bool get_value(wchar_t const*& s
            , size_t& len
            , XInResPtr& inres) const;
        virtual bool get_value(char const*& s
            , size_t& len
            , unsigned int codepage
            , XInResPtr& inres) const;

        // get blob value method
        virtual bool get_value(void const*& p
            , size_t& size
            , XInResPtr& inres) const;

        // set value methods
        virtual void set_value();
        virtual void set_value(char val);
        virtual void set_value(unsigned char val);
        virtual void set_value(short val);
        virtual void set_value(unsigned short val);
        virtual void set_value(long val);
        virtual void set_value(unsigned long val);
        virtual void set_value(long long val);
        virtual void set_value(unsigned long long val);
        virtual void set_value(int val);
        virtual void set_value(unsigned int val);
        virtual void set_value(float val);
        virtual void set_value(double val);
        virtual void set_value(long double val);
        virtual void set_value(bool val);
        virtual void set_value(void* val);
        virtual void set_value(std::shared_ptr<void> const& val);

#ifdef WIN32
        virtual void set_value(IUnknown* val);
        virtual void set_value(IDispatch* val);
#endif
        
        virtual void set_value(wchar_t const* s
            , size_t len);
        virtual void set_value(char const* s
            , size_t len
            , unsigned int codepage);
        virtual void set_value(void const*p
            , size_t size);

        // Child methods
        virtual void push_back_child(XParamPtr const& child);
        virtual void push_front_child(XParamPtr const& child);
        virtual void insert_before(XParamPtr const& before
            , XParamPtr const& child);
        virtual void insert_after(XParamPtr const& after
            , XParamPtr const& child);
        virtual void drop_child(XParamPtr const& child);
        virtual void drop_self();
        virtual void clear_children();
        virtual XParamPtr get_document() const;
        virtual XParamPtr first_child() const;
        virtual XParamPtr first_child(wchar_t const* s
            , size_t len
            , size_t offset) const;
        virtual XParamPtr first_child(char const* s
            , size_t len
            , unsigned int codepage
            , size_t offset) const;

        virtual XParamPtr last_child() const;
        virtual XParamPtr last_child(wchar_t const* s
            , size_t len
            , size_t offset) const;
        virtual XParamPtr last_child(char const* s
            , size_t len
            , unsigned int codepage
            , size_t offset) const;
        virtual size_t children_count() const;
        virtual bool has_index() const;
        virtual void build_index();
        virtual void clear_index();
        virtual XParamPtr prev_name_sake()  const;
        virtual XParamPtr next_name_sake()  const;

        //////////////////////////////////////////////////////////////////////////
        virtual XParamPtr parent() const
        {
            return parent_;
        }

        virtual void parent(XParamPtr const& tmp)
        {
            parent_ = tmp;
        }

        virtual void prev(XParamPtr const& tmp)
        {
            prev_ = tmp;
        }

        virtual XParamPtr prev() const
        {
            return prev_;
        }

        virtual void next(XParamPtr const& tmp)
        {
            next_ = tmp;
        }

        virtual XParamPtr next() const
        {
            return next_;
        }

        virtual xstring<wchar_t>& name()
        {
            return name_;
        }

        virtual xstring<wchar_t> name() const
        {
            return name_;
        }

        virtual X_VALUE_NODE vnode() const
        {
            return vnode_;
        }

        virtual ChildrenTypePtr child() const
        {
            return children_;
        }

        //////////////////////////////////////////////////////////////////////////

        inline void ensure_chilren()
        {
            if (!children_)
            {
                children_.reset(new ChildrenType());
            }
        }

        template <typename T>
        inline void cast_number(T& val) const
        {
            if (vtype_ == xvt_char)
                val = static_cast<T>(char_);
            else if (vtype_ == xvt_uchar)
                val = static_cast<T>(uchar_);
            else if (vtype_ == xvt_short)
                val = static_cast<T>(short_);
            else if (vtype_ == xvt_ushort)
                val = static_cast<T>(ushort_);
            else if (vtype_ == xvt_long)
                val = static_cast<T>(long_);
            else if (vtype_ == xvt_ulong)
                val = static_cast<T>(ulong_);
            else if (vtype_ == xvt_longlong)
                val = static_cast<T>(longlong_);
            else if (vtype_ == xvt_ulonglong)
                val = static_cast<T>(ulonglong_);
            else if (vtype_ == xvt_int)
                val = static_cast<T>(int_);
            else if (vtype_ == xvt_uint)
                val = static_cast<T>(uint_);
            else if (vtype_ == xvt_float)
                val = static_cast<T>(float_);
            else if (vtype_ == xvt_double)
                val = static_cast<T>(double_);
            else if (vtype_ == xvt_longdouble)
                val = static_cast<T>(longdouble_);
            else if (vtype_ == xvt_bool)
                val = static_cast<T>(bool_);
            else if (vtype_ == xvt_pvoid)
                val = static_cast<T>(ptr_int::cast(pvoid_));
        }

        template <typename T>
        inline bool get_integer(T& val) const
        {
            if (vtype_ == xvt_string)
            {
                return tcfl::easy_atoi(M_STRING.c_str()
                    , val
                    , M_STRING.c_str() 
                    + M_STRING.length());
            }

            if (!is_basic_type())
            {
                return false;
            }

            cast_number(val);
            return true;
        }

        template <typename T>
        inline bool get_float(T& val) const
        {
            if (vtype_ == xvt_string)
                return tcfl::easy_atof(M_STRING.c_str()
                , val
                , M_STRING.c_str() 
                + M_STRING.length());

            if (!is_basic_type())
            {
                return false;
            }

            cast_number(val);
            return true;
        }

        template <typename CharType>
        inline bool get_other_string(CharType const*& s
            , size_t& len
            , XInResPtr& inres) const
        {
            if (vtype_ == xvt_char)
                get_integer_string(char_, s, len, inres);
            else if (vtype_ == xvt_uchar)
                get_integer_string(uchar_, s, len, inres);
            else if (vtype_ == xvt_short)
                get_integer_string(short_, s, len, inres);
            else if (vtype_ == xvt_ushort)
                get_integer_string(ushort_, s, len, inres);
            else if (vtype_ == xvt_long)
                get_integer_string(long_, s, len, inres);
            else if (vtype_ == xvt_ulong)
                get_integer_string(ulong_, s, len, inres);
            else if (vtype_ == xvt_longlong)
                get_integer_string(longlong_, s, len, inres);
            else if (vtype_ == xvt_ulonglong)
                get_integer_string(ulonglong_, s, len, inres);
            else if (vtype_ == xvt_int)
                get_integer_string(int_, s, len, inres);
            else if (vtype_ == xvt_uint)
                get_integer_string(uint_, s, len, inres);
            else if (vtype_ == xvt_float)
                get_float_string(float_, s, len, inres);
            else if (vtype_ == xvt_double)
                get_float_string(double_, s, len, inres);
            else if (vtype_ == xvt_longdouble)
                get_float_string(longdouble_, s, len, inres);
            else if (vtype_ == xvt_bool)
                get_bool_string(bool_, s, len, inres);
            else if (vtype_ == xvt_pvoid)
                get_pvoid_string(pvoid_, s, len, inres);
            else if (vtype_ == xvt_blob)
                get_blob_string(M_BLOB, s, len, inres);
            else
                return false;
            return true;
        }

        void free_value();

    private:
        union
        {
            char char_;
            unsigned char uchar_;
            short short_;
            unsigned short ushort_;
            long long_;
            unsigned long ulong_;
            long long longlong_;
            unsigned long long ulonglong_;
            int int_;
            unsigned int uint_;
            float float_;
            double double_;
            long double longdouble_;
            bool bool_;

            void* pvoid_;
            char sp_[sizeof(std::shared_ptr<void>)];
            
#ifdef WIN32
            IUnknown* iunknown_;
            IDispatch* idispatch_;
#endif
            
            char string_[sizeof(xstring<wchar_t>)];
            char blob_[sizeof(xstring<unsigned char>)];
        };

        xstring<wchar_t> name_;
        XParamPtr parent_;
        XParamPtr prev_;
        XParamPtr next_;
        ChildrenTypePtr children_;
    };

    //////////////////////////////////////////////////////////////////////////
} // namespace xparam


#endif // SRC_XPARAM_MODEL_XPARAM_IMPL_H_
