#pragma once
#include "utility/xparam/model/typedef.h"

namespace xparam
{
    class XParamParser
    {
    public:
        virtual ~XParamParser(){}
        virtual bool parse_string(wchar_t const* s, size_t len) = 0;
        virtual bool parse_string(char const* s, size_t len
            , unsigned int codepage)                            = 0;

        virtual bool parse_data(const void* data, size_t size)  = 0;
        virtual bool parse_file(const wchar_t* filename)        = 0;
        virtual bool parse_file(const char* filename)           = 0;

        virtual size_t row() const                              = 0;
        virtual size_t col() const                              = 0;
        virtual size_t offset() const                           = 0;

        virtual unsigned int error_code() const                 = 0;
        virtual XParamPtr root() const                          = 0;
        virtual XParamPtr detach_root()                         = 0;
    };
} // namespace xparam

