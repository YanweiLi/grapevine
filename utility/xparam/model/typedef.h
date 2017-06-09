#ifndef SRC_XPARAM_MODEL_TYPEDEF_H_
#define SRC_XPARAM_MODEL_TYPEDEF_H_

#include <memory>
#include "utility/xparam/xml/xcontainer.h"

namespace xparam
{
    //////////////////////////////////////////////////////////////////////////

    class XInRes;
    typedef std::shared_ptr<XInRes> XInResPtr;

    //////////////////////////////////////////////////////////////////////////

    class XParam;
    typedef std::shared_ptr<XParam> XParamPtr;
    typedef bool (*CallbackType)(XParamPtr const& node, void* context);
    typedef bool (*InternalCallbackType)(XParamPtr const& node, void* context);

    //////////////////////////////////////////////////////////////////////////

    class XParamImpl;
    typedef std::shared_ptr<XParamImpl> XParamImplPtr;

    //////////////////////////////////////////////////////////////////////////

    class XParamParser;
    typedef std::shared_ptr<XParamParser> XParamParserPtr;

    //////////////////////////////////////////////////////////////////////////

    class XParamTraversal;
    typedef std::shared_ptr<XParamTraversal> XParamTraversalPtr;

    //////////////////////////////////////////////////////////////////////////

    class XParamParserImpl;
    typedef std::shared_ptr<XParamParserImpl> XParamParserImplPtr;

    //////////////////////////////////////////////////////////////////////////

    template <typename T> void call_ctor(T& t){ new(&t) T();}
    template <typename T> void call_dtor(T& t){ t.~T();}

    //////////////////////////////////////////////////////////////////////////

    class XParamImpl;

    struct KeyType
    {
        KeyType(wchar_t const* ss, size_t ll);
        KeyType(KeyType const& src);

        // for VC8's shit bug of debug building;
        KeyType(XParamImpl const* ximpl);

        wchar_t const* s;
        size_t l;
    };

    struct FunctorLT
    {
        bool operator()
            ( XParamPtr const& p1
            , XParamPtr const& p2) const;
    };

    struct FunctorLTK
    {
        bool operator()
            ( KeyType const& key1
            , KeyType const& key2) const;

        bool operator()
            ( XParamPtr const& elem
            , KeyType const& key) const;

        bool operator()
            ( KeyType const& elem  
            , XParamPtr const& key) const;
    };

    struct FunctorEK
    {
        bool operator()
            ( XParamPtr const& elem
            , KeyType const& key) const;
    };

    typedef xcontainer<XParam
        , KeyType
        , FunctorLT
        , FunctorLTK
        , FunctorEK> ChildrenType;
    typedef std::shared_ptr<ChildrenType> ChildrenTypePtr;

    //////////////////////////////////////////////////////////////////////////
} // namespace xparam

#endif //SRC_XPARAM_MODEL_TYPEDEF_H_
