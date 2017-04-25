//
//  string_def.h
//  grapevine
//
//  Created by li_yanwei on 2016/10/27.
//  Copyright © 2016年 li_yanwei. All rights reserved.
//

#ifndef CPP_0X_STRING_STRING_DEF_H_
#define CPP_0X_STRING_STRING_DEF_H_

// 使用UNICODE
// #define UNICODE

#include <string>
#include <wchar.h>

#ifdef UNICODE
#define __T(x)                      L ## x
#define _T(x)                       __T(x)
typedef wchar_t                     tchar;
#else
#define _T(xxx)                     xxx
typedef char                        tchar;
#endif

// 字符串
typedef tchar*                      lptchar;
typedef std::basic_string<tchar>    tString;


//

#endif /* CPP_0X_STRING_STRING_DEF_H_ */
