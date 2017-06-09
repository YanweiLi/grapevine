
//////////////////////////////////////////////////////////////////////////

#include "utility/xparam/model/xparam.h"
#include "utility/xparam/model/xparam_impl.h"
#include "utility/xparam/model/xparam_parser_impl.h"
#include "utility/xparam/model/xparam_tranversal_impl.h"
#include "utility/xparam/xml/xml.h"

namespace xparam
{
	//////////////////////////////////////////////////////////////////////////

	class ParamImpl : public IParam
	{
	public:
		virtual XParamPtr create_xparam(wchar_t const* s, size_t len) const
		{
			XParamPtr elem (new XParamImpl());
			elem->set_name(s, len);
			return elem;
		}

		virtual XParamPtr create_xparam(char const* s
			, size_t len
			, unsigned int codepage) const
		{
			XParamPtr elem(new XParamImpl());
			elem->set_name(s, len, codepage);
			return elem;
		}

		virtual XParamParserPtr create_xparam_parser() const
		{
			return XParamParserImpl::create();
		}

		virtual XParamTraversalPtr create_xparam_traversal() const
		{
			return XParamTraversalImpl::create();
		}
	};

	void get_iparam(IParam** ppIParam)
	{
		if (ppIParam)
		{
			delete *ppIParam;
			*ppIParam = new ParamImpl();
		}
	}

	void free_iparam(IParam** ppIParam)
	{
		if (ppIParam)
		{
			delete *ppIParam;
			*ppIParam= NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
} // namespace xparam

