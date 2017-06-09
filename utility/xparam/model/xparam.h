#ifndef SRC_XPARAM_MODEL_XPARAM_H_
#define SRC_XPARAM_MODEL_XPARAM_H_

//////////////////////////////////////////////////////////////////////////

#include "utility/xparam/model/typedef.h"
#include "utility/xparam/model/macrodef.h"

//////////////////////////////////////////////////////////////////////////

namespace xparam
{
    class IParam
    {
    public:
        virtual ~IParam(){}
        virtual XParamPtr create_xparam(wchar_t const* s = NULL
            , size_t len = ZERO_TERMINATOR) const = 0;

        virtual XParamPtr create_xparam(char const* s
            , size_t len
            , unsigned int codepage) const = 0;

        virtual XParamParserPtr create_xparam_parser() const = 0;
        virtual XParamTraversalPtr create_xparam_traversal() const = 0;
    };

    extern "C"
    {
        XPARAM_API void get_iparam(IParam** ppIParam);
        XPARAM_API void free_iparam(IParam** ppIParam);
    };
}

#endif //SRC_XPARAM_MODEL_XPARAM_H_
