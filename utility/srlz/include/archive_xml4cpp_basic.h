#ifndef __ARCHIVE_XML4CPP_BASIC_H_AD81B9F0_043C_46DE_861A_A5C30F60D657__
#define __ARCHIVE_XML4CPP_BASIC_H_AD81B9F0_043C_46DE_861A_A5C30F60D657__

#include "./srlz.h"
#include "./archive_pchar.h"
#include "./xml_addons.h"

#include <boost/smart_ptr.hpp>
#include <typeinfo>

NS_SERIALIZATION_LITE_BEGIN


class xml4cpp_standin;

class xml4cpp_standin : public standin
{
public:
	inline xml4cpp_standin(xml4cpp::XmlNodeP const& node) : m_node(node){}
	inline xml4cpp_standin(xml4cpp_standin const& src) : m_node(src.m_node){}
	inline static const char* static_type() {return "xml4cpp_standin{86543A65-DD5B-4f1a-9826-E9DCB0D1F955}";}
	virtual const char* type() const {return static_type();}
	xml4cpp::XmlNodeP m_node;
};

//////////////////////////////////////////////////////////////////////////
// archives 定义

//////////////////////////////////////////////////////////////////////////
// iarchive_xml4cpp_attr
// 
class iarchive_xml4cpp_attr : public iarchive_base< iarchive_xml4cpp_attr >
{
public:
	inline iarchive_xml4cpp_attr(xml4cpp::XmlAttrP const& attr)
		: m_attr(attr)
	{
	}
	inline iarchive_xml4cpp_attr(const iarchive_xml4cpp_attr& src)
		: m_attr(src.m_attr)
	{
	}
	inline bool exists() const
	{
		return m_attr;
	}
	inline wchar_t const* name(size_t* nlen, xml4cpp::XmlInResP& inres) const
	{
		if (exists())
			return m_attr->GetName(nlen, inres);
		if (nlen)
			*nlen = 0;
		return NULL;
	}
	inline void value(wchar_t const*& v, size_t* vlen, xml4cpp::XmlInResP& inres) const
	{
		if (exists())
			v = m_attr->GetValue(vlen, inres);
		else
		{
			v = NULL;
			if (vlen)
				*vlen = 0;
		}
	}
	inline void value(char const*& v, size_t* vlen, xml4cpp::XmlInResP& inres) const
	{
		if (exists())
			v = m_attr->GetValue(0, vlen, inres);
		else
		{
			v = NULL;
			if (vlen)
				*vlen = 0;
		}
	}
	inline bool equal_value(wchar_t const* v) const
	{
		if (exists())
		{
			wchar_t const* vv = NULL;
			size_t vvlen;
			xml4cpp::XmlInResP inres;
			vv = m_attr->GetValue(&vvlen, inres);
			if (!v || !vv)
				return false;
			wchar_t const* end = vv + vvlen;
			for (; vv < end && *v; ++vv, ++v)
			{
				if (*v != *vv)
					return false;
			}
			return vv == end && *v == 0;
		}
		return false;
	}
	inline iarchive_xml4cpp_attr next_sibling() const
	{
		return iarchive_xml4cpp_attr(exists() ? m_attr->NextSibling() : xml4cpp::XmlAttrP());
	}
private:
	xml4cpp::XmlAttrP m_attr;
};

//////////////////////////////////////////////////////////////////////////
// class oarchive_xml4cpp_attr
class oarchive_xml4cpp_attr : public oarchive_base<oarchive_xml4cpp_attr>
{
public:
	inline oarchive_xml4cpp_attr(wchar_t const* name, xml4cpp::XmlNodeP const& parent)
	: m_parent(parent)
	, m_name(name)
	{
	}
	inline oarchive_xml4cpp_attr(oarchive_xml4cpp_attr const& src)
	: m_parent(src.m_parent)
	, m_attr(src.m_attr)
	, m_name(src.m_name)
	{
	}
	inline wchar_t const* name()
	{
		return m_name;
	}
	inline void name(wchar_t const* n)
	{
		m_name = n;
	}
	inline void value(wchar_t const* v, size_t vlen)
	{
		ensure();
		m_attr->SetValue(v, vlen);
	}
	inline void value(char const* v, size_t vlen)
	{
		ensure();
		m_attr->SetValue(v, vlen, 0);
	}
	inline oarchive_xml4cpp_attr next_sibling(wchar_t const* n)
	{
		ensure();
		return oarchive_xml4cpp_attr(n, m_parent);
	}	
private:
	inline void ensure()
	{
		if (!m_attr)
		{
			m_attr = m_parent->CreateAttr();
			m_attr->SetName(m_name, xml4cpp::ZERO_TERMINATOR);
			m_parent->PushBackAttr(m_attr);
		}
	}
	xml4cpp::XmlNodeP m_parent;
	xml4cpp::XmlAttrP m_attr;
	wchar_t const* m_name;
};


