//
//  convert.h
//  grapevine
//
//  Created by li_yanwei on 16/7/7.
//  Copyright © 2016年 li_yanwei. All rights reserved.
//

#ifndef SRC_UTILITY_STRING_CONVERT_H_
#define SRC_UTILITY_STRING_CONVERT_H_

#include <string>
#include <stdlib.h>

namespace String
{
    /////////////////////////////////////////////////////////////////
    // 泛华
    template<typename FROM , typename TO>
    class Convert;
    
    /////////////////////////////////////////////////////////////////
    // 特化: wchar_t to char
    
    template<>
    class Convert<wchar_t , char>
    {
    public:
        std::string operator()(wchar_t const*const from)
        {
            return this->to_string(from);
        }
        
        static std::string to_string(std::wstring const& from)
        {
            std::string tmp(from.length(), '\0');
            size_t size = wcstombs((char*)tmp.data() , (wchar_t*)from.data() , from.length());
            return tmp.assign(tmp.data(),tmp.data() + size);
        }
    };
    
    typedef Convert<wchar_t , char> W2M;
#define STRING_W2M(str) String::W2M::done(str)
    
    /////////////////////////////////////////////////////////////////
    // 特化: char to wchar
    
    template<>
    class Convert<char , wchar_t>
    {
    public:
        std::wstring operator()(char const*const from)
        {
            return this->to_wstring(from);
        }
        
        static std::wstring to_wstring(std::string const& from)
        {
            std::wstring tmp(from.length(), '\0');
            size_t size = mbstowcs((wchar_t*)tmp.data() , (char*)from.data() , from.length());
            return tmp.assign(tmp.data(),tmp.data() + size);
            
        }
    };
    
    typedef Convert<char , wchar_t> M2W;
#define STRING_M2W(str) String::M2W::done(str)
    
} // namespace String

#endif /* SRC_UTILITY_STRING_CONVERT_H_ */
