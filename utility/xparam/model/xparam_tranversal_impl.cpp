#include "utility/xparam/model/xparam_tranversal_impl.h"
#include "utility/xparam/model/xparam_impl.h"

namespace xparam
{
//////////////////////////////////////////////////////////////////////////

	xparam::XParamTraversalPtr XParamTraversalImpl::create()
	{
		return XParamTraversalPtr(new XParamTraversalImpl());
	}

	bool XParamTraversalImpl::interal_traverse(XParamPtr const& node 
		, InternalCallbackType callback 
		, void* context)
	{
		if (callback && !callback(node, context))
		{
			return false;
		}

		XParamPtr elem = node->first_child();
		for (; elem ; elem = elem->next())
		{
			if (!interal_traverse(elem, callback, context))
			{
				return false;
			}
		}
		return true;
	}

	void XParamTraversalImpl::traverse(XParamPtr const& node 
		, CallbackType callback 
		, void* context) const
	{
		if (callback)
		{
			struct CallbackProxy
			{
				static bool exec(XParamPtr const& node, void* context)
				{
					CallbackProxy* pThis = reinterpret_cast<CallbackProxy*>(context);
					return pThis->m_callback 
						? pThis->m_callback(node, pThis->m_context) : true;
				}

				CallbackType m_callback;
				void* m_context;
			};

			CallbackProxy proxy = {callback, context};
			interal_traverse(node, &CallbackProxy::exec, &proxy);
		}
		else
		{
			interal_traverse(node, NULL, NULL);
		}
	}

	void XParamTraversalImpl::interal_to_string(XParamPtr const& node
		, xstring<wchar_t>& destination
		, xstring<wchar_t> const& indent_unit
		, xstring<wchar_t> const& current_indent
		, xstring<wchar_t> const& crlf)
	{
		if (node->name().empty())
		{
			XParamPtr elem = node->first_child();
			for (; elem; elem = elem->next())
			{
				if (elem->vnode() != xvn_attr)
				{
					interal_to_string(elem
						, destination
						, indent_unit
						, current_indent
						, crlf);
				}
			}

			return;
		}

		destination += current_indent;
		destination.append(L'<');
		destination += node->name();

		size_t ChildElementCount = 0;
		XParamPtr elem = node->first_child();
		for (; elem; elem = elem->next())
		{
			if (elem->vnode() == xvn_attr)
			{
				destination.append(L' ');
				destination += elem->name();
				destination.append(L"=\"", 2);
				wchar_t const* val;
				size_t vallen;
				XInResPtr inres;
				if (elem->get_value(val, vallen, inres))
				{
					XmlEscaperImpl::escapeStr(val, vallen, destination);
				}

				destination.append(L'\"');
			}
			else
			{
				++ ChildElementCount;
			}
		}

		if (ChildElementCount)
		{
			destination.append(L'>');
			destination += crlf;
			xstring<wchar_t> new_indent = current_indent;
			new_indent += indent_unit;
			if (!node->is_empty_type())
			{
				wchar_t const* val;
				size_t vallen;
				XInResPtr inres;
				if (node->get_value(val, vallen, inres))
				{
					destination += new_indent;
					if (node->vnode() == xvn_cdata)
					{
						destination.append(L"<![CDATA[", 9);
						destination.append(val, vallen);
						destination.append(L"]]>", 3);
					}
					else
					{
						XmlEscaperImpl::escapeStr(val, vallen, destination);
					}

					destination += crlf;
				}
			}

			XParamPtr elem = node->first_child();
			for (; elem; elem = elem->next())
			{
				if (elem->vnode() != xvn_attr)
				{
					interal_to_string(elem
						, destination
						, indent_unit
						, new_indent
						, crlf);
				}
			}

			destination += current_indent;
			destination.append(L"</", 2);
			destination += node->name();
			destination.append(L'>');
			destination += crlf;
		}
		else
		{
			if (!node->is_empty_type())
			{
				destination.append(L'>');
				wchar_t const* val;
				size_t vallen;
				XInResPtr inres;
				if (node->get_value(val, vallen, inres))
				{
					if (node->vnode() == xvn_cdata)
					{
						destination.append(L"<![CDATA[", 9);
						destination.append(val, vallen);
						destination.append(L"]]>", 3);
					}
					else
					{
						XmlEscaperImpl::escapeStr(val, vallen, destination);
					}
				}
				destination.append(L"</", 2);
				destination += node->name();
				destination.append(L'>');
			}
			else
			{
				destination.append(L"/>", 2);
			}
			destination += crlf;
		}
	}

