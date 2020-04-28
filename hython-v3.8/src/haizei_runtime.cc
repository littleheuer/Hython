#include <haizei_runtime.h>
#include <haizei_parameter.h>
#include <haizei_handler.h>
#include <haizei_master.h>
#include <haizei_tree.h>
#include <haizei_dfa.h>

namespace haizei {
    RunTimeEnv::RunTimeEnv(ASTree *tree) : tree(tree) {}
    void RunTimeEnv::run() {
        SParameter p = std::make_shared<Parameter>(nullptr);
        //给tree的每个节点绑定工厂
        FactoryHandler::init_factory(tree);
        //创建dfa对象
        DFA program(tree);
        //由dfa执行整个程序逻辑
        program.run(p);
        return ;
    }
    SIValue RunTimeEnv::GetValue(ASTree *tree, SParameter p) {
        //由每个节点的工厂创建相应的master
        IMaster *m = tree->factory->create(tree, p);
        //master执行具体操作返回结果对象
        SIValue val = m->run();
        //回收
        tree->factory->destory(m);
        //返回结果
        return val;
    }
}

