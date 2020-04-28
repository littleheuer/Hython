/*************************************************************************
	> File Name: haizei_type.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 18:16:07 2020
 ************************************************************************/

#ifndef _HAIZEI_TYPE_H
#define _HAIZEI_TYPE_H

#include <string>
#include <vector>
#include <haizei_shared_type.h>

namespace haizei {

class IValue {
public :
    IValue();
    IValue(std::string);
    class IVisitor {
    public :
        virtual void visit(IntValue *) = 0;
        virtual void visit(FloatValue *) = 0;
        virtual void visit(StringValue *) = 0;
        virtual void visit(FunctionValue *) = 0;
        virtual void visit(ListValue *) = 0;
    };
    //返回对应类型(string)
    std::string type();
    virtual void accept(IVisitor *) = 0;
    //条件真值
    virtual bool isTrue() = 0;
    virtual bool isFalse();
    virtual int intVal();
    virtual SIValue run(SParameter &, ASTree *);
    virtual SIValue operator+(IValue &);
    virtual SIValue operator-(IValue &);
    virtual SIValue operator*(IValue &);
    virtual SIValue operator%(IValue &);
    virtual bool operator<(IValue &);
    virtual bool operator>(IValue &);
    virtual bool operator<=(IValue &);
    virtual bool operator>=(IValue &);
    virtual bool operator==(IValue &);
    virtual bool operator!=(IValue &);
    void operator_plus_error(IValue &);
    void operator_minus_error(IValue &);
    void operator_times_error(IValue &);
    void operator_mod_error(IValue &);
    void operator_compare_error(IValue &);
    virtual ~IValue();
protected:
    //存类型的字符串信息
    std::string Tname;
};

class IntValue : public IValue {
public :
    IntValue(int = 0);
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual int intVal();
    virtual SIValue operator+(IValue &);
    virtual SIValue operator-(IValue &);
    virtual SIValue operator*(IValue &);
    virtual SIValue operator%(IValue &);
    virtual bool operator<(IValue &);
    int val();
    ~IntValue();

private:
    int __val;
};
class FloatValue : public IValue {
public :
    FloatValue(double = 0.0);
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual SIValue operator+(IValue &);
    virtual SIValue operator-(IValue &);
    virtual SIValue operator*(IValue &);
    virtual bool operator<(IValue &);
    double val();
    ~FloatValue();

private:
    double __val;
};

class StringValue : public IValue {
public :
    StringValue(std::string = "");
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual SIValue operator+(IValue &);
    virtual SIValue operator*(IValue &);
    virtual bool operator<(IValue &);
    std::string val();
    ~StringValue();

private: 
    std::string __val;
};


class FunctionValue : public IValue {
public :
    FunctionValue(ASTree *, DFA *);
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual SIValue run(SParameter &, ASTree *);
    std::string val();
    void set_init_param(SParameter);
    std::string name();
    ~FunctionValue();

private:
    void convert(ASTree *);
    std::string func_name;
    //函数参数列表名存储
    std::vector<std::string> param;
    //存函数定义时的作用域
    SParameter init_param;
    DFA *__val;
};


class ListValue : public IValue {
public :
    ListValue(ASTree *, SParameter);
    ListValue(std::vector<SIValue> );
    virtual void accept(IVisitor *);
    virtual bool isTrue();
    virtual SIValue operator+(IValue &);
    virtual SIValue operator-(IValue &);
    std::vector<SIValue> val();
    ~ListValue();

private: 
    void convert(ASTree *, SParameter);
    std::vector<SIValue> __val;
};





//空值常量
static SIntValue null_val = std::make_shared<IntValue>(0);
//真值常量
static SIntValue True_val = std::make_shared<IntValue>(1);
//假值常量
static SIntValue False_val = std::make_shared<IntValue>(0);
//匿名返回值常量
static std::string ReturnValueName = "172567returnValue";

}
#endif
