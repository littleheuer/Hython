/*************************************************************************
	> File Name: haizei_master.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 19:39:40 2020
 ************************************************************************/

#ifndef _HAIZEI_MASTER_H
#define _HAIZEI_MASTER_H
#include <haizei_shared_type.h>

namespace haizei {


class IMaster {
public :
    IMaster(ASTree *, SParameter);
    //工厂接口
    class IFactory {
    public :
        //生产不同对象用于处理节点
        virtual IMaster *create(ASTree *, SParameter) = 0;
        //销毁对象
        virtual void destory(IMaster *);
    };
    virtual SIValue run() = 0;
    virtual ~IMaster();

protected:
    //存抽象语法树
    ASTree* tree;
    //存作用域
    SParameter p;
};

class PrintMaster : public IMaster {
public :
    PrintMaster(ASTree *, SParameter);
    virtual SIValue run();
};

class ExprMaster : public IMaster {
public :
    ExprMaster(ASTree *, SParameter);
    virtual SIValue run();
};

class BlockMaster : public IMaster {
public :
    BlockMaster(ASTree *, SParameter);
    virtual SIValue run();
};

class ConditionMaster : public IMaster {
public :
    ConditionMaster(ASTree *, SParameter);
    virtual SIValue run();
};

//条件表达式处理
}

#endif
