/*************************************************************************
	> File Name: haizei_dfa.h
	> Author: 
	> Mail: 
	> Created Time: 日  4/ 5 18:40:20 2020
 ************************************************************************/

#ifndef _HAIZEI_DFA_H
#define _HAIZEI_DFA_H
#include <vector>
#include <haizei_shared_type.h>
#include <stack>

namespace haizei {

class DFA {
public :
    //将ASTree构建成状态机
    DFA(ASTree *);
    //dfa状态机执行方法,传入当前作用域
    void run(SParameter &);

private:
    //dfa构建
    static DFA *build(ASTree *);
    //可跳转节点存储，按作用域依次跳转
    static std::stack<IDFANode *> breakPoint;
    static std::stack<IDFANode *> continuePoint;
    static std::stack<IDFANode *> returnPoint;
    //dfa标记，用于正确退出作用域
    static int blockPosition;
    //防止外部调用空的默认构造函数
    DFA();
    // 状态机头节点和尾节点
    IDFANode *head, *tail;
};

//接口类(基类)
class IDFANode {
public :
    IDFANode(ASTree *, int, std::string);
    //状态转移的方法，返回转移到的下一个状态
    virtual IDFANode *next(SParameter &) = 0;
    //用于转移状态连接
    IDFANode *&at(int);
    std::string type();
protected:
    //当前dfa节点能处理的抽象语法树根节点
    ASTree *tree;
    //存当前节点可以状态转移的子节点
    std::vector<IDFANode *> childs;
    std::string __type;
};

//一个状态转移分支的dfaNode
//接口类
class DFANode_1 : public IDFANode {
public :
    DFANode_1(ASTree *, std::string);
    virtual IDFANode *next(SParameter &) = 0;
};

//两个状态转移分支的dfanode
//接口类
class DFANode_2 : public IDFANode {
public :
    DFANode_2(ASTree *, std::string);
    virtual IDFANode *next(SParameter &) = 0;
};

class ExprDFANode : public DFANode_1 {
public :
    ExprDFANode(ASTree *);
    virtual IDFANode *next(SParameter &);
};

class BlockBeginDFANode : public DFANode_1 {
public :
    BlockBeginDFANode(int);
    virtual IDFANode *next(SParameter &);
    int pos();

private:
    int __pos;
};

class BlockEndDFANode : public DFANode_1 {
public :
    BlockEndDFANode(int);
    virtual IDFANode *next(SParameter &);
    int pos();

private :
    int __pos;
};

//条件判断
class ConditionDFANode : public DFANode_2 {
public :
    ConditionDFANode(ASTree *);
    virtual IDFANode *next(SParameter &);
};

class NopeDFANode : public DFANode_1 {
public :
    //默认-100
    NopeDFANode(int = -100);
    void set_pos(int);
    virtual IDFANode *next(SParameter &);

private:
    int pos;
};

//用于break，continue的跳转
class JumpDFANode : public DFANode_1 {
public :
    JumpDFANode(IDFANode *);
    virtual IDFANode *next(SParameter &);

private:
    //直接记录要跳到的dfa节点
    IDFANode *direct_node;
};

//函数定义
class DefFunctionDFANode : public DFANode_1 {
public :
    DefFunctionDFANode(ASTree *, DFA *);
    virtual IDFANode *next(SParameter &);

private:
    SFunctionValue func;
};

//函数返回值的跳转
class ReturnDFANode : public JumpDFANode {
public :  
    ReturnDFANode(IDFANode *, ASTree *);
    virtual IDFANode *next(SParameter &);

private:
    ASTree *tree;
};

}
#endif
