#ifndef __ARCHIVE_MSXML_BASIC_H_E9C45AE0_1318_4D40_8EF0_15F3E63D4E86__
#define __ARCHIVE_MSXML_BASIC_H_E9C45AE0_1318_4D40_8EF0_15F3E63D4E86__

//////////////////////////////////////////////////////////////////////////

#include "srlz/include/archive_xparam.h"
#include "utility/json_spirit/archive_json.h"
#include "utility/string/tstring.h"
#include "xparam/include/xparam.h"

//////////////////////////////////////////////////////////////////////////

// srlz
NS_SERIALIZATION_LITE_BEGIN

    // json
    template <class CharType>
void serialize_load(iarchive_json<CharType> const& ar
    , String::std_string_wrapper<CharType>& t)
{
    typename String::std_string_wrapper<CharType>::value_type tmp;
    serialize_load( ar, tmp );
    t = tmp;
}

template <class CharType>
void serialize_save(oarchive_json<CharType>& ar
    , String::std_string_wrapper<CharType> const& t)
{
    typename String::std_string_wrapper<CharType>::value_type 
        tmp = (const CharType*)t;
    serialize_save( ar, tmp );
}

template<>
struct detail::map_key<String::stString>
{
    static std::string Key2Str( String::stString const& key )
    {
        return Convertor< tChar, char >::done(key.c_str());
    }

    static bool Str2Key( std::string const& str, String::stString& key )
    {
        key = Convertor< char, tChar >::done(str.c_str());
        return true;
    }
};

// xparam
template <typename CharType>
void serialize_load(iarchive_xparam const& ar
    , String::std_string_wrapper<CharType>& t)
{
    if (ar.exists())
    {
        CharType const* v   = {0};
        size_t vlen         = 0;
        xparam::XInResPtr inres;
        if (ar.value(v, &vlen, inres))
        {
            t = String::std_string_wrapper<CharType>(v, vlen);
        }
    }
}

template <typename CharType>
void serialize_save(oarchive_xparam& ar
    , String::std_string_wrapper<CharType> const& t)
{
    ar.value((const CharType*)t, t.length());
}

NS_SERIALIZATION_LITE_END

#endif // __ARCHIVE_MSXML_BASIC_H_E9C45AE0_1318_4D40_8EF0_15F3E63D4E86__