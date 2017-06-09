#ifndef SRC_UTILITY_STRING_ALGORITHM_H_
#define SRC_UTILITY_STRING_ALGORITHM_H_
#include "utility/common/grapevine_define.h"
#include <sstream>
#include <vector>

namespace String
{
    //////////////////////////////////////////////////////////////////
    
	tString to_upper(tString const& str);
	tString to_lower(tString const& str);
	
    tString trim(tString const& str);
	
    void replace_all(tString& str
		, tString const& from_tag
		, tString const& to_tag);
    
    //////////////////////////////////////////////////////////////////
    
    std::vector<std::string> split(std::string const& src
                                   , std::string const& tag);
    
    std::vector<std::wstring> split(std::wstring const& src
                                    , std::wstring const& tag);

    //////////////////////////////////////////////////////////////////
    
    template<typename T>
    std::string to_string(T const& t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    template<typename T>
    std::wstring to_wstring(T const& t)
    {
        std::wstringstream ss;
        ss << t;
        return ss.str();
    }

} // namespace algorithm
#endif // SRC_UTILITY_STRING_ALGORITHM_H_