//////////////////////////////////////////////////////////////////////////
// class iarchive_xml4cpp
class iarchive_xml4cpp : public iarchive_base<iarchive_xml4cpp>
{
public:
	inline iarchive_xml4cpp(xml4cpp::XmlNodeP const& element, bool wildmatch = false)
		: m_element(element)
		, m_wildmatch(wildmatch)
	{
	}
	inline iarchive_xml4cpp(const iarchive_xml4cpp& src)
		: m_element(src.m_element)
		, m_wildmatch(src.m_wildmatch)
	{
	}
	inline iarchive_xml4cpp(const boost::shared_ptr<standin>& src)
		: m_wildmatch(false)
	{
		boost::shared_ptr<xml4cpp_standin> s = standin_cast<xml4cpp_standin>(src);
		if (s)
			m_element = s->m_node;
		
	}
	inline bool exists() const
	{
		return m_element;
	}
	inline wchar_t const* name(size_t* nlen, xml4cpp::XmlInResP& inres) const
	{
		if (exists())
			return m_element->GetName(nlen, inres);
		if (nlen)
			*nlen = 0;
		return NULL;
	}
	inline bool equal_name(wchar_t const* n) const
	{
		if (exists())
		{
			wchar_t const* nn = NULL;
			size_t nnlen;
			xml4cpp::XmlInResP inres;
			nn = m_element->GetName(&nnlen, inres);
			if (!n || !nn)
				return false;
			wchar_t const* end = nn + nnlen;
			for (; nn < end && *n; ++nn, ++n)
			{
				if (*n != *nn)
					return false;
			}
			return nn == end && *n == 0;
		}
		return false;
	}
	inline void value(wchar_t const*& v, size_t* vlen, xml4cpp::XmlInResP& inres) const
	{
		if (exists())
		{
			xml4cpp::XmlNodeP node = m_element->FirstChild(xml4cpp::node_cdata);
			if (node)
			{
				v = node->GetValue(vlen, inres);
				return;
			}
			node = m_element->FirstChild(xml4cpp::node_text);
			if (node)
				v = node->GetValue(vlen, inres);
		}
		else
		{
			v = NULL;
			if (vlen)
				*vlen = 0;
		}
	}
	inline void value(char const*& v, size_t* vlen, xml4cpp::XmlInResP& inres) const
	{
		if (exists())
		{
			xml4cpp::XmlNodeP node = m_element->FirstChild(xml4cpp::node_cdata);
			if (node)
			{
				v = node->GetValue(0, vlen, inres);
				return;
			}
			node = m_element->FirstChild(xml4cpp::node_text);
			if (node)
				v = node->GetValue(0, vlen, inres);
		}
		else
		{
			v = NULL;
			if (vlen)
				*vlen = 0;
		}
	}
	inline bool equal_value(wchar_t const* v) const
	{
		if (exists())
		{
			wchar_t const* vv = NULL;
			size_t vvlen;
			xml4cpp::XmlInResP inres;
			vv = m_element->GetValue(&vvlen, inres);
			if (!v || !vv)
				return false;
			wchar_t const* end = vv + vvlen;
			for (; vv < end && *v; ++vv, ++v)
			{
				if (*v != *vv)
					return false;
			}
			return vv == end && *v == 0;
		}
		return false;
	}
	inline iarchive_xml4cpp first_child(wchar_t const* name, size_t nlen) const
	{
		if (!exists())
			return iarchive_xml4cpp(xml4cpp::XmlNodeP());
		if (nlen ==0 || !name || !name[0])
			return iarchive_xml4cpp(m_element->FirstChild(xml4cpp::node_element), true);
		return iarchive_xml4cpp(m_element->FirstChild(xml4cpp::node_element, name, nlen));
	}
	inline iarchive_xml4cpp next_sibling(wchar_t const* name, size_t nlen) const
	{
		if (!exists())
			return iarchive_xml4cpp(xml4cpp::XmlNodeP());
		if (m_wildmatch)
			return iarchive_xml4cpp(m_element->NextSibling(xml4cpp::node_element), true);
		return iarchive_xml4cpp(m_element->NextSibling(xml4cpp::node_element, name, nlen));
	}
	inline iarchive_xml4cpp_attr attr(wchar_t const* name, size_t nlen) const
	{
		if (!exists())
			return iarchive_xml4cpp_attr(xml4cpp::XmlAttrP());
		else if (name == NULL || name[0] == 0)
			return iarchive_xml4cpp_attr(m_element->FirstAttr());
		else
			return iarchive_xml4cpp_attr(m_element->Attribute(name, nlen));
	}
	inline void to_standin(boost::shared_ptr<standin>& t) const
	{
		if (exists())
			t.reset(new xml4cpp_standin(m_element));
	}
private:
	xml4cpp::XmlNodeP m_element;
	bool m_wildmatch;
};

