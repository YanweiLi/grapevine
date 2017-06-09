#ifndef SRC_XPARAM_MODEL_MARCODEF_H_
#define SRC_XPARAM_MODEL_MARCODEF_H_

#ifdef WIN32

// win32
#ifdef XPARAM_EXPORTS
#define XPARAM_API __declspec(dllexport)
#else
#define XPARAM_API __declspec(dllimport)
#endif

#ifndef __IUnknown_INTERFACE_DEFINED__
struct IUnknown;
#endif
#ifndef __IDispatch_INTERFACE_DEFINED__
struct IDispatch;
#endif


#else

// macOS iOS
#define XPARAM_API


#endif

namespace xparam
{
    enum {ZERO_TERMINATOR = -1};
    
    enum X_VALUE_TYPE
    {
        xvt_empty = 0,
        xvt_beginof_basic = 0x10,
        xvt_char,
        xvt_uchar,
        xvt_short,
        xvt_ushort,
        xvt_long,
        xvt_ulong,
        xvt_longlong,
        xvt_ulonglong,
        xvt_int,
        xvt_uint,
        xvt_float,
        xvt_double,
        xvt_longdouble,
        xvt_bool,
        xvt_endof_basic = 0x2f,
        
        xvt_beginof_pointer = 0x30,
        xvt_pvoid,
        xvt_sp,
        xvt_iunknown,
        xvt_idispatch,
        xvt_endof_pointer = 0x4f,
        
        xvt_string,
        xvt_blob,
    };
    
    enum X_VALUE_NODE
    {
        xvn_text,
        xvn_cdata,
        xvn_attr
    };
    
    enum X_PARSE_ERRORS
    {
        xpe_no_error                = 0,    // 正确，没有错误
        xpe_unknown                 = 1,    // 未知错误
        xpe_callback_cancel,                // 解析过程被回调函数取消
        xpe_null_root,                      // 没有根节点，通常是因为没有调用BeginParse，或者在解析过程中调用了DetachRoot
        xpe_need_initialize,                // 调用Parsing前没有调用BeginParse
        xec_unescape_failed,                // 反转义失败，通常是读到了错误的转义编码
        xec_unexpected_char,                // 读到非法字符
        xec_unmatched_tag,                  // 标签不匹配
        xec_too_much_doc_element,           // 文档元素太多，xml规范规定每个xml文档只能有一个文档元素
        xec_decode_failed,                  // 解码失败
        xec_cannot_switch_coding,           // 无法切换编码
        xec_mssing_tail,                    // 找不到文档末尾的结束标签
        xec_read_file_failed,               // 读取文件错误
        xec_write_file_failed,              // 写文件错误
        xec_not_supported                   // 不支持的操作
    };
    
} // namespace xparam


#endif // SRC_XPARAM_MODEL_MARCODEF_H_
