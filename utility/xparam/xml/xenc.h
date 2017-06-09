#ifndef SRC_UTILITY_XPARAM_XML_XEHC_H_
#define SRC_UTILITY_XPARAM_XML_XEHC_H_
#include "utility/string/convert.h"
namespace xenc
{
    // 判断本机的字节序
    inline bool is_big_endian()
    {
        static const short word = 0x1234;
        return reinterpret_cast<const char*>(&word)[0] == 0x12;
    }
    
    // 匹配字符串，给detect_xml_encoding调用
    inline const char* match_str(const char* begin
                                 , const char* end
                                 , const char* str)
    {
        // skip space chars
        const char* p = begin;
        while( p < end && (*p == ' ' || *p == '\t') )
            ++p;
        
        // match content
        const char* str_p = str;
        while ( (p < end) && *str_p )
        {
            if (*p != *str_p)	// matching failed
                return NULL;
            ++p;
            ++str_p;
        }
        return p;
    }
    
    // 检测xml的编码
    inline bool detect_xml_encoding(const void* data
                                    , size_t size
                                    , char* encoding
                                    , size_t len
                                    , size_t* prefix_size)
    {
        // 长度太短，无法判断，
        // 或者encoding的内存太小，无法写入
        if (size < 4
            || encoding == NULL
            || len == 0)
        {
            return false;
        }
        
        memset(encoding, 0, len);
        if (prefix_size)
        {
            *prefix_size = 0;
        }
        
        // 先看看是否能够通过头两个
        // 字节来判断是否是utf16
        const unsigned short* pUShort =
        reinterpret_cast<const unsigned short*>(data);
        switch(*pUShort)
        {
            case 0xfeff:
                if (prefix_size){*prefix_size = 2;}
            case 0x003c:
                // UTF-16LE
                strncpy(encoding, "utf-16le", len - 1);
                return true;
            case 0xfffe:
                if (prefix_size){*prefix_size = 2;}
            case 0x3c00:
                // UTF-16BE
                strncpy(encoding, "utf-16be", len - 1);
                return true;
            default:
                break;
        }
        
        // 看看能否通过头4个字节判断是utf8
        const unsigned long* pULong
        = reinterpret_cast<const unsigned long*>(data);
        if (*pULong == 0x3cbfbbef)
        {
            // UTF-8
            if (prefix_size){*prefix_size = 3;}
            strncpy(encoding, "utf-8", len - 1);
            return true;
        }
        
        const char* pChar = reinterpret_cast<const char*>(data);
        const char* pEnd = pChar + size;
        
        //////////////////////////////////////////////////////////////////////////
        
#define __DEAL_MATCHING_RESULT(x)											\
do {                                                                        \
if (x == NULL){return true;}                                                \
if (x == pEnd){return false;}                                               \
} while (0)
        
        //////////////////////////////////////////////////////////////////////////
        
        // 匹配<?xml
        pChar = match_str(pChar, pEnd, "<?xml");
        __DEAL_MATCHING_RESULT(pChar);
        
        // 必须有个空格
        if (pChar == pEnd){return false;}
        if (*pChar != ' ' && *pChar != '\t'){return true;}
        ++pChar;
        
        // 匹配version
        pChar = match_str(pChar, pEnd, "version");
        __DEAL_MATCHING_RESULT(pChar);
        
        // 匹配=
        pChar = match_str(pChar, pEnd, "=");
        __DEAL_MATCHING_RESULT(pChar);
        
        // 匹配"1.0"或者'1.0'
        const char* p = match_str(pChar, pEnd, "\"1.0\"");
        if (p == NULL){p = match_str(pChar, pEnd, "'1.0'");}
        pChar = p;
        __DEAL_MATCHING_RESULT(pChar);
        
        // 必须有个空格
        if (pChar == pEnd){return false;}
        if (*pChar != ' ' && *pChar != '\t'){return true;}
        ++pChar;
        
        // 匹配encoding
        pChar = match_str(pChar, pEnd, "encoding");
        __DEAL_MATCHING_RESULT(pChar);
        
        // 匹配=
        pChar = match_str(pChar, pEnd, "=");
        __DEAL_MATCHING_RESULT(pChar);
        
        // 匹配"或者'
        p = match_str(pChar, pEnd, "\"");
        if (p == NULL){p = match_str(pChar, pEnd, "'");}
        pChar = p;
        __DEAL_MATCHING_RESULT(pChar);
        
        // 程序运行到此处，说明很可能存在编码的指令。获得其内容
        p = pChar;
        for (; p < pEnd; ++ p)
        {
            if ( (*p >= 'A' && *p <= 'Z')
                || (*p >= 'a' && *p <= 'z')
                || (*p >= '0' && *p <= '9')
                || *p == '-'
                || *p == '_' ) //合法字符
            {
                continue;
            }
            else
            {
                break;
            }
        }
        if (p == pEnd){return false;}
        if (*p != '"' && *p != '\''){return true;}
        
        // 到这里，可以肯定我们已经能拿到encoding了
        // ，拷贝它到encoding变量，并转化成小写
        size_t i = 0;
        for (; i < len - 1 && pChar < p; ++pChar, ++i)
        {
            if (*pChar >= 'A' && *pChar <= 'Z')
            {
                encoding[i] = *pChar + ('a' - 'A');
            }
            else
            {
                encoding[i] = *pChar;
            }
        }
        
        return true;
#undef __DEAL_MATCHING_RESULT
    }
    