//////////////////////////////////////////////////////////////////////////
// class oarchive_xml4cpp
class oarchive_xml4cpp : public oarchive_base<oarchive_xml4cpp>
{
public:
	inline oarchive_xml4cpp(wchar_t const* name, xml4cpp::XmlNodeP const& parent, xml4cpp::XML_NODE_TYPE value_ntype = xml4cpp::node_text)
	: m_parent(parent)
	, m_name(name)
	, m_value_nodetype(value_ntype)
	{
	}
	inline oarchive_xml4cpp(oarchive_xml4cpp const& src)
	: m_parent(src.m_parent)
	, m_element(src.m_element)
	, m_name(src.m_name)
	, m_value_nodetype(src.m_value_nodetype)
	{
	}
	inline wchar_t const* name()
	{
		return m_name;
	}
	inline void name(wchar_t const* n)
	{
		m_name = n;
	}
	inline void value(wchar_t const* v, size_t vlen)
	{
		ensure();
		xml4cpp::XmlNodeP child = m_element->CreateNode(m_value_nodetype); 
		m_element->PushBackChild(child);
		child->SetValue(v, vlen);
	}
	inline void value(char const* v, size_t vlen)
	{
		ensure();
		xml4cpp::XmlNodeP child = m_element->CreateNode(m_value_nodetype); 
		m_element->PushBackChild(child);
		child->SetValue(v, vlen, 0);
	}
	inline oarchive_xml4cpp first_child(wchar_t const* n, xml4cpp::XML_NODE_TYPE value_ntype)
	{
		ensure();
		return oarchive_xml4cpp(n, m_element, value_ntype);
	}
	inline oarchive_xml4cpp next_sibling(wchar_t const* n)
	{
		ensure();
		return oarchive_xml4cpp(n, m_parent, m_value_nodetype);
	}
	inline oarchive_xml4cpp_attr attr(wchar_t const* n)
	{
		ensure();
		return oarchive_xml4cpp_attr(n, m_element);
	}
	inline bool exists_attr(wchar_t const* n) const
	{
		return (m_element && m_element->Attribute(n, xml4cpp::ZERO_TERMINATOR));
	}
	inline bool exists_child(wchar_t const* n, xml4cpp::XML_NODE_TYPE t) const
	{
		return (m_element && m_element->FirstChild(t, n, xml4cpp::ZERO_TERMINATOR));
	}

	inline void from_standin(boost::shared_ptr<standin> const& t)
	{
		boost::shared_ptr<xml4cpp_standin> xsi = standin_cast<xml4cpp_standin>(t);
		if (xsi)
		{
			if (m_element)
				m_parent->DropChild(m_element);
			m_parent->PushBackChild(xsi->m_node);
		}
	}
	inline xml4cpp::XmlNodeP get() const
	{
		return m_element;
	}
private:
	inline void ensure()
	{
		if (!m_element)
		{
			m_element = m_parent->CreateNode(xml4cpp::node_element);
			m_element->SetName(m_name, xml4cpp::ZERO_TERMINATOR);
			m_parent->PushBackChild(m_element);
		}
	}
	xml4cpp::XmlNodeP m_parent;
	xml4cpp::XmlNodeP m_element;
	wchar_t const* m_name;
	xml4cpp::XML_NODE_TYPE m_value_nodetype;
};


