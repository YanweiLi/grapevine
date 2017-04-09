#if 0//ndef CPP_0X_COMMON_GUID_H_
#define CPP_0X_COMMON_GUID_H_

#include <uuid/uuids.h>

typedef boost::uuids::uuid GUID;

class CGUID
{
public:
    CGUID ();
    CGUID (CGUID const& src);
    CGUID (GUID const& src);
    
    CGUID& operator = (CGUID const& rhs);
    CGUID& operator = (GUID const& rhs);
    bool   operator ==(CGUID const& rhs) const;;
    bool   operator ==(GUID const& rhs) const;
    bool   operator !=(CGUID const& rhs) const;
    bool   operator !=(GUID const& rhs) const;
    bool   operator < (CGUID const& rhs) const;
    bool   operator < (GUID const& rhs) const;
    
    operator GUID const&() const {return m_guid;}
    operator GUID& () {return m_guid;}
    
    bool is_valid() const;
    GUID* address();
    const GUID* address() const;
    static tString new_string();
    
private:
    GUID m_guid;
};


NS_SERIALIZATION_LITE_BEGIN

// CGuid–Ú¡–ªØ
inline void serialize_load(srlz::iarchive_xparam const& ar, CGUID& t)
{
    if (ar.exists())
    {
        size_t vlen = 0;
        xparam::XInResPtr inres;
        tchar const* v = {0};
        if (ar.value(v, &vlen, inres))
        {
            //::CLSIDFromString(const_cast<tchar>(v), t.address());
        }
    }
}

inline void serialize_save(srlz::oarchive_xparam& ar, CGUID const& t)
{
    static const int cchMax = 64;
    tchar buf[cchMax];
    //int ret = ::StringFromGUID2(GUID(t), buf, cchMax);
    ar.value(buf, -1);
}

NS_SERIALIZATION_LITE_END

#endif // CPP_0X_COMMON_GUID_H_