    enum
    {
        CP_ACP = 65001
        , CP_UTF8 = 65001
    };
    
    static const int enc_count = 20;
    static const char enc_table[enc_count][32] =
    {
        "",					// 65001
        "utf-8",			// 65001
        "utf8",				// 65001
        "utf-16le",			// 1200
        "utf16le",			// 1200
        "utf-16",			// 1200
        "utf16",			// 1200
        "unicode",			// 1200
        "utf-16be",			// 1201
        "utf16be",			// 1201
        "unicodefffe",		// 1201
        "gb2312",			// 936
        "gbk",				// 936
        "x-cp20936",		// 20936
        "hz-gb-2312",		// 52936
        "gb18030",			// 54936
        "big5",				// 950
        "shift_jis",		// 932
        "ks_c_5601-1987",	// 949
        "us-ascii"			// 20127
    };
    
    static const unsigned int cp_table[enc_count] =
    {
        65001,
        65001,
        65001,
        1200,
        1200,
        1200,
        1200,
        1200,
        1201,
        1201,
        1201,
        936,
        936,
        20936,
        52936,
        54936,
        950,
        932,
        949,
        20127
    };
    
    inline unsigned int enc2cp(const char* enc)
    {
        for (int i = 0; i < enc_count; ++i)
        {
            if (strcmp(enc, enc_table[i]) == 0){return cp_table[i];}
        }
        
        return CP_ACP;
    }
    
    inline bool w2a(const wchar_t* s
                    , size_t slen
                    , unsigned int dcp
                    , char*& d
                    , size_t& dlen)
    {
        std::string tmp = String::Convert<wchar_t , char>::to_string(s);
        if (!tmp.empty())
        {
            d = (char*)::operator new(tmp.length());
            memcpy(d, tmp.c_str(), tmp.length());
            dlen = tmp.length();
            return true;
        }
        
        return false;
    }
    
    inline bool a2w(unsigned int scp
                    , const char* s
                    , size_t slen
                    , wchar_t*& d
                    , size_t& dlen)
    {
        std::wstring tmp = String::Convert<char , wchar_t>::to_wstring(s);
        if (!tmp.empty())
        {
            d = (wchar_t*)::operator new(tmp.length());
            wmemcpy(d, tmp.c_str(), tmp.length());
            dlen = tmp.length();
            return true;
        }
        
        return false;
    }
    
    inline void change_byte_order(const wchar_t* s
                                  , size_t slen
                                  , wchar_t*& d
                                  , size_t& dlen)
    {
        dlen = slen;
        d = (wchar_t*)::operator new(dlen * sizeof(wchar_t));
        const unsigned char* ps = reinterpret_cast<const unsigned char*>(s);
        unsigned char* pd = reinterpret_cast<unsigned char*>(d);
        size_t count = slen * 2;
        for (size_t i = 0; i < count; i += 2)
        {
            pd[i] = ps[i + 1];
            pd[i + 1] = ps[i];
        }
    }
    
    template <typename CH>
    inline void normalize_encode(const CH* enc, char (&encoding)[32])
    {
        encoding[0] = 0;
        if (enc)
        {
            char* p = encoding;
            while (*enc && p < encoding + 31)
            {
                if (*enc >= 'A' && *enc <= 'Z')
                    *p = static_cast<char>(*enc + ('a' - 'A'));
                else
                    *p = (char)*enc;
                ++ enc;
                ++ p;
            }
            *p = 0;
        }
    }
    
