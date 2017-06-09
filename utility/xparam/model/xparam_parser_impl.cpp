#include "utility/xparam/model/xparam_parser_impl.h"
#include "utility/xparam/model/utils.h"
#include "utility/xparam/model/macrodef.h"
#include "utility/xparam/model/xparam_impl.h"

namespace xparam
{
    //////////////////////////////////////////////////////////////////////////
    
#define _error_return(code) {m_error_code = (code); return false;}
#define _option_test(option) (m_parse_options & (option))
    
    //////////////////////////////////////////////////////////////////////////
    
    XParamParserImpl::XParamParserImpl()
    : m_processor(NULL)
    , m_error_code(xpe_no_error)
    {
        
    }
    
    xparam::XParamParserImplPtr XParamParserImpl::create()
    {
        return XParamParserImplPtr(new XParamParserImpl());
    }
    
    void XParamParserImpl::parse_begin()
    {
        m_reader.reset();
        m_escaper.reset(true);
        m_text.clear();
        m_nname.clear();
        m_root = XParamImpl::create();
        m_current_node = m_root;
        m_processor = &XParamParserImpl::find_lt;
        m_error_code = xpe_no_error;
    }
    
    bool XParamParserImpl::parsing(wchar_t const* s, size_t len)
    {
        if (!m_root){_error_return(xpe_null_root);}
        if (!m_current_node){_error_return(xpe_need_initialize);}
        
        m_reader.feed(s, len);
        while (m_reader.can_read())
        {
            if ( !(this->*m_processor)() )
            {
                return false;
            }
        }
        return true;
    }
    
    bool XParamParserImpl::parsing(char const* s
                                   , size_t len
                                   , unsigned int codepage)
    {
        if (!m_root){_error_return(xpe_null_root);}
        if (!m_current_node){_error_return(xpe_need_initialize);}
        
        xstring<wchar_t> ws;
        mbs2wstring(ws, s, len, codepage);
        m_reader.feed(ws.data(), ws.length());
        while (m_reader.can_read())
        {
            if ( !(this->*m_processor)() )
            {
                return false;
            }
        }
        return true;
    }
    
    bool XParamParserImpl::parse_end()
    {
        m_current_node = NULL;
        return true;
    }
    
    bool XParamParserImpl::parse_string(wchar_t const* s, size_t len)
    {
        parse_begin();
        if (len == 0
            || s == NULL
            || (len == -1
                && s[0] == 0)) // 空串失败
        {
            return false;
        }
        
        return (parsing(s, len) && parse_end());
    }
    
    bool XParamParserImpl::parse_string(char const* s
                                        , size_t len
                                        , unsigned int codepage)
    {
        parse_begin();
        if (len == 0
            || s == NULL
            || (len == -1
                && s[0] == 0)) // 空串失败
        {
            return false;
        }
        
        return (parsing(s, len, codepage) && parse_end());
    }
    
    bool XParamParserImpl::parse_data(const void* data, size_t size)
    {
        const wchar_t* s = NULL;
        size_t len = 0;
        decoder dec;
        if (!dec.begin(data, size, s, len))
        {
            parse_begin();
            m_error_code = xec_decode_failed;
            return false;
        }
        return parse_string(s, len);
    }
    
    bool XParamParserImpl::parse_file(const wchar_t* filename)
    {
#ifdef WIN32
        return parse_file_impl(filename);
#else
        parse_begin();
        m_error_code = xec_not_supported;
        return false;
#endif
    }
    
    bool XParamParserImpl::parse_file(const char* filename)
    {
        return parse_file_impl(filename);
    }
    
    size_t XParamParserImpl::row() const
    {
        return m_reader.row();
    }
    
    size_t XParamParserImpl::col() const
    {
        return m_reader.col();
    }
    
    size_t XParamParserImpl::offset() const
    {
        return m_reader.offset();
    }
    
    unsigned int XParamParserImpl::error_code() const
    {
        return m_error_code;
    }
    
    xparam::XParamPtr XParamParserImpl::root() const
    {
        return m_root;
    }
    
    xparam::XParamPtr XParamParserImpl::detach_root()
    {
        XParamPtr r = m_root;
        m_root = NULL;
        m_current_node = NULL;
        return r;
    }
    
    bool XParamParserImpl::process_begin()
    {
        wchar_t c = m_reader.read();
        if (chartype_lt(c)) //'<'(&lt);
        {
            m_processor = &XParamParserImpl::resolve_node;
            return true;
        }
        else
            _error_return(xec_unexpected_char);
    }
    
