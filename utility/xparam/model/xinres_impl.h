#ifndef SRC_XPARAM_MODEL_XINRES_IMPL_H_
#define SRC_XPARAM_MODEL_XINRES_IMPL_H_

#include "utility/xparam/model/xinres_interface.h"
#include "utility/xparam/model/xinres_interface.h"
#include "utility/xparam/model/utils.h"

namespace xparam
{
    //////////////////////////////////////////////////////////////////////////

    template <typename CH> 
    class XStrInRes 
        : public ref_counter_base<XInRes, XStrInRes<CH>>
    {
    public:
        explicit XStrInRes(xstring<CH>& src)
        {
            m_str.swap(src);
        }

    private:
        XStrInRes(XStrInRes const&);
        XStrInRes& operator=(XStrInRes const&);
        xstring<CH> m_str;
    };

    template <typename CH>
    void swap_xstring_inres(XInResPtr& inres, xstring<CH>& src)
    {
        inres = XInResPtr(new XStrInRes<CH>(src));
    }

    //////////////////////////////////////////////////////////////////////////
} // namespace xparam

#endif  //SRC_XPARAM_MODEL_XINRES_IMPL_H_