    inline void getbom(const void** bom
                       , size_t* bom_size
                       , unsigned int cp)
    {
        if (cp == CP_UTF8)
        {
            static const unsigned char BOM[4] = {0xef, 0xbb, 0xbf};
            if (bom){*bom = BOM;}
            if (bom_size){*bom_size = 3;}
        }
        else if (cp == 1200)
        {
            static const unsigned char BOM[4] = {0xff, 0xfe};
            if (bom){*bom = BOM;}
            if (bom_size){*bom_size = 2;}
        }
        else if (cp == 1201)
        {
            static const unsigned char BOM[4] = {0xfe, 0xff};
            if (bom){*bom = BOM;}
            if (bom_size){*bom_size = 2;}
        }
        else
        {
            static const unsigned char BOM[4] = {0};
            if (bom){*bom = BOM;}
            if (bom_size){*bom_size = 0;}
        }
    }
    
    class decoder
    {
    public:
        inline decoder() : m_wstr(NULL), m_cp(0){}
        inline ~decoder() {::operator delete(m_wstr);}
        inline bool begin(const void* src
                          , size_t src_size
                          , const wchar_t*& dest
                          , size_t& dest_len)
        {
            dest = NULL;
            dest_len = 0;
            size_t prefix_size;
            bool bRet = detect_xml_encoding(src
                                            , src_size
                                            , m_enc
                                            , enc_len
                                            , &prefix_size);
            if (!bRet){return false;}
            
            m_cp = enc2cp(m_enc);
            return continue_doing((const char*)src + prefix_size
                                  , src_size - prefix_size
                                  , dest
                                  , dest_len);
        }
        
        inline bool continue_doing(const void* src
                                   , size_t src_size
                                   , const wchar_t*& dest
                                   , size_t& dest_len)
        {
            if ( (m_cp == 1200
                  && !is_big_endian())
                || (m_cp == 1201
                    && is_big_endian()) )
            {
                dest_len = src_size / sizeof(wchar_t);
                dest = reinterpret_cast<const wchar_t*>(src);
                return true;
            }
            
            dest = NULL;
            dest_len = 0;
            if (m_wstr)
            {
                ::operator delete(m_wstr);
                m_wstr = NULL;
            }
            
            if ( (m_cp == 1200 && is_big_endian())
                || (m_cp == 1201 && !is_big_endian()) )
            {
                change_byte_order((const wchar_t*)src
                                  , src_size / sizeof(wchar_t)
                                  , m_wstr
                                  , dest_len);
                dest = m_wstr;
                return true;
            }
            
            bool bret = a2w(m_cp
                            , (const char*)src
                            , src_size
                            , m_wstr
                            , dest_len);
            dest = m_wstr;
            return bret;
        }
        
    private:
        wchar_t* m_wstr;
        enum {enc_len = 32};
        char m_enc[enc_len];
        unsigned int m_cp;
        decoder& operator=(const decoder&);
    };
    
    class encoder
    {
    public:
        inline encoder() : m_data(NULL), m_cp(0){}
        inline ~encoder() {::operator delete(m_data);}
        inline bool begin(const wchar_t* enc
                          , const wchar_t*src
                          , size_t src_len
                          , const void*& dest
                          , size_t& dest_size
                          , const void** bom
                          , size_t* bom_size)
        {
            char encoding[32] = {0};
            normalize_encode(enc, encoding);
            m_cp = enc2cp(encoding);
            getbom(bom, bom_size, m_cp);
            return continue_doing(src, src_len, dest, dest_size);
        }
        
        inline bool continue_doing(const wchar_t*src
                                   , size_t src_len
                                   , const void*& dest
                                   , size_t& dest_size)
        {
            if ( (m_cp == 1200
                  && !is_big_endian())
                || (m_cp == 1201
                    && is_big_endian()) )
            {
                dest = reinterpret_cast<const void*>(src);
                dest_size = src_len * sizeof(wchar_t);
                return true;
            }
            
            dest = NULL;
            dest_size = 0;
            if (m_data)
            {
                ::operator delete(m_data);
                m_data = NULL;
            }
            
            if ( (m_cp == 1200
                  && is_big_endian())
                || (m_cp == 1201
                    && !is_big_endian()) )
            {
                wchar_t* wstr;
                size_t wlen;
                change_byte_order(src, src_len, wstr, wlen);
                m_data = reinterpret_cast<void*>(wstr);
                dest = m_data;
                dest_size = wlen * sizeof(wchar_t);
                return true;
            }
            
            char* d;
            size_t dlen;
            if (w2a(src, src_len, m_cp, d, dlen))
            {
                m_data = reinterpret_cast<void*>(d);
                dest = m_data;
                dest_size = dlen;
                return true;
            }
            return false;
        }
        
    private:
        void* m_data;
        unsigned int m_cp;
        encoder& operator=(const encoder&);
    };
}

#endif  // SRC_UTILITY_XPARAM_XML_XEHC_H_