    bool XParamParserImpl::find_lt()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_lt(c)) //'<'(&lt);
            {
                if (!m_escaper.empty())
                {
                    _error_return(xec_unescape_failed);
                }
                
                if (!m_text.empty()
                    && m_current_node != m_root
                    && m_current_node->is_empty_type())
                {
                    m_text.trim();
                    if (m_text.length())
                    {
                        m_current_node->set_value(m_text.data(), m_text.length());
                        m_current_node->value_node(xvn_text);
                    }
                }
                m_text.clear();
                m_processor = &XParamParserImpl::resolve_node;
                break;
            }
            else
            {
                if (!m_escaper.unescape(c, m_text))
                {
                    _error_return(xec_unescape_failed);
                }
            }
        }while (m_reader.can_read());
        
        return true;
    }
    
    bool XParamParserImpl::find_gt()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_gt(c)) // '>'(&gt;)
            {
                leave();
                break;
            }
            else if (!chartype_space(c))
            {
                _error_return(xec_unexpected_char);
            }
            
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::skip_to_gt()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_gt(c)) // '>'(&gt;)
            {
                leave2();
                break;
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::resolve_node()
    {
        wchar_t c = m_reader.read();
        if (chartype_special(c)) // '!'
        {
            m_processor = &XParamParserImpl::process_special;
        }
        else if (chartype_pi(c)) // '?'
        {
            // note! create pi node
            m_processor = &XParamParserImpl::process_pi;
            m_nname.clear();
        }
        else if (chartype_close(c)) // '/'
        {
            // note! end of element node
            m_processor = &XParamParserImpl::process_close;
            m_nname.clear();
        }
        else if (chartype_symbol(c))
        {
            // note! create element node
            XParamPtr node(XParamImpl::create());
            m_current_node->push_back_child(node);
            m_current_node = node;
            node->name().assign(c);
            m_processor = &XParamParserImpl::process_element;
        }
        else
        {
            _error_return(xec_unexpected_char);
        }
        return true;
    }
    
    bool XParamParserImpl::process_special() // 找"<!"后的节点名
    {
        wchar_t c = m_reader.read();
        if (chartype_lbracket(c))
        {
            m_processor = &XParamParserImpl::process_lbracket;
            m_nname.clear();
        }
        else if (chartype_dash(c))
        {
            m_processor = &XParamParserImpl::test_comment;
        }
        else
            m_processor = &XParamParserImpl::skip_to_gt;
        return true;
    }
    
    bool XParamParserImpl::process_lbracket() // 找"<!["后的节点名
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_lbracket(c))
            {
                if (m_nname.equal(L"CDATA", 5))
                {
                    m_current_node->value_node(xvn_cdata);
                    m_current_node->set_value(L"");
                }
                m_mark = 0;
                m_processor = &XParamParserImpl::find_2rbracket_gt;
                m_text.clear();
                m_nname.clear();
                break;
            }
            else if (!chartype_crlf(c))
                m_nname.append(c); // note! set name of current node
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::test_comment() //测试是否是comment
    {
        if (!chartype_dash(m_reader.read()))
            _error_return(xec_unexpected_char);
        m_mark = 0;
        m_processor = &XParamParserImpl::process_comment;
        return true;
    }
    
    bool XParamParserImpl::process_comment()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_dash(c))
            {
                if (m_mark == 1)
                {
                    m_processor = &XParamParserImpl::process_must_gt;
                    break;
                }
                else
                    ++m_mark;
            }
            else
            {
                if (m_mark > 0)
                {
                    m_mark = 0;
                }
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::find_2rbracket_gt()
    {
        static const wchar_t rbracket_rbracket_gt[3] = {L']', L']', L'>'};
        do
        {
            wchar_t c = m_reader.read();
            if (c == rbracket_rbracket_gt[m_mark])
            {
                if (m_mark == 2)		//finished
                {
                    m_current_node->set_value(m_text.data(), m_text.length());
                    m_text.clear();
                    leave2();
                    break;
                }
                else
                {
                    ++m_mark;
                }
            }
            else
            {
                if (m_mark == 1 || (m_mark == 2 && !chartype_rbracket(c)) )
                {
                    // note! set value of current node
                    m_text.append(rbracket_rbracket_gt, m_mark);
                    m_mark = 0;
                }
                // note! set value of current node
                m_text.append(c);
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::process_close()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_symbol(c))
            {
                m_nname.append(c);
            }
            else
            {
                //ensure close element name equals m_current_node name;
                if (m_nname.empty()
                    || m_nname != m_current_node->name())
                {
                    _error_return(xec_unmatched_tag);
                }
                
                m_nname.clear();
                if (chartype_gt(c))
                {
                    leave();
                    break;
                }
                else if (chartype_space(c))
                {
                    m_processor = &XParamParserImpl::find_gt;
                    break;
                }
                else
                {
                    _error_return(xec_unexpected_char);
                }
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::process_pi()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_symbol(c))
            {
                // note! set name of current node
                m_nname.append(c);
            }
            else
            {
                if (m_nname.empty())
                    _error_return(xec_unexpected_char);
                m_nname.clear();
                if (chartype_space(c))
                {
                    m_processor = &XParamParserImpl::skip_over_pi;
                    m_mark = 0;
                }
                else if (chartype_pi(c))
                    m_processor = &XParamParserImpl::process_must_gt;
                else
                    _error_return(xec_unexpected_char);
                break;
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::process_element()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_symbol(c))
            {
                m_current_node->name().append(c);
            }
            else if (chartype_space(c))
            {
                m_processor = &XParamParserImpl::find_attr;
                break;
            }
            else if (chartype_close(c))
            {
                m_processor = &XParamParserImpl::find_gt;
                break;
            }
            else if (chartype_gt(c))
            {
                leave2();
                break;
            }
            else
            {
                _error_return(xec_unexpected_char);
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::find_attr()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_close(c))
            {
                m_processor = &XParamParserImpl::find_gt;
                break;
            }
            else if (chartype_gt(c))
            {
                leave2();
                break;
            }
            else if (chartype_symbol(c))
            {
                // note! create attribute
                XParamPtr node = XParamImpl::create();
                node->name().assign(c);
                m_current_node->push_back_child(node);
                m_current_node = node;
                m_processor = &XParamParserImpl::find_attr_name;
                break;
            }
            else if (!chartype_space(c))
            {
                _error_return(xec_unexpected_char);
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::find_attr_name()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_symbol(c))
            {
                m_current_node->name().append(c);
            }
            else if (chartype_equals(c))
            {
                m_processor = &XParamParserImpl::find_attr_value_begin;
                break;
            }
            else if (chartype_space(c))
            {
                m_processor = &XParamParserImpl::find_equal;
                break;
            }
            else
            {
                _error_return(xec_unexpected_char);
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::find_equal()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_equals(c))
            {
                m_processor = &XParamParserImpl::find_attr_value_begin;
                break;
            }
            else if (!chartype_space(c))
            {
                _error_return(xec_unexpected_char);
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::find_attr_value_begin()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (chartype_quote(c))
            {
                m_current_quote = c;
                m_processor = &XParamParserImpl::find_attr_value;
                m_text.clear();
                break;
            }
            else if (!chartype_space(c))
            {
                _error_return(xec_unexpected_char);
                break;
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::find_attr_value()
    {
        do
        {
            wchar_t c = m_reader.read();
            if (c != m_current_quote)
            {
                if (!m_escaper.unescape(c, m_text))
                {
                    _error_return(xec_unescape_failed);
                }
            }
            else
            {
                if (m_escaper.empty())
                {
                    m_processor = &XParamParserImpl::find_attr;
                    m_current_node->set_value(m_text.data(), m_text.length());
                    m_current_node->value_node(xvn_attr);
                    m_current_node = m_current_node->parent();
                }
                else
                {
                    _error_return(xec_unescape_failed);
                }
                break;
            }
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::skip_over_pi()
    {
        do
        {
            if (build_instruction(m_reader.read()))
                break;
        }while (m_reader.can_read());
        return true;
    }
    
    bool XParamParserImpl::process_must_gt()
    {
        if (chartype_gt(m_reader.read()))
        {
            leave2();
            return true;
        }
        _error_return(xec_unexpected_char);
    }
    
    bool XParamParserImpl::build_instruction(wchar_t c)
    {
        static const wchar_t question_gt[2] = {L'?', L'>'};
        if (c == question_gt[m_mark])
        {
            if (m_mark == 1)
            {
                leave2();
                return true;
            }
            else
            {
                m_mark = 1;
            }
        }
        else
        {
            if (m_mark == 1 && !chartype_pi(c))
            {
                m_mark = 0;
            }
        }
        return false;
    }
    
    void XParamParserImpl::leave()
    {
        m_current_node = m_current_node->parent();
        m_processor = &XParamParserImpl::find_lt;
        m_text.clear();
    }
    
    void XParamParserImpl::leave2()
    {
        m_processor = &XParamParserImpl::find_lt;
        m_text.clear();
    }
    
} // namespace xparam