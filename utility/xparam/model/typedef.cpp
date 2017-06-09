
#include "utility/xparam/model/typedef.h"
#include "utility/xparam/model/xparam_impl.h"

namespace xparam
{
    //////////////////////////////////////////////////////////////////////////

    KeyType::KeyType(wchar_t const* ss, size_t ll) 
        : s(ss)
        , l(ll)
    {

    }

    KeyType::KeyType(KeyType const& src) 
        : s(src.s)
        , l(src.l)
    {

    }

    //////////////////////////////////////////////////////////////////////////

    KeyType::KeyType(XParamImpl const* ximpl) 
        : s(ximpl->name().data())
        , l(ximpl->name().length())
    {

    }

    bool FunctorLT::operator()
        ( XParamPtr const& p1
        , XParamPtr const& p2) const
    {
        const wchar_t* data = p2->name().data();
        size_t len = p2->name().length();
        return p1->name().compare(data , len) < 0;
    }

    bool FunctorLTK::operator()
        ( KeyType const& key1
        , KeyType const& key2) const
    {
        return xstring<wchar_t>::compare(key1.s
            , key1.l
            , key2.s
            , key2.l) < 0;
    }

    bool FunctorLTK::operator()
        (XParamPtr const& elem 
        , KeyType const& key) const
    {
        return elem->name().compare(key.s, key.l) < 0;
    }

    bool FunctorLTK::operator()
        (KeyType const& elem 
        , XParamPtr const& key) const
    {
        return key->name().compare(elem.s,elem.l) > 0;
    }

    bool FunctorEK::operator()(XParamPtr const& elem 
        , KeyType const& key) const
    {
        return elem->name().compare(key.s, key.l) == 0;
    }

    //////////////////////////////////////////////////////////////////////////
} // namespace xparam
