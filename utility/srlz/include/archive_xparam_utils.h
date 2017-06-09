#ifndef __ARCHIVE_XPARAM_UTILS_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
#define __ARCHIVE_XPARAM_UTILS_H_AD81B9F0_043C_46de_861A_A5C30F60D657__

#include "utility/xparam/include/xparam.h"
#include "./archive_xparam_basic.h"
#include "utility/common/singleton.h"
#include "utility/common/grapevine_define.h"

NS_SERIALIZATION_LITE_BEGIN

class XParamSLHelper
{
    typedef std::shared_ptr<xparam::IParam> IParamPtr;
    DECLARE_SINGLETON_CLASS(XParamSLHelper);
    
public:
    template <typename T>
    bool from_xparam(xparam::XParamPtr const& p, T& t)
    {
        if (!p) {return false;}
        
        // iOS临时变量无法使用左值
        auto tmp = SL_SINGLE_ITEM(t);
        iarchive_xparam(p) >> tmp;
        return true;
    }
    
    template <typename T>
    xparam::XParamPtr to_xparam(tchar const* plname, T const& t)
    {
        if (!  is_valid()
            || plname == nullptr
            || *plname == 0)
        {
            return xparam::XParamPtr();
        }
        
        xparam::XParamPtr root = param_ref_->create_xparam();
        oarchive_xparam(L"", root) << SL_SINGLE_ITEM(t);
        
#ifdef UNICODE
        root->get_document()->set_name(plname, xparam::ZERO_TERMINATOR);
#else
        root->get_document()->set_name(plname, xparam::ZERO_TERMINATOR, 0);
#endif
        return root->get_document();
    }
    
    template <typename T>
    bool from_string(const std::basic_string<wchar_t>& s, T& t)
    {
        if (!is_valid()) {return false;}
        
        xparam::XParamParserPtr parser
        = param_ref_->create_xparam_parser();
        if (!parser->parse_string(s.c_str(), s.length()))
        {
            return false;
        }
        
        return from_xparam(parser->root()->get_document(), t);
    }
    
    template <typename T>
    bool from_string(const std::basic_string<char>& s
                     , unsigned int codepage
                     , T& t)
    {
        if (!is_valid()) {return false;}
        
        xparam::XParamParserPtr parser
        = param_ref_->create_xparam_parser();
        if (!parser->parse_string(s.c_str()
                                  , s.length()
                                  , codepage))
        {
            return false;
        }
        
        return from_xparam(parser->root()->get_document(), t);
    }
    
    template <typename T>
    bool to_string(std::basic_string<wchar_t>& s
                   , wchar_t const* lpname
                   , T const& t
                   , wchar_t const* indent = L"\t"
                   , wchar_t const* crlf = L"\r\n")
    {
        xparam::XParamPtr p = to_xparam(lpname, t);
        if (!p) {return false;}
        if (!is_valid()){return false;}
        
        xparam::XParamTraversalPtr traversal
        = param_ref_->create_xparam_traversal();
        
        xparam::XInResPtr inres;
        size_t dlen = 0;
        
        wchar_t const* dst
        = traversal->to_string(p
                               , &dlen
                               , indent
                               , crlf
                               , inres);
        
        s.assign(dst, dlen);
        return true;
    }
    
    template <typename T>
    bool to_string( std::basic_string<char>& s
                   , char const* lpszParamName
                   , unsigned int codepage
                   , T const& t
                   , char const* indent = "\t"
                   , char const* crlf = "\r\n")
    {
        xparam::XParamPtr p = to_xparam(lpszParamName, t);
        if (!p){return false;}
        if (!is_valid()){return false;}
        
        xparam::XParamTraversalPtr traversal
        = param_ref_->create_xparam_traversal();
        
        xparam::XInResPtr inres;
        size_t dlen = 0;
        
        char const* dst
        = traversal->to_string(p
                               , &dlen
                               , indent
                               , crlf
                               , codepage
                               , inres);
        
        s.assign(dst, dlen);
        return true;
    }
    
    template <typename T>
    bool from_file(lptchar lpname, T& t)
    {
        if (!is_valid()){return false;}
        
        xparam::XParamParserPtr parser
        = param_ref_->create_xparam_parser();
        if (!parser->parse_file(lpname))
        {
            return false;
        }
        
        return from_xparam(parser->root()->get_document(), t);
    }
    
    template <typename T>
    bool to_file( lptchar lpname
                 , lptchar lpparam
                 , T const& t
                 , tString encoding = _T("utf-8")
                 , tString indent = _T("\t")
                 , tString crlf = _T("\r\n"))
    {
        xparam::XParamPtr elem = to_xparam(lpparam, t);
        if (!elem){return false;}
        if (!is_valid()){return false;}
        
        xparam::XParamTraversalPtr traversal
        = param_ref_->create_xparam_traversal();
        
#ifdef UNICODE
        return traversal->to_file(elem
                                  , lpname
                                  , indent.c_str()
                                  , crlf.c_str()
                                  , encoding.c_str());
#else
        return traversal->to_file(elem
                                  , lpname
                                  , indent.c_str()
                                  , crlf.c_str()
                                  , encoding.c_str()
                                  , 0);
#endif
    }
    
    bool is_valid()
    {
        return param_ref_ != nullptr;
    }
    
private:
    XParamSLHelper()
    {
        xparam::IParam* pparam = nullptr;
        xparam::get_iparam(&pparam);
        if (!pparam)
        {
            throw std::bad_alloc();
        }
        
        param_ref_.reset(pparam , [](xparam::IParam* p)
                         {
                             xparam::free_iparam(&p);
                         });
    }
    
private:
    IParamPtr param_ref_;
};

NS_SERIALIZATION_LITE_END

//////////////////////////////////////////////////////////////////////////

#define XSL_INSTANCE Singleton<srlz::XParamSLHelper>::instance()

//////////////////////////////////////////////////////////////////////////
#endif//__ARCHIVE_XPARAM_UTILS_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
