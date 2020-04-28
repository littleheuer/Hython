/*************************************************************************
	> File Name: haizei_util.h
	> Author: 
	> Mail: 
	> Created Time: 六  4/ 4 14:28:51 2020
 ************************************************************************/

#ifndef _HAIZEI_UTIL_H
#define _HAIZEI_UTIL_H
#include <string>

namespace haizei {
    //用于处理字符串中的转义字符
    //为什么会出现这个问题?
    //
    //在string中\n是存成两个字符的
    //而实际上\n在c语言中是占一个字符的
    //所以要进行转义
    std::string ConvertStringToCString(const std::string &);

}

#endif
