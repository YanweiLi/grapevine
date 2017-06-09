#ifndef SRC_XPARAM_MODEL_XPARAM_TRANVERSAL_IMPL_H_
#define SRC_XPARAM_MODEL_XPARAM_TRANVERSAL_IMPL_H_

#include "utility/xparam/model/typedef.h"
#include "utility/xparam/model/xinres_impl.h"
#include "utility/xparam/model/xparam_tranversal_interface.h"

namespace xparam
{
    class XParamTraversalImpl 
        : public XParamTraversal
    {
    private:
        static bool interal_traverse( XParamPtr const& node
            , InternalCallbackType callback
            , void* context);

        static void interal_to_string(
            XParamPtr const& node, 
            xstring<wchar_t>& destination, 
            xstring<wchar_t> const& indent_unit, 
            xstring<wchar_t> const& current_indent,
            xstring<wchar_t> const& crlf
            );

        template <typename CH>
        static bool interal_to_file(
            XParamPtr const& node,
            const CH* filename, 
            xstring<wchar_t> const& indent, 
            xstring<wchar_t> const& crlf, 
            xstring<wchar_t> const& encoding
            );

    public:
        static XParamTraversalPtr create();

        virtual void traverse(XParamPtr const& node
            , CallbackType callback
            , void* context) const;

        virtual wchar_t const* to_string(
            XParamPtr const& node, 
            size_t* dlen, 
            wchar_t const* indent,
            wchar_t const* crlf,
            XInResPtr& inres
            ) const;

        virtual char const* to_string(
            XParamPtr const& node, 
            size_t* dlen, 
            char const* indent,
            char const* crlf,
            unsigned int codepage, 
            XInResPtr& inres
            ) const;

        virtual bool to_file(
            XParamPtr const& node, 
            const char* filename, 
            const char* indent, 
            const char* crlf, 
            const char* encoding, 
            unsigned int codepage
            ) const;

        virtual bool to_file(
            XParamPtr const& node, 
            const wchar_t* filename, 
            const wchar_t* indent, 
            const wchar_t* crlf, 
            const wchar_t* encoding
            ) const;

        //////////////////////////////////////////////////////////////////////////
    }; // class XParamTraversalImpl
} // namespace xparam

#endif //SRC_XPARAM_MODEL_XPARAM_TRANVERSAL_IMPL_H_