#ifndef __ARCHIVE_XML4CPP_UTILS_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
#define __ARCHIVE_XML4CPP_UTILS_H_AD81B9F0_043C_46de_861A_A5C30F60D657__

#include <rv/cppmodule.h>
#include <rv/xml4cpp.h>
#include "./archive_xml4cpp_basic.h"

NS_SERIALIZATION_LITE_BEGIN

class XmlSLHelper
{
public:
	XmlSLHelper() : m_Xml4cppModule(_T("xml4cpp.dll")) {}

	template <typename T>
	inline bool FromString(const std::basic_string<wchar_t>& s, T& t)
	{
		if (!m_Xml4cppModule.IsValid())
			return false;
		xml4cpp::XmlParserP parser = m_Xml4cppModule->CreateXmlParser();
		if (!parser->ParseString(s.c_str(), s.length()))
			return false;
		iarchive_xml4cpp iax(parser->Root()->DocumentElement());
		iax >> t;
		return true;
	}

	template <typename T>
	inline bool FromString(const std::basic_string<char>& s, unsigned int codepage, T& t)
	{
		if (!m_Xml4cppModule.IsValid())
			return false;
		xml4cpp::XmlParserP parser = m_Xml4cppModule->CreateXmlParser();
		if (!parser->ParseString(s.c_str(), s.length(), codepage))
			return false;
		iarchive_xml4cpp iax(parser->Root()->DocumentElement());
		iax >> t;
		return true;
	}

	template <typename T>
	inline bool ToString(
		std::basic_string<wchar_t>& s, 
		wchar_t const* lpszDocNodeName, 
		T const& t, 
		wchar_t const* indent = L"\t", 
		wchar_t const* crlf = L"\r\n"
		)
	{
		if (!m_Xml4cppModule.IsValid())
			return false;
		xml4cpp::XmlNodeP root = m_Xml4cppModule->CreateXmlNode(xml4cpp::node_root);
		oarchive_xml4cpp oax(lpszDocNodeName, root);
		oax << t;
		xml4cpp::XmlTraversalP traversal = m_Xml4cppModule->CreateXmlTraversal();
		xml4cpp::XmlInResP inres;
		size_t dlen;
		wchar_t const* dst = traversal->ToString(root, &dlen, indent, crlf, inres);
		s.assign(dst, dlen);
		return true;
	}

	template <typename T>
	inline bool ToString(
		std::basic_string<char>& s, 
		char const* lpszDocNodeName, 
		unsigned int codepage,
		T const& t, 
		char const* indent = "\t", 
		char const* crlf = "\r\n"
		)
	{
		if (!m_Xml4cppModule.IsValid())
			return false;
		xml4cpp::XmlNodeP root = m_Xml4cppModule->CreateXmlNode(xml4cpp::node_root);
		oarchive_xml4cpp oax(L"", root);
		oax << t;
		xml4cpp::XmlTraversalP traversal = m_Xml4cppModule->CreateXmlTraversal();
		xml4cpp::XmlInResP inres;
		size_t dlen;
		root->DocumentElement()->SetName(lpszDocNodeName, xml4cpp::ZERO_TERMINATOR, 0);
		char const* dst = traversal->ToString(root, &dlen, indent, crlf, codepage, inres);
		s.assign(dst, dlen);
		return true;
	}

	template <typename T>
	inline bool FromFile(
		LPCTSTR lpszFileName, 
		T& t
	)
	{
		if (!m_Xml4cppModule.IsValid())
			return false;
		xml4cpp::XmlParserP parser = m_Xml4cppModule->CreateXmlParser();
		if (!parser->ParseFile(lpszFileName))
			return false;
		iarchive_xml4cpp iax(parser->Root()->DocumentElement());
		iax >> t;
		return true;
	}

	template <typename T>
	bool ToFile(
		LPCTSTR lpszFileName, 
		LPCTSTR lpszDocNodeName, 
		T const& t,
		LPCTSTR encoding = _T("utf-8"),
		LPCTSTR indent = _T("\t"), 
		LPCTSTR crlf = _T("\r\n")
		)
	{
		if (!m_Xml4cppModule.IsValid())
			return false;
		xml4cpp::XmlNodeP root = m_Xml4cppModule->CreateXmlNode(xml4cpp::node_root);
		oarchive_xml4cpp oax(L"", root);
		oax << t;
		xml4cpp::XmlTraversalP traversal = m_Xml4cppModule->CreateXmlTraversal();
#ifdef UNICODE
		root->DocumentElement()->SetName(lpszDocNodeName, xml4cpp::ZERO_TERMINATOR);
		return traversal->ToFile(root, lpszFileName, indent, crlf, encoding);
#else
		root->DocumentElement()->SetName(lpszDocNodeName, xml4cpp::ZERO_TERMINATOR, 0);
		return traversal->ToFile(root, lpszFileName, indent, crlf, encoding, 0);
#endif
	}

private:
	CppModuleLoader<xml4cpp::IModule> m_Xml4cppModule;
};

NS_SERIALIZATION_LITE_END

#endif//__ARCHIVE_XML4CPP_UTILS_H_AD81B9F0_043C_46de_861A_A5C30F60D657__
