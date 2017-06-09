//
//  grapevine_define.h
//  grapevine
//
//  Created by li_yanwei on 2016/10/27.
//  Copyright © 2016年 li_yanwei. All rights reserved.
//

#ifndef CPP_0X_GRAPEVINE_DEFINE_H_
#define CPP_0X_GRAPEVINE_DEFINE_H_

////////////////////////////////////////////////////////////

//#define WIN32
#ifdef WIN32 //|| WIN64
#define WIN32_PF 1
#else
#define WIN32_PF 0
#endif

////////////////////////////////////////////////////////////

//#define MAC_OS


////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////

#endif /* CPP_0X_GRAPEVINE_DEFINE_H_ */
