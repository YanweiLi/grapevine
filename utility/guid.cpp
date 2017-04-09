//#include "utility/common/guid.h"
//#include "utility/string/format.h"
//
//CGUID::CGUID()
//{
//    boost::uuids::random_generator rgen;
//    m_guid = rgen();
//}
//
//CGUID::CGUID(CGUID const& src)
//: m_guid(src.m_guid)
//{
//    
//}
//
//CGUID::CGUID(GUID const& src)
//: m_guid(src)
//{
//    
//}
//
//CGUID& CGUID::operator=(CGUID const& rhs)
//{
//    if (this != &rhs)
//    {
//        m_guid = rhs.m_guid;
//    }
//    
//    return *this;
//}
//
//CGUID& CGUID::operator=(GUID const& rhs)
//{
//    if (&m_guid != &rhs)
//    {
//        m_guid = rhs;
//    }
//    
//    return *this;
//}
//
//bool CGUID::operator==(CGUID const& rhs) const
//{
//    return (m_guid == rhs.m_guid) != 0;
//}
//
//bool CGUID::operator==(GUID const& rhs) const
//{
//    return (m_guid == rhs) != 0;
//}
//
//bool CGUID::operator!=(CGUID const& rhs) const
//{
//    return (m_guid != rhs.m_guid) != 0;
//}
//
//bool CGUID::operator!=(GUID const& rhs) const
//{
//    return (m_guid != rhs) != 0;
//}
//
//bool CGUID::operator<(CGUID const& rhs) const
//{
//    return memcmp(&m_guid, &rhs.m_guid, sizeof(GUID)) < 0;
//}
//
//bool CGUID::operator<(GUID const& rhs) const
//{
//    return memcmp(&m_guid, &rhs, sizeof(GUID)) < 0;
//}
//
//bool CGUID::is_valid() const
//{
//    const long* p = (const long*)&m_guid;
//    const long* end = p + sizeof(GUID) / sizeof(long);
//    while (p < end)
//    {
//        if (*p != 0)
//        {
//            return true;
//        }
//        
//        ++p;
//    }
//    
//    return false;
//}
//
//GUID* CGUID::address()
//{
//    return &m_guid;
//}
//
//const GUID* CGUID::address() const
//{
//    return &m_guid;
//}
//
//tString CGUID::new_string()
//{
//    tString tmp;
//    
//    boost::uuids::random_generator rgen;
//    GUID guid = rgen();
//    if (!guid.is_nil())
//    {
//#ifdef UNICODE
//        tmp = boost::uuids::to_wstring(guid);
//#else
//        tmp = boost::uuids::to_string(guid);
//#endif
//    }
//    
//    return boost::move(tmp);
//}
//
