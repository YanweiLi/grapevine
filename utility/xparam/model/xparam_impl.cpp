
#include "utility/xparam/model/xparam_impl.h"
#include "utility/xparam/model/xinres_impl.h"
#include "utility/xparam/model/utils.h"
#include "utility/xparam/model/typedef.h"
#include "utility/xparam/model/xparam_interface.h"


namespace xparam
{
    XParamImpl::XParamImpl() : parent_(nullptr)
    , vtype_(xvt_empty)
    , vnode_(xvn_text)
    , flags_(0)
    {
        
    }
    
    XParamImpl::~XParamImpl()
    {
        free_value();
    }
    
    XParamPtr XParamImpl::create()
    {
        return XParamPtr(new XParamImpl());
    }
    
    XParamPtr XParamImpl::clone() const
    {
        XParamImpl* newcopy = new XParamImpl();
        newcopy->name_ = name_;
        newcopy->vtype_ = vtype_;
        newcopy->vnode_ = vnode_;
        newcopy->flags_ = flags_;
        
        if (vtype_ == xvt_string)
        {
            call_ctor(*(xstring<wchar_t>*)newcopy->string_);
            *(xstring<wchar_t>*)newcopy->string_ = M_STRING;
        }
        else if (vtype_ == xvt_char)
            newcopy->char_ = char_;
        else if (vtype_ == xvt_uchar)
            newcopy->uchar_ = uchar_;
        else if (vtype_ == xvt_short)
            newcopy->short_ = short_;
        else if (vtype_ == xvt_ushort)
            newcopy->ushort_ = ushort_;
        else if (vtype_ == xvt_long)
            newcopy->long_ = long_;
        else if (vtype_ == xvt_ulong)
            newcopy->ulong_ = ulong_;
        else if (vtype_ == xvt_longlong)
            newcopy->longlong_ = longlong_;
        else if (vtype_ == xvt_ulonglong)
            newcopy->ulonglong_ = ulonglong_;
        else if (vtype_ == xvt_int)
            newcopy->int_ = int_;
        else if (vtype_ == xvt_uint)
            newcopy->uint_ = uint_;
        else if (vtype_ == xvt_float)
            newcopy->float_ = float_;
        else if (vtype_ == xvt_double)
            newcopy->double_ = double_;
        else if (vtype_ == xvt_longdouble)
            newcopy->longdouble_ = longdouble_;
        else if (vtype_ == xvt_bool)
            newcopy->bool_ = bool_;
        
        else if (vtype_ == xvt_pvoid)
            newcopy->pvoid_ = pvoid_;
        else if (vtype_ == xvt_sp)
        {
            call_ctor(*(std::shared_ptr<void>*)newcopy->sp_);
            *(std::shared_ptr<void>*)newcopy->sp_ = M_SP;
        }
        
#ifdef WIN32
        else if (vtype_ == xvt_iunknown)
        {
            iunknown_->AddRef();
            newcopy->iunknown_ = iunknown_;
        }
        else if (vtype_ == xvt_idispatch)
        {
            idispatch_->AddRef();
            newcopy->idispatch_ = idispatch_;
        }
#endif
        
        else if (vtype_ == xvt_blob)
        {
            call_ctor(*(xstring<unsigned char>*)newcopy->blob_);
            *(xstring<unsigned char>*)newcopy->blob_ = M_BLOB;
        }
        
        XParamPtr elem = first_child();
        for (; elem; elem = elem->next())
        {
            newcopy->push_back_child(elem->clone());
        }
        
        return XParamPtr(newcopy);
    }
    
    xparam::XParamPtr XParamImpl::create_xparam() const
    {
        return create();
    }
    
    xparam::X_VALUE_TYPE XParamImpl::get_value_type() const
    {
        return vtype_;
    }
    
    bool XParamImpl::is_empty_type() const
    {
        return vtype_ == xvt_empty;
    }
    
    bool XParamImpl::is_basic_type() const
    {
        return vtype_ > xvt_beginof_basic && vtype_ < xvt_endof_basic;
    }
    
