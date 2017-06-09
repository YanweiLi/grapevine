/**
 *	@file	str_conv.h
 *	@date	2006-7-15	0:01:53
 *	@author	shaoyoushi
 *	@brief	提供一些字符串和其他类型的转换函数，改善了C标准库的接口或者效率
 */
#ifndef __STR_CONV_H_C98AC0D7_6DBF_4a59_81CB_345963291831__
#define __STR_CONV_H_C98AC0D7_6DBF_4a59_81CB_345963291831__
#include <math.h>
#include <wchar.h>
#include <stdlib.h>

namespace tcfl{


/**
 *	@fn			easy_atoi
 *	@brief		将字符串转成整型，支持各种字符类型和整型
 *	@param		s	源字符串开始位置，如果end < s，则查找0结束符 
 *	@param		i	目标整型，字符串s被转换成功后，存到i中，如果转换失败，则不修改i
 *	@param		end	源字符串结束位置，如果end < s，则查找0结束符
 *	@return		是否成功，如果字符串不是一个合法的整数，则返回false，且不修改参数i的值，否则返回true，并将转换后的结果存入i
 *
 *				这个函数支持各种字符类型和整型，合法的整数格式是指以c语言的整型格式，支持10进制和16进制；数字前面可以
 *				有正负符号；16进制以x或者0x或者更多的0接上一个x开始，x可以被$替代(pascal)；字符串中出现的任何空格都会被简单忽略。
 *				与标准库相比，本函数使用模板支持多种类型，并支持返回成功还是失败，而且经过测试，其性能大约是标准库的atoi
 *				算法的3倍。
 *  @exception	sss
 */
template <typename CharType, typename IntType>
bool easy_atoi(const CharType* s, IntType& i, const CharType* end = NULL)
{
	IntType result = 0;

	if (s == NULL || s == end || *s == 0)
		return false;

	bool positive = true;
	IntType base = 10;

	const CharType* p = s;
	CharType c;
	bool succeed = false;
	do
	{
		c = *p;
		if (c >= 0x30 && c <= 0x39) // numbers
		{
			result = result * base + (c - 0x30);
		}
		else if(c >= 0x61 && c <= 0x66) // a - f
		{
			if (base != 16)
				break;
			result = result * base + (c - (0x61 - 0xa));
		}
		else if (c >= 0x41 && c <= 0x46) // A - F
		{
			if (base != 16)
				break;
			result = result * base + (c - (0x41 - 0xA));
		}
		else if (c == 0x58 || c == 0x78 || c == 0x24) // X or x or $
		{
			if (result || base != 10)
				break;
			base = 16;
		}
		else if (c == 0x20 || c == 0x9) // space or tab
			continue;
		else if (c == 0)
		{
			succeed = true;
			break;
		}
		else if (c == 0x2d) // minus
		{
			if (result)
				break;
			positive = false;
		}
		else if (c == 0x2b) // plus
		{
			if (result || !positive)
				break;
		}
		else
			break;
	}
	while (++p != end);

	if (succeed || p == end)
	{
		i = positive ? result : 0 - result;
		return true;
	}
	return false;
}


/**
*	@fn			easy_atof
*	@brief		将字符串转成浮点型，支持各种字符类型和浮点型
*	@param		s	源字符串开始位置，如果end < s，则查找0结束符
*	@param		f	目标浮点型，字符串s被转换成功后，存到f中，如果转换失败，则不修改f
*	@param		end	源字符串结束位置，如果end < s，则查找0结束符
*	@return		是否成功，如果字符串不是一个合法的浮点数，则返回false，且不修改参数f的值，否则返回true，并将转换后的结果存入f
*
*				这个函数支持各种字符类型和浮点型，合法的浮点格式是指以c语言的浮点型格式，数字前面可以
*				有正负符号；字符串中出现的任何空格都会被简单忽略。
*				与标准库相比，本函数使用模板支持多种类型，并支持返回成功还是失败，但不支持科学计数法格式，
*				只能识别通常格式的浮点数字，经过测试，其性能大约是标准库的atof算法的6倍。
*  @exception	sss
*/

template <typename CharType, typename FloatType>
bool easy_atof(const CharType* s, FloatType& f, const CharType* end = NULL)
{
	FloatType result = 0;

	if (s == NULL || s == end || *s == 0)
		return false;

	bool positive = true;
	bool before_dot = true;
	bool succeed = false;
	FloatType base = 10.0;

	const CharType* p = s;
	CharType c;
	do
	{
		c = *p;
		if (c >= 0x30 && c <= 0x39) // numbers
		{
			if (before_dot)
				result = result * base + (c - 0x30);
			else
				result += (base /= 10.0) * (c - 0x30);
		}
		else if (c == 0x20 || c == 0x9) // space or tab
			continue;
		else if (c == 0x2e) // dot
		{
			if (!before_dot)
				break;
			before_dot = false;
			base = 1.0;
		}
		else if (c == 0)
		{
			succeed = true;
			break;
		}
		else if (c == 0x2d) // minus
		{
			if (result > 0 || result < 0)
				break;
			positive = false;
		}
		else if (c == 0x2b) // plus
		{
			if (result > 0 || result < 0 || !positive)
				break;
		}
		else
			break;
	} while (++p != end);

	if (succeed || p == end)
	{
		f = positive ? result : -result;
		return true;
	}
	return false;
}


/**
*	@fn			easy_itoa
*	@brief		将整型转成字符串，支持各种字符类型和整型
*	@param		i	源整数
*	@param		s	目标字符串，i被转换成功后，存到字符串s中，如果转换失败，则不修改s，本函数不会给s添加0结束符
*	@param		len 字符串内存的大小，单位是字符数。
*	@param		base	表明是使用10进制还是16进制进行转换，目前只支持10和16两个值
*	@return		整数i转换成s，所需要的内存大小，不包括0结束符，如果返回值大于参数len，说明有一些数字没有存入s中，
*				在这种情况下，存储不下的高位数字会被丢弃。如果返回为0，则表示有错误发生，通常是参数不正确。
*
*				这个函数支持各种字符类型和整型，不支持转成带正负号的16进制格式，
*				与标准库相比，本函数使用模板支持多种类型，并支持返回所需要空间，其性能和标准库的itoa算法基本相同。
*/
template <typename CharType, typename IntType>
size_t easy_itoa(IntType i, CharType* s, size_t len, unsigned int base)
{
	if ((base != 10 && base != 16) || (i < 0 && base == 16))
		return 0;

	if (s == NULL)
		len = 0;

	size_t result = 0;
	IntType val;

	// if negative
	if (i < 0)
	{
		val = 0 - i;
		if (len > 1)
		{
			*s++ = 0x2d; // minus
			--len;
		}
		++result;
	}
	else
		val = i;
	IntType digival;
	CharType* p = s;
	CharType* end = s + len;

	do
	{
		++ result;
		digival = val % base;
		val /= base;
		if (p != end)
		{
			CharType digiChar = static_cast<CharType>
				(digival > 9 ? digival + (0x61 - 0xa) : digival + 0x30);
			*p++ = digiChar;
		}
	} while (val);

	// reverse string
	size_t real_len = p - s;  // exact length
	CharType* mid = s + real_len / 2;
	CharType* left = s;
	CharType* right = p - 1;
	while (left < mid)
	{
		CharType t = *left;
		*left = *right;
		*right = t;
		++left;
		--right;
	}
	return result;
}

/**
*	@fn			easy_ftoa
*	@brief		将浮点数转成字符串，支持各种字符类型和浮点型
*	@param		f	源浮点数，整数部分和小数部分均不能超过9位有效数字，小数部分超出部分将被丢弃，整数部超出将导致溢出。
*	@param		s	目标字符串，i被转换成功后，存到字符串s中，如果转换失败，则不修改s，本函数不会给s添加0结束符
*	@param		len 字符串内存的大小，单位是字符数。
*	@return		浮点f转换成s，所需要的内存大小，不包括0结束符，如果返回值大于参数len，说明有一些数字没有存入s中。
*
*				与标准库相比，本函数使用模板支持多种类型，并支持返回所需要空间，其性能是标准库的ftoa算法的7倍。
*/
template <typename CharType, typename FloatType>
size_t easy_ftoa(FloatType f, CharType* s, size_t len)
{
	if (s == NULL)
		len = 0;

	FloatType fractional;
	FloatType integer;
    
	fractional = modfl((long double)f, (long double*)&integer);
    if (fractional < 0)
    {
		fractional = 0 - fractional;
    }
	
    size_t result = easy_itoa((long)integer, s, len, 10);
	s += result;

	const static long digi = 1000000000;
	long lfra = long(fractional * digi);
	if (lfra != 0)
	{
		if (len > result)
        {
			*s++ = 0x2e; // dot
        }
        
		++result;

		for (long _div = digi / 10; _div && lfra / _div == 0; _div /= 10)
		{
			if (len > result)
            {
				*s++ = 0x30; //'0'
            }
            
			++result;
		}
        
		while (lfra % 10 == 0)
        {
			lfra /= 10;
        }
        
		result += easy_itoa(lfra, s, len > result ? len - result : 0, 10);
	}
    
	return result;
}


//其实上面那几个模板是王道，但是...
inline int safe_wtoi(const wchar_t *str)
{
	if(str == NULL)
	{
		return 0;
	}
    
    wchar_t* end;
    size_t len = wcslen(str);
	return (int)wcstol(str , &end , (int)len);
}

inline long safe_wtol(const wchar_t *str)
{
	if(str == NULL)
	{
		return 0;
	}
    
    wchar_t* end;
    size_t len = wcslen(str);
    return wcstol(str , &end , (int)len);
}

inline int safe_atoi(const char *str)
{
	if(str == NULL)
	{
		return 0;
	}
	return atoi(str);
}

inline long safe_atol(const char *str)
{
	if(str == NULL)
	{
		return 0;
	}
	return atol(str);
}

}
#endif//__STR_CONV_H_C98AC0D7_6DBF_4a59_81CB_345963291831__
