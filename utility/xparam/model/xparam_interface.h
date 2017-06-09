#ifndef SRC_XPARAM_MODEL_XPARAM_INTERFACE_H_
#define SRC_XPARAM_MODEL_XPARAM_INTERFACE_H_

#include "utility/xparam/model/typedef.h"
#include "utility/xparam/model/macrodef.h"
#include "utility/xparam/xml/xstr.h"
using namespace xstr;

namespace xparam
{
    //////////////////////////////////////////////////////////////////////////

    class XParam
    {
    public:

        virtual ~XParam(){}
        virtual XParamPtr clone() const = 0;
        virtual XParamPtr create_xparam() const = 0;

        // value type methods
        virtual X_VALUE_TYPE get_value_type() const = 0;
        virtual bool is_empty_type() const = 0;
        virtual bool is_basic_type() const = 0;
        virtual bool is_string_compatible_type() const = 0;
        virtual bool is_blob_type() const = 0;
        virtual bool is_pointer_type() const = 0;

        // value implementation methods
        virtual X_VALUE_NODE value_node() const = 0;
        virtual void value_node(X_VALUE_NODE xvn)= 0;

        // name methods
        virtual bool get_name(wchar_t const*& s
            , size_t& len
            , XInResPtr& inres) const = 0;

        virtual bool get_name(char const*& s
            , size_t& len
            , unsigned int codepage
            , XInResPtr& inres) const = 0;

        virtual void set_name(wchar_t const* s
            , size_t len = ZERO_TERMINATOR) = 0;

        virtual void set_name(char const* s
            , size_t len
            , unsigned int codepage) = 0;

        // get basic value methods
        virtual bool get_value(char& val) const = 0;
        virtual bool get_value(unsigned char& val) const = 0;
        virtual bool get_value(short& val) const = 0;
        virtual bool get_value(unsigned short& val) const = 0;
        virtual bool get_value(long& val) const = 0;
        virtual bool get_value(unsigned long& val) const = 0;
        virtual bool get_value(long long& val) const = 0;
        virtual bool get_value(unsigned long long& val) const = 0;
        virtual bool get_value(int& val) const = 0;
        virtual bool get_value(unsigned int& val) const = 0;
        virtual bool get_value(float& val) const = 0;
        virtual bool get_value(double& val) const = 0;
        virtual bool get_value(long double& val) const = 0;
        virtual bool get_value(bool& val) const = 0;

        // get pointer value methods
        virtual bool get_value(void*& val) const = 0;
        virtual bool get_value(std::shared_ptr<void>& val) const = 0;
        
#ifdef WIN32
        virtual bool get_value(IUnknown*& val) const = 0;
        virtual bool get_value(IDispatch*& val) const = 0;
#endif
        
        // get string value methods
        virtual bool get_value(wchar_t const*& s
            , size_t& len
            , XInResPtr& inres) const = 0;

        virtual bool get_value(char const*& s
            , size_t& len
            , unsigned int codepage
            , XInResPtr& inres) const = 0;

        // get blob value methods
        virtual bool get_value(void const*& p
            , size_t& size
            , XInResPtr& inres) const = 0;


        // set null value methods
        virtual void set_value() = 0;

        // set basic value methods
        virtual void set_value(char val) = 0;
        virtual void set_value(unsigned char val) = 0;
        virtual void set_value(short val) = 0;
        virtual void set_value(unsigned short val) = 0;
        virtual void set_value(long val) = 0;
        virtual void set_value(unsigned long val) = 0;
        virtual void set_value(long long val) = 0;
        virtual void set_value(unsigned long long val) = 0;
        virtual void set_value(int val) = 0;
        virtual void set_value(unsigned int val) = 0;
        virtual void set_value(float val) = 0;
        virtual void set_value(double val) = 0;
        virtual void set_value(long double val) = 0;
        virtual void set_value(bool val) = 0;

        // set pointer value methods
        virtual void set_value(void* val) = 0;
        virtual void set_value(std::shared_ptr<void> const& val) = 0;
        
#ifdef WIN32
        virtual void set_value(IUnknown* val) = 0;
        virtual void set_value(IDispatch* val) = 0;
#endif
        
        // set string value methods
        virtual void set_value(wchar_t const* s
            , size_t len = ZERO_TERMINATOR) = 0;

        virtual void set_value(char const* s
            , size_t len = ZERO_TERMINATOR
            , unsigned int codepage = 0) = 0;

        // set blob value methods
        virtual void set_value(void const* p, size_t size) = 0;

        // Child methods
        virtual void push_back_child(XParamPtr const& child) = 0;
        virtual void push_front_child(XParamPtr const& child) = 0;

        virtual void insert_before(XParamPtr const& before
            , XParamPtr const& child) = 0;
        virtual void insert_after(XParamPtr const& after
            , XParamPtr const& child) = 0;

        virtual void drop_child(XParamPtr const& child) = 0;
        virtual void drop_self() = 0;
        virtual void clear_children() = 0;

        virtual XParamPtr get_document() const = 0;
        virtual XParamPtr first_child() const = 0;
        virtual XParamPtr first_child(wchar_t const* s
            , size_t len = ZERO_TERMINATOR
            , size_t offset = 0) const = 0;

        virtual XParamPtr first_child(char const* s
            , size_t len
            , unsigned int codepage
            , size_t offset) const = 0;

        virtual XParamPtr last_child() const = 0;
        virtual XParamPtr last_child(wchar_t const* s
            , size_t len = ZERO_TERMINATOR
            , size_t offset = 0) const = 0;

        virtual XParamPtr last_child(char const* s
            , size_t len
            , unsigned int codepage
            , size_t offset) const = 0;
        virtual size_t children_count() const = 0;

        // index methods
        virtual bool has_index() const = 0;
        virtual void build_index() = 0;
        virtual void clear_index() = 0;

        virtual XParamPtr prev_name_sake()  const = 0;
        virtual XParamPtr next_name_sake()  const = 0;

        //////////////////////////////////////////////////////////////////////////

        virtual XParamPtr parent() const = 0;
        virtual void parent(XParamPtr const&) = 0;

        virtual XParamPtr next() const = 0;
        virtual void next(XParamPtr const&) = 0;

        virtual XParamPtr prev() const = 0;
        virtual void prev(XParamPtr const&) = 0;

        virtual xstring<wchar_t>& name() = 0;
        virtual xstring<wchar_t> name() const = 0;
        virtual X_VALUE_NODE vnode() const = 0;
        virtual ChildrenTypePtr child() const = 0;

    };

    //////////////////////////////////////////////////////////////////////////
} // namespace xparam

#endif // SRC_XPARAM_MODEL_XPARAM_INTERFACE_H_