    bool XParamImpl::is_string_compatible_type() const
    {
        return vtype_ == xvt_string || is_basic_type() || is_blob_type();
    }
    
    bool XParamImpl::is_blob_type() const
    {
        return vtype_ == xvt_blob;
    }
    
    bool XParamImpl::is_pointer_type() const
    {
        return vtype_ > xvt_beginof_pointer && vtype_ < xvt_endof_pointer;
    }
    
    xparam::X_VALUE_NODE XParamImpl::value_node() const
    {
        return vnode_;
    }
    
    void XParamImpl::value_node(X_VALUE_NODE xvn)
    {
        vnode_ = xvn;
    }
    
    bool XParamImpl::get_name(wchar_t const*& s
                              , size_t& len
                              , XInResPtr&) const
    {
        s = name_.c_str();
        len = name_.length();
        return true;
    }
    
    bool XParamImpl::get_name(char const*& s
                              , size_t& len
                              , unsigned int codepage
                              , XInResPtr& inres) const
    {
        return wstring2mbs(name_, s, len, codepage, inres);
    }
    
    void XParamImpl::set_name(wchar_t const* s, size_t len)
    {
        name_.assign(s, len);
        if (parent_ && parent_->has_index())
        {
            XParamPtr pa = parent_;
            XParamPtr prev = prev_;
            drop_self();
            pa->insert_after(prev, shared_from_this());
        }
    }
    
    void XParamImpl::set_name(char const* s, size_t len, unsigned int codepage)
    {
        mbs2wstring(name_, s, len, codepage);
        if (parent_ && parent_->has_index())
        {
            XParamPtr pa = parent_;
            XParamPtr prev = prev_;
            drop_self();
            pa->insert_after(prev, shared_from_this());
        }
    }
    