//////////////////////////////////////////////////////////////////////////
// 序列化方法


//////////////////////////////////////////////////////////////////////////
// member_nvio的序列化
namespace detail
{
	// 使用模版特化来完成addon中的节点类型匹配
	template <typename T> struct xml4cpp_nvio_serializer_;

	template <>
	struct xml4cpp_nvio_serializer_<addon::xml_mapping_nodetype::xmn_attr>
	{
		template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
		inline static void load(const iarchive_xml4cpp& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType>& nvio)
		{
			iarchive_xml4cpp_attr attr = ar.attr(nvio.name, xml4cpp::ZERO_TERMINATOR);
			if (attr.exists())
				attr >> nvio.value;
			else
			{
				iarchive_xml4cpp child = ar.first_child(nvio.name, xml4cpp::ZERO_TERMINATOR);
				if (child.exists())
					child >> nvio.value;
			}
		}
		template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
		inline static void save(oarchive_xml4cpp& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType> const& nvio)
		{
			ar.attr(nvio.name) << nvio.value;
		}
	};
	template <>
	struct xml4cpp_nvio_serializer_<addon::xml_mapping_nodetype::xmn_text>
	{
		template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
		inline static void load(const iarchive_xml4cpp& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType>& nvio)
		{
			//TODO: 这里有个bug，当容器对象不为空，但xml内容为空时，容器对象的内容不会被清除
			iarchive_xml4cpp child = ar.first_child(nvio.name, xml4cpp::ZERO_TERMINATOR);
			if (child.exists())
				child >> nvio.value;
			else
			{
				iarchive_xml4cpp_attr attr = ar.attr(nvio.name, xml4cpp::ZERO_TERMINATOR);
				if (attr.exists())
					attr >> nvio.value;
			}
		}
		template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
		inline static void save(oarchive_xml4cpp& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType> const& nvio)
		{
			ar.first_child(nvio.name, xml4cpp::node_text) << nvio.value;
		}
	};
	template <>
	struct xml4cpp_nvio_serializer_<addon::xml_mapping_nodetype::xmn_cdata>
	{
		template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
		inline static void load(const iarchive_xml4cpp& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType>& nvio)
		{
			iarchive_xml4cpp child = ar.first_child(nvio.name, xml4cpp::ZERO_TERMINATOR);
			if (child.exists())
				child >> nvio.value;
			else
			{
				iarchive_xml4cpp_attr attr = ar.attr(nvio.name, xml4cpp::ZERO_TERMINATOR);
				if (attr.exists())
					attr >> nvio.value;
			}
		}
		template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
		inline static void save(oarchive_xml4cpp& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType> const& nvio)
		{
			ar.first_child(nvio.name, xml4cpp::node_cdata) << nvio.value;
		}
	};
}

template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
inline void serialize_load(const iarchive_xml4cpp& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType>& nvio)
{
	if (nvio.name == NULL)
		ar >> nvio.value;
	else
		detail::xml4cpp_nvio_serializer_<addon::xml_mapping<OwnerType, offset>::nodetype>::load(ar, nvio);
}

template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
inline void serialize_load(const iarchive_xml4cpp_attr& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType>& nvio)
{
	if (ar.exists())
		ar >> nvio.value;
}

template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
inline void serialize_save(oarchive_xml4cpp& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType> const& nvio)
{
	if (nvio.name == NULL)
		ar << nvio.value;
	else
		detail::xml4cpp_nvio_serializer_<addon::xml_mapping<OwnerType, offset>::nodetype>::save(ar, nvio);
}

template <typename OwnerType, size_t offset, typename CharType, typename ValueType, typename InitType>
inline void serialize_save(oarchive_xml4cpp_attr& ar, member_nvio<OwnerType, offset, CharType, ValueType, InitType> const& nvio)
{
	ar << nvio.value;
}

