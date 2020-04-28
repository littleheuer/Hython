/*************************************************************************
	> File Name: haizei_visitor.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 20:08:54 2020
 ************************************************************************/

#ifndef _HAIZEI_VISITOR_H
#define _HAIZEI_VISITOR_H

#include <vector>
#include <haizei_type.h>

namespace haizei {

class ListVisitor : public IValue::IVisitor {
public :
    ListVisitor(std::vector<int>);
    void visit(IntValue *);
    void visit(FloatValue *);
    void visit(StringValue *);
    void visit(FunctionValue *);
    void visit(ListValue *);
    std::vector<int> index;
    SIValue result;
};
    
//输出类型的值的字符串信息
class ConvertStringVisitor : public IValue::IVisitor {
public :
    void visit(IntValue *);
    void visit(FloatValue *);
    void visit(StringValue *);
    void visit(FunctionValue *);
    void visit(ListValue *);
    std::string result;
};

// value Operator
// 运算符操作的基类
// 类型相加错误报错的位置
class ValueOperator : public IValue::IVisitor {
public :
    //op_type为成员方法指针类型
    //返回void，传入IValue&
    typedef void (IValue::*op_type)(IValue &);
    ValueOperator(IValue *, op_type);
    
    //用于类型运算报错
    void visit(IntValue *);
    void visit(FloatValue *);
    void visit(StringValue *);
    void visit(FunctionValue *);
    void visit(ListValue *);
    //op为成员指针变量
    //指向IValue中用于报错的成员函数
    op_type op;
    //用于调用IValue对象中的报错函数
    IValue *lvalue;
    //存运算结果
    SIValue result;
};

// type value Operator
// 运算操作的基类
// 用于存储运算符左边的值
class IntValueOperator : public ValueOperator {
public :
    IntValueOperator(IntValue *, op_type);
    //运算符左边的值
    IntValue *left;
};

class FloatValueOperator : public ValueOperator {
public :
    FloatValueOperator(FloatValue *, op_type);
    FloatValue *left;
};

class StringValueOperator : public ValueOperator {
public :
    StringValueOperator(StringValue *, op_type);
    StringValue *left;
};

class ListValueOperator : public ValueOperator {
public :
    ListValueOperator(ListValue *, op_type);
    ListValue *left;
};

// IntValue Operator visitor
// IntValue类型运算符操作的访问者
class IntValuePlusOpVisitor : public IntValueOperator {
public :
    //传入左值
    IntValuePlusOpVisitor(IntValue *);
    //重载visit()，传入运算符右边的值
    void visit(IntValue *);
    void visit(FloatValue *);
};

class IntValueMinusOpVisitor : public IntValueOperator {
public :
    IntValueMinusOpVisitor(IntValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
};

class IntValueTimesOpVisitor : public IntValueOperator {
public :
    IntValueTimesOpVisitor(IntValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
};

class IntValueLittleOpVisitor : public IntValueOperator {
public :
    IntValueLittleOpVisitor(IntValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
};

class IntValueModOpVisitor : public IntValueOperator {
public :
    IntValueModOpVisitor(IntValue *);
    void visit(IntValue *);
};

// FloatValue Operator visitor
class FloatValuePlusOpVisitor : public FloatValueOperator {
public :
    FloatValuePlusOpVisitor(FloatValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
};

class FloatValueMinusOpVisitor : public FloatValueOperator {
public :
    FloatValueMinusOpVisitor(FloatValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
};

class FloatValueTimesOpVisitor : public FloatValueOperator {
public :
    FloatValueTimesOpVisitor(FloatValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
};

class FloatValueLittleOpVisitor : public FloatValueOperator {
public :
    FloatValueLittleOpVisitor(FloatValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
};

// StringValue Operator visitor
class StringValuePlusOpVisitor : public StringValueOperator {
public :
    StringValuePlusOpVisitor(StringValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
    void visit(StringValue *);
};

class StringValueTimesOpVisitor : public StringValueOperator {
public :
    StringValueTimesOpVisitor(StringValue *);
    void visit(IntValue *);
};

class StringValueLittleOpVisitor : public StringValueOperator {
public :
    StringValueLittleOpVisitor(StringValue *);
    void visit(StringValue *);
};

//list
class ListValuePlusOpVisitor : public ListValueOperator {
public :
    ListValuePlusOpVisitor(ListValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
    void visit(StringValue *);
    void visit(ListValue *);
};

class ListValueMinusOpVisitor : public ListValueOperator {
public :
    ListValueMinusOpVisitor(ListValue *);
    void visit(IntValue *);
    void visit(FloatValue *);
    void visit(StringValue *);
    void visit(ListValue *);
};



}
#endif
