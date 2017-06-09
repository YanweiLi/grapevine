#include "utility/string/algorithm.h"
//#include <boost/algorithm/string.hpp>

namespace String
{
//	tString to_upper(tString const& str)
//	{
//		return boost::algorithm::to_upper_copy(str);
//	}
//
//	tString to_lower(tString const& str)
//	{
//		return boost::algorithm::to_lower_copy(str);
//	}
//
//	tString trim(tString const& str)
//	{
//		return boost::algorithm::trim_copy(str);
//	}
//
//	void replace_all(tString& str 
//		, tString const& from_tag 
//		, tString const& to_tag)
//	{
//		boost::algorithm::replace_all(str
//			, from_tag
//			, to_tag);
//	}
    
    std::vector<std::string> split(const std::string &str,const std::string &pattern)
    {
        //const char* convert to char*
        
        std::string newStr = str;
        char * strc = (char*)newStr.data();
        
        std::vector<std::string> resultVec;
        char* tmpStr = strtok(strc, pattern.c_str());
        while (tmpStr != NULL)
        {
            resultVec.push_back(std::string(tmpStr));
            tmpStr = strtok(NULL, pattern.c_str());
        }
        
        return resultVec;
    };
    
//    std::vector<std::string> split(std::string const& src
//                                   , std::string const& tag)
//    {
//        std::vector<std::string> tmp;
//        
//        boost::split(tmp
//                     , src
//                     , boost::is_any_of(tag)
//                     , boost::token_compress_on);
//        
//        return boost::move(tmp);
//    }
//    
//    std::vector<std::wstring> split(std::wstring const& src
//                                    , std::wstring const& tag)
//    {
//        std::vector<std::wstring> tmp;
//        
//        boost::split(tmp 
//                     , src 
//                     , boost::is_any_of(tag) 
//                     , boost::token_compress_on);
//        
//        return boost::move(tmp);
//    }

}
