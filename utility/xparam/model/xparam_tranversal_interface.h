#ifndef SRC_XPARAM_MODEL_XPARAM_TRANVERSAL_INTERFACE_H_
#define SRC_XPARAM_MODEL_XPARAM_TRANVERSAL_INTERFACE_H_

#include "utility/xparam/model/typedef.h"

namespace xparam
{
    class XParamTraversal
    {
    public:
        virtual ~XParamTraversal(){}

        virtual void traverse(
            XParamPtr const& node, 
            CallbackType callback, 
            void* context
            ) const = 0;

        virtual wchar_t const* to_string(
            XParamPtr const& node, 
            size_t* dlen, 
            wchar_t const* indent,
            wchar_t const* crlf,
            XInResPtr& inres
            ) const = 0;

        virtual char const* to_string(
            XParamPtr const& node, 
            size_t* dlen, 
            char const* indent,
            char const* crlf,
            unsigned int codepage, 
            XInResPtr& inres
            ) const = 0;

        virtual bool to_file(
            XParamPtr const& node, 
            const char* filename, 
            const char* indent, 
            const char* crlf, 
            const char* encoding, 
            unsigned int codepage
            ) const = 0;

        virtual bool to_file(
            XParamPtr const& node, 
            const wchar_t* filename, 
            const wchar_t* indent, 
            const wchar_t* crlf, 
            const wchar_t* encoding
            ) const = 0;
    };
} // namespace xparam

#endif // SRC_XPARAM_MODEL_XPARAM_TRANVERSAL_INTERFACE_H_