//////////////////////////////////////////////////////////////////////////
// standin的序列化
inline void serialize_load(iarchive_xml4cpp const& ar, boost::shared_ptr<standin>& t)
{
	ar.to_standin(t);
}

inline void serialize_load(iarchive_xml4cpp_attr const& ar, boost::shared_ptr<standin>& t)
{
	// NULL
}

inline void serialize_save(oarchive_xml4cpp& ar, boost::shared_ptr<standin> const& t)
{
	ar.from_standin(t);
}

inline void serialize_save(oarchive_xml4cpp_attr& ar, boost::shared_ptr<standin> const& t)
{
	// NULL
}

//////////////////////////////////////////////////////////////////////////
// 字符数组序列化
template <typename CharType, size_t array_size>
inline void serialize_load(iarchive_xml4cpp const& ar, CharType (&t)[array_size])
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

template <typename CharType, size_t array_size>
inline void serialize_load(iarchive_xml4cpp_attr const& ar, CharType (&t)[array_size])
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

template <typename CharType, size_t array_size>
inline void serialize_save(oarchive_xml4cpp& ar, CharType const (&t)[array_size])
{
	oarchive_pchar<CharType> opc;
	opc << t;
	ar.value(opc.pchar, opc.len);
}

template <typename CharType, size_t array_size>
inline void serialize_save(oarchive_xml4cpp_attr& ar, CharType const (&t)[array_size])
{
	oarchive_pchar<CharType> opc;
	opc << t;
	ar.value(opc.pchar, opc.len);
}

//////////////////////////////////////////////////////////////////////////
// std::basic_string序列化

template <typename CharType, typename _Traits, typename _Ax>
inline void serialize_load(iarchive_xml4cpp const& ar, std::basic_string<CharType, _Traits, _Ax>& t)
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

template <typename CharType, typename _Traits, typename _Ax>
inline void serialize_load(iarchive_xml4cpp_attr const& ar, std::basic_string<CharType, _Traits, _Ax>& t)
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

template <typename CharType, typename _Traits, typename _Ax>
inline void serialize_save(oarchive_xml4cpp& ar, std::basic_string<CharType, _Traits, _Ax> const& t)
{
	ar.value(t.c_str(), t.length());
}

template <typename CharType, typename _Traits, typename _Ax>
inline void serialize_save(oarchive_xml4cpp_attr& ar, std::basic_string<CharType, _Traits, _Ax> const& t)
{
	ar.value(t.c_str(), t.length());
}

//////////////////////////////////////////////////////////////////////////
// 指针类型的序列化，支持多态的方式使用
template <typename T>
struct serialize_loadptr_executor<iarchive_xml4cpp_attr, T>
{
	template <typename Base>
	inline static bool exec(iarchive_xml4cpp_attr const& ar, Base*& p, void*)
	{
		if (!p)
			p = ptr_creator<T>::create();
		ar >> *p;
		return true;
	}
	template <typename Base, typename CharType>
	inline static bool exec(iarchive_xml4cpp_attr const& ar, Base*& p, CharType const* clsname)
	{
		return true;
	}
};

template <typename T>
struct serialize_loadptr_executor<iarchive_xml4cpp, T>
{
	template <typename Base>
	inline static bool exec(iarchive_xml4cpp const& ar, Base*& p, void*)
	{
		if (!p)
			p = ptr_creator<T>::create();
		ar >> *p;
		return true;
	}

	template <typename Base, typename CharType>
	inline static bool exec(iarchive_xml4cpp const& ar, Base*& p, CharType const* clsname)
	{
		bool matched = false;
		if (addon::xml_inherited_tag<Base>::type() == addon::xitt_self)
			matched = ar.equal_name(clsname);
		else if (addon::xml_inherited_tag<Base>::type() == addon::xitt_attr)
		{
			iarchive_xml4cpp_attr attr = ar.attr(addon::xml_inherited_tag<Base>::name((CharType)0), -1);
			matched = attr.exists() && attr.equal_value(clsname);
		}
		else
		{
			iarchive_xml4cpp ch = ar.first_child(addon::xml_inherited_tag<Base>::name((CharType)0), -1);
			matched = ch.exists() && ch.equal_value(clsname);
		}
		if (matched)
		{
			if (!p || typeid(*p) != typeid(T))
				p = ptr_creator<T>::create();
			ar >> *static_cast<T*>(p);
			return true;
		}
		return false;
	}
};