    bool XParamImpl::get_value(char& val) const
    {
        if (vtype_ == xvt_char) { val = char_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(unsigned char& val) const
    {
        if (vtype_ == xvt_uchar) { val = uchar_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(short& val) const
    {
        if (vtype_ == xvt_short) { val = short_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(long& val) const
    {
        if (vtype_ == xvt_long) { val = long_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(unsigned long& val) const
    {
        if (vtype_ == xvt_ulong) { val = ulong_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(unsigned short& val) const
    {
        if (vtype_ == xvt_ushort) { val = ushort_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(long long& val) const
    {
        if (vtype_ == xvt_longlong) { val = longlong_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(unsigned long long& val) const
    {
        if (vtype_ == xvt_ulonglong) { val = ulonglong_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(int& val) const
    {
        if (vtype_ == xvt_int) { val = int_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(unsigned int& val) const
    {
        if (vtype_ == xvt_uint) { val = uint_; return true; }
        return get_integer(val);
    }
    
    bool XParamImpl::get_value(float& val) const
    {
        if (vtype_ == xvt_float) { val = float_; return true; }
        return get_float(val);
    }
    
    bool XParamImpl::get_value(double& val) const
    {
        if (vtype_ == xvt_double) { val = double_; return true; }
        return get_float(val);
    }
    
    bool XParamImpl::get_value(long double& val) const
    {
        if (vtype_ == xvt_double) { val = longdouble_; return true; }
        return get_float(val);
    }
    
    bool XParamImpl::get_value(bool& val) const
    {
        if (vtype_ == xvt_bool)
        {
            val = bool_;
            return true;
        }
        
        if (vtype_ == xvt_string
            && M_STRING.length() > 0)
        {
            if (M_STRING[0] == L'T'
                || M_STRING[0] == L't'
                || M_STRING[0] == L'Y'
                || M_STRING[0] == L'y')
            {
                val = true;
                return true;
            }
            
            if (M_STRING[0] == L'F'
                || M_STRING[0] == L'f'
                || M_STRING[0] == L'N'
                || M_STRING[0] == L'n')
            {
                val = false;
                return true;
            }
        }
        
        unsigned int i;
        if (get_integer(i))
        {
            val = (i!= 0);
            return true;
        }
        return false;
    }
    
    bool XParamImpl::get_value(void*& val) const
    {
        if (vtype_ == xvt_pvoid) { val = pvoid_; return true; }
        //return GetValue(ptr_int::cast(val));
        return false;
    }
    
    bool XParamImpl::get_value(std::shared_ptr<void>& val) const
    {
        if (vtype_ == xvt_sp) { val = M_SP; return true; }
        return false;
    }
    
#ifdef WIN32
    bool XParamImpl::get_value(IUnknown*& val) const
    {
        if (vtype_ == xvt_iunknown)
        {
            if (val)
                val->Release();
            val = iunknown_;
            if (val)
                val->AddRef();
            return true;
        }
        if (vtype_ == xvt_idispatch)
        {
            HRESULT hr = idispatch_->QueryInterface(IID_IUnknown, (void**)&val);
            return SUCCEEDED(hr);
        }
        
        return false;
    }
    
    bool XParamImpl::get_value(IDispatch*& val) const
    {
        if (vtype_ == xvt_idispatch)
        {
            if (val)
                val->Release();
            val = idispatch_;
            if (val)
                val->AddRef();
            return true;
        }
        
        if (vtype_ == xvt_iunknown)
        {
            HRESULT hr = iunknown_->QueryInterface(IID_IDispatch, (void**)&val);
            return SUCCEEDED(hr);
        }
        
        return false;
    }
    
#endif
    
    bool XParamImpl::get_value(wchar_t const*& s
                               , size_t& len
                               , XInResPtr& inres) const
    {
        if (vtype_ == xvt_string)
        {
            s = M_STRING.c_str();
            len = M_STRING.length();
            return true;
        }
        return get_other_string(s, len, inres);
    }
    
    bool XParamImpl::get_value(char const*& s
                               , size_t& len
                               , unsigned int codepage
                               , XInResPtr& inres) const
    {
        if (vtype_ == xvt_string)
        {
            return wstring2mbs(M_STRING, s, len, codepage, inres);
        }
        return get_other_string(s, len, inres);
    }
    
    bool XParamImpl::get_value(void const*& p
                               , size_t& size
                               , XInResPtr& inres) const
    {
        if (vtype_ == xvt_blob)
        {
            p = M_BLOB.data();
            size = M_BLOB.size();
            return true;
        }
        
        if (vtype_ == xvt_string && M_STRING.length() > 0)
        {
            size_t dlen = base64_encode_len(M_STRING.length());
            unsigned char* buffer = new unsigned char[dlen];
            bool bret = base64_decode(M_STRING.data()
                                      , M_STRING.length()
                                      , buffer, dlen);
            if (!bret)
            {
                delete[] buffer;
                return false;
            }
            
            assign_array_inres(inres, buffer);
            p = buffer;
            size = dlen;
            return true;
        }
        return false;
    }
    
    void XParamImpl::set_value()
    {
        free_value();
    }
    
    void XParamImpl::set_value(char val)
    {
        free_value();
        vtype_ = xvt_char;
        char_ = val;
    }
    
    void XParamImpl::set_value(unsigned char val)
    {
        free_value();
        vtype_ = xvt_uchar;
        uchar_ = val;
    }
    
    void XParamImpl::set_value(short val)
    {
        free_value();
        vtype_ = xvt_short;
        short_ = val;
    }
    
    void XParamImpl::set_value(unsigned short val)
    {
        free_value();
        vtype_ = xvt_ushort;
        ushort_ = val;
    }
    
    void XParamImpl::set_value(long val)
    {
        free_value();
        vtype_ = xvt_long;
        long_ = val;
    }
    
    void XParamImpl::set_value(unsigned long val)
    {
        free_value();
        vtype_ = xvt_ulong;
        ulong_ = val;
    }
    
    void XParamImpl::set_value(long long val)
    {
        free_value();
        vtype_ = xvt_longlong;
        longlong_ = val;
    }
    
    void XParamImpl::set_value(unsigned long long val)
    {
        free_value();
        vtype_ = xvt_ulonglong;
        ulonglong_ = val;
    }
    
    void XParamImpl::set_value(int val)
    {
        free_value();
        vtype_ = xvt_int;
        int_ = val;
    }
    
    void XParamImpl::set_value(unsigned int val)
    {
        free_value();
        vtype_ = xvt_uint;
        uint_ = val;
    }
    
    void XParamImpl::set_value(float val)
    {
        free_value();
        vtype_ = xvt_float;
        float_ = val;
    }
    
    void XParamImpl::set_value(double val)
    {
        free_value();
        vtype_ = xvt_double;
        double_ = val;
    }
    
    void XParamImpl::set_value(long double val)
    {
        free_value();
        vtype_ = xvt_longdouble;
        longdouble_ = val;
    }
    
    void XParamImpl::set_value(bool val)
    {
        free_value();
        vtype_ = xvt_bool;
        bool_ = val;
    }
    
    void XParamImpl::set_value(void* val)
    {
        free_value();
        vtype_ = xvt_pvoid;
        pvoid_ = val;
    }
    
    void XParamImpl::set_value(std::shared_ptr<void> const& val)
    {
        if (vtype_ != xvt_sp)
        {
            free_value();
            call_ctor(M_SP);
        }
        M_SP = val;
        vtype_ = xvt_sp;
    }
    
#ifdef WIN32
    
    void XParamImpl::set_value(IUnknown* val)
    {
        if (val)
            val->AddRef();
        free_value();
        iunknown_ = val;
        vtype_ = xvt_iunknown;
    }
    
    void XParamImpl::set_value(IDispatch* val)
    {
        
        if (val)
            val->AddRef();
        free_value();
        idispatch_ = val;
        vtype_ = xvt_idispatch;
        
    }
    
#endif
    
    void XParamImpl::set_value(wchar_t const* s, size_t len)
    {
        if (vtype_ != xvt_string)
        {
            free_value();
            call_ctor(M_STRING);
        }
        xstring<wchar_t>::normalize_size(s, len);
        if (len)
            M_STRING.assign(s, len);
        vtype_ = xvt_string;
    }
    
    void XParamImpl::set_value(char const* s, size_t len, unsigned int codepage)
    {
        if (vtype_ != xvt_string)
        {
            free_value();
            call_ctor(M_STRING);
        }
        mbs2wstring(M_STRING, s, len, codepage);
        vtype_ = xvt_string;
    }
    
    void XParamImpl::set_value(void const*p, size_t size)
    {
        if (vtype_ != xvt_blob)
        {
            free_value();
            call_ctor(M_BLOB);
        }
        if (p && size)
            M_BLOB.assign(reinterpret_cast<unsigned char const*>(p), size);
        vtype_ = xvt_blob;
    }
    
    void XParamImpl::push_back_child(XParamPtr const& child)
    {
        ensure_chilren();
        children_->push_back(child, shared_from_this());
    }
    
    void XParamImpl::push_front_child(XParamPtr const& child)
    {
        ensure_chilren();
        children_->push_front(child, shared_from_this());
    }
    
    void XParamImpl::insert_before( XParamPtr const& before
                                   , XParamPtr const& child)
    {
        ensure_chilren();
        children_->insert_before(before
                                 , child
                                 , shared_from_this());
    }
    
    void XParamImpl::insert_after( XParamPtr const& after
                                  , XParamPtr const& child)
    {
        ensure_chilren();
        children_->insert_after(after, child 
                                , shared_from_this());
    }
    
    void XParamImpl::drop_child(XParamPtr const& child)
    {
        if (children_ && child)
        {
            children_->remove(child, shared_from_this());
        }
    }
    
    void XParamImpl::drop_self()
    {
        if (parent_ && parent_->child())
        {
            parent_->child()->remove(shared_from_this(), parent_);
        }
    }
    
    void XParamImpl::clear_children()
    {
        if (children_)
        {
            children_->clear();
        }
    }
    
    xparam::XParamPtr XParamImpl::get_document() const
    {
        if (name_.length())
        {
            XParamImpl* _this = const_cast<XParamImpl*>(this);
            return _this->shared_from_this();
        }
        
        return first_child();
    }
    
    xparam::XParamPtr XParamImpl::first_child() const
    {
        if (children_)
        {
            return children_->first();
        }
        
        return XParamPtr();
    }
    
    xparam::XParamPtr XParamImpl::first_child(wchar_t const* s
                                              , size_t len
                                              , size_t offset) const
    {
        if (children_)
        {
            KeyType key(s, len);
            return children_->first(key, offset);
        }
        return XParamPtr();
    }
    
    xparam::XParamPtr XParamImpl::first_child(char const* s 
                                              , size_t len 
                                              , unsigned int codepage 
                                              , size_t offset) const
    {
        if (children_)
        {
            xstring<wchar_t> ws;
            if (!mbs2wstring(ws, s, len, codepage))
            {
                return XParamPtr();
            }
            
            KeyType key(ws.data(), ws.length());
            return children_->first(key, offset);
        }
        
        return XParamPtr();
    }
    
    xparam::XParamPtr XParamImpl::last_child() const
    {
        if (children_)
        {
            return children_->last();
        }
        
        return XParamPtr();
    }
    
    xparam::XParamPtr XParamImpl::last_child(wchar_t const* s 
                                             , size_t len 
                                             , size_t offset) const
    {
        if (children_)
        {
            KeyType key(s, len);
            return children_->last(key, offset);
        }
        return XParamPtr();
    }
    
    xparam::XParamPtr XParamImpl::last_child(char const* s 
                                             , size_t len 
                                             , unsigned int codepage 
                                             , size_t offset) const
    {
        if (children_)
        {
            xstring<wchar_t> ws;
            if (!mbs2wstring(ws, s, len, codepage))
            {
                return XParamPtr();
            }
            
            KeyType key(ws.data(), ws.length());
            return children_->last(key, offset);
        }
        return XParamPtr();
    }
    
    size_t XParamImpl::children_count() const
    {
        return children_ ? children_->count() : 0;
    }
    
    bool XParamImpl::has_index() const
    {
        return children_ 
        ? children_->has_index() 
        : false;
    }
    
    void XParamImpl::build_index()
    {
        ensure_chilren();
        children_->build_index();
    }
    
    void XParamImpl::clear_index()
    {
        if (children_)
        {
            children_->clear_index();
        }
    }
    
    void XParamImpl::free_value()
    {
        if (vtype_ > xvt_endof_basic)
        {
            if (vtype_ == xvt_string)
            {
                call_dtor(M_STRING);
            }
            else if (vtype_ == xvt_blob)
            {
                call_dtor(M_BLOB);
            }
            else if (vtype_ == xvt_sp)
            {
                call_dtor(M_SP);
            }
#ifdef WIN32
            else if (vtype_ == xvt_iunknown && iunknown_)
            {
                iunknown_->Release();
            }
            else if (vtype_ == xvt_idispatch && idispatch_)
            {
                idispatch_->Release();
            }
#endif
        }
        
        vtype_ = xvt_empty;
    }
    
    xparam::XParamPtr XParamImpl::prev_name_sake() const
    {
        for (XParamPtr elem = prev_
             ; elem
             ; elem = elem->prev())
        {
            if (elem->name() == name_)
            {
                return elem;
            }
        }
        return XParamPtr();
    }
    
    xparam::XParamPtr XParamImpl::next_name_sake() const
    {
        for (XParamPtr elem = next_
             ; elem
             ; elem = elem->next())
        {
            if (elem->name() == name_)
            {
                return elem;
            }
        }
        return XParamPtr();
    }
    
    //////////////////////////////////////////////////////////////////////////
} // namespace xparam