	wchar_t const* XParamTraversalImpl::to_string(XParamPtr const& node
		, size_t* dlen
		, wchar_t const* indent
		, wchar_t const* crlf
		, XInResPtr& inres) const
	{
		if (!node)
		{
			if (dlen)
				*dlen = 0;
			return L"";
		}

		xstring<wchar_t> indent_unit(indent, ZERO_TERMINATOR);
		xstring<wchar_t> crlf_str(crlf, ZERO_TERMINATOR);
		xstring<wchar_t> destination;

		interal_to_string(node
			, destination
			, indent_unit
			, xstring<wchar_t>()
			, crlf_str);

		if (dlen){*dlen = destination.length();}
		wchar_t const* result = destination.c_str();
		swap_xstring_inres(inres, destination);
		return result;
	}

	char const* XParamTraversalImpl::to_string(XParamPtr const& node
		, size_t* dlen
		, char const* indent
		, char const* crlf
		, unsigned int codepage
		, XInResPtr& inres) const
	{
		if (!node)
		{
			if (dlen){*dlen = 0;}
			return "";
		}

		xstring<wchar_t> indent_unit;
		mbs2wstring(indent_unit, indent, ZERO_TERMINATOR, codepage);
		xstring<wchar_t> crlf_str;
		mbs2wstring(crlf_str, crlf, ZERO_TERMINATOR, codepage);

		xstring<wchar_t> destination;
		interal_to_string(node
			, destination
			, indent_unit
			, xstring<wchar_t>()
			, crlf_str);

		char const* result;
		size_t resultlen;
		wstring2mbs(destination, result, resultlen, codepage, inres);
		if (dlen){*dlen = resultlen;}
		return result;
	}

	template <typename CH>
	bool XParamTraversalImpl::interal_to_file(XParamPtr const& node
		, const CH* filename
		, xstring<wchar_t> const& indent
		, xstring<wchar_t> const& crlf
		, xstring<wchar_t> const& encoding)
	{
		xstring<wchar_t> xmlpi(L"<?xml version=\"1.0\" encoding=\"", ZERO_TERMINATOR);
		wchar_t const* enc = L"utf-8";
		if (encoding.length()){enc = encoding.c_str();}
			
		xmlpi.append(enc, ZERO_TERMINATOR);
		xmlpi.append(L"\"?>", 3);
		xmlpi += crlf;

		xstring<wchar_t> s(xmlpi);
		interal_to_string(node, s, indent, xstring<wchar_t>(), crlf);
		encoder encdr;
		const void* data;
		size_t size;
		const void* bom;
		size_t bom_size;
		if (!encdr.begin(enc
			, s.c_str()
			, s.length()
			, data
			, size
			, &bom
			, &bom_size))
		{
			return false;
		}

		static const CH mode[] = {119, 98, 43, 0}; // "wb+"
		FILE* file = xfopen(filename, mode);
		if (!file){return false;}
			
		if (bom_size)
		{
			if (fwrite(bom
				, 1
				, bom_size
				, file) != bom_size)
			{
				fclose(file);
				return false;
			}
		}

		if (fwrite(data, 1, size, file) != size)
		{
			fclose(file);
			return false;
		}

		fclose(file);
		return true;
	}

	bool XParamTraversalImpl::to_file(XParamPtr const& node
		, const char* filename
		, const char* indent
		, const char* crlf
		, const char* encoding
		, unsigned int codepage) const
	{
		xstring<wchar_t> indent_unit;
		mbs2wstring(indent_unit, indent, ZERO_TERMINATOR, codepage);

		xstring <wchar_t> crlf_str;
		mbs2wstring(crlf_str, crlf, ZERO_TERMINATOR, codepage);
		
		xstring<wchar_t> enc;
		mbs2wstring(enc, encoding, ZERO_TERMINATOR, codepage);
		
		return interal_to_file(node, filename, indent_unit, crlf_str, enc);
	}

	bool XParamTraversalImpl::to_file(XParamPtr const& node
		, const wchar_t* filename
		, const wchar_t* indent
		, const wchar_t* crlf
		, const wchar_t* encoding) const
	{
#ifdef WIN32
		xstring<wchar_t> indent_unit(indent, ZERO_TERMINATOR);
		xstring<wchar_t> crlf_str(crlf, ZERO_TERMINATOR);
		xstring<wchar_t> enc(encoding, ZERO_TERMINATOR);
		return interal_to_file(node, filename, indent_unit, crlf_str, enc);
#else
		return false;
#endif
	}

//////////////////////////////////////////////////////////////////////////
} // namespace xparam