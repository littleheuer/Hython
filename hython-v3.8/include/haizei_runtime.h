/*************************************************************************
	> File Name: haizei_runtime.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 19:10:33 2020
 ************************************************************************/

#ifndef _HAIZEI_RUNTIME_H
#define _HAIZEI_RUNTIME_H
#include <haizei_shared_type.h>

namespace haizei {

class RunTimeEnv {
public :
    //传入抽象语法树到运行环境
    RunTimeEnv(ASTree *);
    //运行环境
    void run();
    //工厂创建master用于递归执行tree中的代码逻辑
    //返回当前节点的master->run()后的结果(Value)
    static SIValue GetValue(ASTree *, SParameter);

private:
    ASTree *tree;
};

}
#endif
