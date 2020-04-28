/*************************************************************************
	> File Name: haizei_handler.h
	> Author: 
	> Mail: 
	> Created Time: 二  3/31 18:52:24 2020
 ************************************************************************/

#ifndef _HAIZEI_HANDLER_H
#define _HAIZEI_HANDLER_H

namespace haizei {

//使用声明，而不是包含头文件
//避免头文件互相包含的问题
class ASTree;
class PrintFactory;
class ExprFactory;
class BlockFactory;
class ConditionFactory;
class ControlFactory;

class FactoryHandler {
public :
    //用具体的责任链在每个节点上挂工厂
    static void init_factory(ASTree *);
    //获取责任链头节点
    static FactoryHandler *get();
    //判断当前节点是否属于自己处理
    virtual bool is_valid(ASTree *) = 0;
    //挂工厂操作
    virtual void do_factory(ASTree *) = 0;
    
protected:
    //责任链头指针
    static FactoryHandler *head;
    //责任链中的下一个
    FactoryHandler *next;
};

class PrintFactoryHandler : public FactoryHandler {
public :
    virtual bool is_valid(ASTree *);
    virtual void do_factory(ASTree *);

private:
    //静态的具体工厂
    static PrintFactory *factory;
};

class ExprFactoryHandler : public FactoryHandler {
public :
    virtual bool is_valid(ASTree *);
    virtual void do_factory(ASTree *);

private:
    static ExprFactory *factory;
};

class BlockFactoryHandler : public FactoryHandler {
public :
    virtual bool is_valid(ASTree *);
    virtual void do_factory(ASTree *);

private:
    static BlockFactory *factory;
};

class ConditionFactoryHandler : public FactoryHandler {
public :
    virtual bool is_valid(ASTree *);
    virtual void do_factory(ASTree *);

private:
    static ConditionFactory *factory;
};


}
#endif
