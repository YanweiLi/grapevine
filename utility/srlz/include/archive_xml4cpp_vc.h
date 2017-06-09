#ifndef __ARCHIVE_XML4CPP_VC_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
#define __ARCHIVE_XML4CPP_VC_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
#include "./archive_xml4cpp_basic.h"
#include "./archive_pchar_vc.h"
#include <cstringt.h>

NS_SERIALIZATION_LITE_BEGIN

//////////////////////////////////////////////////////////////////////////
// CString–Ú¡–ªØ
template <typename CharType, typename _Traits>
inline void serialize_load(iarchive_xml4cpp const& ar, CStringT<CharType, _Traits>& t)
{
	if (ar.exists())
	{
		size_t vlen;
		xml4cpp::XmlInResP inres;
		CharType const* v;
		ar.value(v, &vlen, inres);
		iarchive_pchar<CharType>(v, vlen) >> t;
	}
}

template <typename CharType, typename _Traits>
inline void serialize_load(iarchive_xml4cpp_attr const& ar, CStringT<CharType, _Traits>& t)
{
	if (ar.exists())
	{
		size_t vlen;
		xml4cpp::XmlInResP inres;
		CharType const* v;
		ar.value(v, &vlen, inres);
		iarchive_pchar<CharType>(v, vlen) >> t;
	}
}

template <typename CharType, typename _Traits>
inline void serialize_save(oarchive_xml4cpp& ar, CStringT<CharType, _Traits> const& t)
{
	ar.value(t.GetString(), t.GetLength());
}

template <typename CharType, typename _Traits>
inline void serialize_save(oarchive_xml4cpp_attr& ar, CStringT<CharType, _Traits> const& t)
{
	ar.value(t.GetString(), t.GetLength());
}

NS_SERIALIZATION_LITE_END



#endif//__ARCHIVE_XML4CPP_VC_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
