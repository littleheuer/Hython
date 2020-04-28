/*************************************************************************
	> File Name: parameters.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 16:28:05 2020
 ************************************************************************/

#ifndef _PARAMETER_H
#define _PARAMETER_H

#include <map>
#include <string>
#include <haizei_shared_type.h>

namespace haizei {

class Parameter {
public :
    Parameter(SParameter next, int = -1);
    //定义变量
    void define_param(std::string);
    //获取变量的值
    SIValue get(std::string) const;
    //设置变量的值
    void set(std::string, SIValue);
    //获取父类作用域
    SParameter next() const;
    int pos();

private:
    //作用域内变量存储
    mutable std::map<std::string, SIValue> memory;
    //指向父作用域
    SParameter __next;
    int __pos;
};

}
#endif