template <typename T>
struct serialize_saveptr_executor<oarchive_xml4cpp, T>
{
	template <typename Base>
	inline static bool exec(oarchive_xml4cpp& ar, Base*const& p, void*)
	{
		if (p)
			ar << *p;
		return true;
	}
	template <typename Base, typename CharType>
	inline static bool exec(oarchive_xml4cpp& ar, Base*const& p, CharType const* clsname)
	{
		if (p && typeid(*p) == typeid(T)) // 使用type_info比较和static_cast性能上应该比dynamic_cast好，并且有静态类型检查
		{
			T* pt = static_cast<T*>(p);
			if (clsname)
			{
				if (addon::xml_inherited_tag<Base>::type() == addon::xitt_self)
				{
					ar.name(clsname);
					ar << *pt;
				}
				else if (addon::xml_inherited_tag<Base>::type() == addon::xitt_attr)
				{
					ar << *pt;
					ar.attr(addon::xml_inherited_tag<Base>::name((CharType)0)) << std::basic_string<CharType>(clsname);
				}
				else
				{
					ar << *pt;
					ar.first_child(addon::xml_inherited_tag<Base>::name((CharType)0), xml4cpp::node_text) << std::basic_string<CharType>(clsname);
				}
			}
			else
				ar << *pt;
			return true;
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////////////
// 基础类型的序列化

#define XML4CPP_TRANSLATE_BASIC_TYPE(type) \
inline void serialize_load(iarchive_xml4cpp const& ar, type& t)\
{\
	size_t vlen;\
	xml4cpp::XmlInResP inres;\
	wchar_t const* v;\
	ar.value(v, &vlen, inres);\
	iarchive_pchar<wchar_t>(v, vlen) >> t;\
}\
inline void serialize_load(iarchive_xml4cpp_attr const& ar, type& t)\
{\
	size_t vlen;\
	xml4cpp::XmlInResP inres;\
	wchar_t const* v;\
	ar.value(v, &vlen, inres);\
	iarchive_pchar<wchar_t>(v, vlen) >> t;\
}\
inline void serialize_save(oarchive_xml4cpp& ar, type const& t)\
{\
	oarchive_pchar<wchar_t> opc;\
	opc << t;\
	ar.value(opc.pchar, opc.len);\
}\
inline void serialize_save(oarchive_xml4cpp_attr& ar, type const& t)\
{\
	oarchive_pchar<wchar_t> opc;\
	opc << t;\
	ar.value(opc.pchar, opc.len);\
}\


XML4CPP_TRANSLATE_BASIC_TYPE(char)
XML4CPP_TRANSLATE_BASIC_TYPE(unsigned char)
XML4CPP_TRANSLATE_BASIC_TYPE(short)
XML4CPP_TRANSLATE_BASIC_TYPE(unsigned short)
XML4CPP_TRANSLATE_BASIC_TYPE(long)
XML4CPP_TRANSLATE_BASIC_TYPE(unsigned long)
XML4CPP_TRANSLATE_BASIC_TYPE(long long)
XML4CPP_TRANSLATE_BASIC_TYPE(unsigned long long)
XML4CPP_TRANSLATE_BASIC_TYPE(int)
XML4CPP_TRANSLATE_BASIC_TYPE(unsigned int)

XML4CPP_TRANSLATE_BASIC_TYPE(float)
XML4CPP_TRANSLATE_BASIC_TYPE(double)
XML4CPP_TRANSLATE_BASIC_TYPE(long double)

XML4CPP_TRANSLATE_BASIC_TYPE(bool)
XML4CPP_TRANSLATE_BASIC_TYPE(void*)


#undef XML4CPP_TRANSLATE_BASIC_TYPE



NS_SERIALIZATION_LITE_END

#endif//__ARCHIVE_XML4CPP_BASIC_H_AD81B9F0_043C_46DE_861A_A5C30F60D657__

