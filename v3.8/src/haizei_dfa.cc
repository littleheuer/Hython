#include <iostream>
#include <haizei_dfa.h>
#include <haizei_runtime.h>
#include <haizei_parameter.h>
#include <haizei_type.h>
#include <haizei_tree.h>

namespace haizei {

    std::stack<IDFANode *> DFA::breakPoint;
    std::stack<IDFANode *> DFA::continuePoint;
    std::stack<IDFANode *> DFA::returnPoint;

    int DFA::blockPosition = 100;
    
    DFA::DFA() : head(nullptr), tail(nullptr) {}
    DFA::DFA(ASTree *tree) {
        DFA *ret = DFA::build(tree);
        head = ret->head, tail = ret->tail;
    }
    void DFA::run(SParameter &p) {
        IDFANode *now = this->head;
        //dfa一直执行到结束
        while (now) {
            //next()是状态转移
            now = now->next(p);
        }
        return ;
    }
    DFA *DFA::build(ASTree *tree) {
        DFA *ret = new DFA();
        switch (tree->type()) {
            case FUNCTION: {
                //创建返回节点
                NopeDFANode *return_node = new NopeDFANode();
                
                //用于函数内部处理return关键字
                returnPoint.push(return_node);
                //给函数内部stmt创建dfa
                DFA *dfa = DFA::build(tree->at(2));
                returnPoint.pop();
                
                
                return_node->set_pos(
                    dynamic_cast<BlockBeginDFANode *>(dfa->head)->pos()
                );
                return_node->at(0) = dfa->tail;
                
                ret->head = new DefFunctionDFANode(tree, dfa);
                //给bb新增一个标记
                blockPosition += 1;
                ret->tail = new BlockBeginDFANode(blockPosition);
                ret->head->at(0) = ret->tail;
            } break;
            case BREAK: {
                //无可跳转节点的情况
                if (breakPoint.empty()) {
                    std::string msg = "\nbreak statement is wrong! there is no loop.";
                    throw std::runtime_error(msg);
                }
                ret->head = ret->tail = new JumpDFANode(breakPoint.top());
            } break;
            case RETURN: {
                if (returnPoint.empty()) {
                    std::string msg = "\nreturn statement is wrong! there is no function.";
                    throw std::runtime_error(msg);
                }
                //有return则给returnDFA节点赋值return子节点
                ret->head = ret->tail = new ReturnDFANode(
                    returnPoint.top(), 
                    tree->size() ? tree->at(0) : nullptr
                );
            } break;
            case CONTINUE: {
                if (continuePoint.empty()) {
                    std::string msg = "\ncontinue statement is wrong! there is no loop.";
                    throw std::runtime_error(msg);
                }
                ret->head = ret->tail = new JumpDFANode(continuePoint.top());
            } break;
            case IF: {
                DFA *temp;
                //条件判断
                ret->head = new ConditionDFANode(tree->at(0));
                //if代码段收尾的节点
                ret->tail = new NopeDFANode();
                //真值为真时的block
                temp = DFA::build(tree->at(1));
                
                ret->head->at(0) = temp->head;
                temp->tail->at(0) = ret->tail;
                //else部分的dfa节点创建
                if (tree->size() == 3) {
                    temp = DFA::build(tree->at(2));
                    ret->head->at(1) = temp->head;
                    temp->tail->at(0) = ret->tail;
                } else {
                    ret->head->at(1) = ret->tail;
                }
            } break;
            case FOR: {
                blockPosition += 1;
                ret->head = new BlockBeginDFANode(blockPosition);
                ret->tail = new BlockEndDFANode(blockPosition);
                
                IDFANode *break_node = new NopeDFANode(blockPosition);
                IDFANode *continue_node = new NopeDFANode(blockPosition);
                breakPoint.push(break_node);
                continuePoint.push(continue_node);
                
                //init
                IDFANode *init_node = new ExprDFANode(tree->at(0));
                //cond
                IDFANode *cond_node = new ConditionDFANode(tree->at(1)) ;
                //expr
                IDFANode *do_node = new ExprDFANode(tree->at(2));
                
                //stmt
                DFA *stmt = DFA::build(tree->at(3));
                
                //dfa连接
                ret->head->at(0) = init_node;
                init_node->at(0) = cond_node;
                cond_node->at(0) = stmt->head;
                cond_node->at(1) = ret->tail;
                stmt->tail->at(0) = do_node;
                do_node->at(0) = cond_node;
                
                break_node->at(0) = ret->tail;
                continue_node->at(0) = do_node;
                breakPoint.pop();
                continuePoint.pop();
            } break;
            case BLOCK: {
                DFA *temp;
                blockPosition += 1;
                //给每个block创建初始状态和结束状态dfa节点
                ret->head = new BlockBeginDFANode(blockPosition);
                ret->tail = new BlockEndDFANode(blockPosition);
                IDFANode *p = ret->head;
                //将block的每个子树递归转换为dfa
                for (int i = 0; i < tree->size(); i++) {
                    temp = DFA::build(tree->at(i));
                    p->at(0) = temp->head;
                    p = temp->tail;
                }
                p->at(0) = ret->tail;
            } break;
            case WHILE: {
                blockPosition += 1;
                //创建bb和be节点以便break和continue跳转
                ret->head = new BlockBeginDFANode(blockPosition);
                ret->tail = new BlockEndDFANode(blockPosition);

                IDFANode *break_node = new NopeDFANode(blockPosition);
                IDFANode *continue_node = new NopeDFANode(blockPosition);
                breakPoint.push(break_node);
                continuePoint.push(continue_node);

                IDFANode *cond_node = new ConditionDFANode(tree->at(0));
                DFA *stmt = DFA::build(tree->at(1));
                ret->head->at(0) = cond_node;
                cond_node->at(0) = stmt->head;
                cond_node->at(1) = ret->tail;
                stmt->tail->at(0) = cond_node;
                
                break_node->at(0) = ret->tail;
                continue_node->at(0) = cond_node;
                breakPoint.pop();
                continuePoint.pop();
            } break;
            case DOWHILE: {
                blockPosition += 1;
                ret->head = new BlockBeginDFANode(blockPosition);
                ret->tail = new BlockEndDFANode(blockPosition);
                
                IDFANode *break_node = new NopeDFANode(blockPosition);
                IDFANode *continue_node = new NopeDFANode(blockPosition);
                breakPoint.push(break_node);
                continuePoint.push(continue_node);
                
                IDFANode *cond_node = new ConditionDFANode(tree->at(0));
                DFA *stmt = DFA::build(tree->at(1));
                //bb先指向stmt执行一遍
                ret->head->at(0) = stmt->head;
                stmt->tail->at(0) = cond_node;
                cond_node->at(0) = stmt->head;
                cond_node->at(1) = ret->tail;

                break_node->at(0) = ret->tail;
                continue_node->at(0) = cond_node;
                breakPoint.pop();
                continuePoint.pop();
            } break;
            //默认构造表达式dfa节点
            default: {
                ret->head = ret->tail = new ExprDFANode(tree);
            } break;
        }
        return ret;
    }

    // DFA node constructor
    IDFANode::IDFANode(ASTree *tree, int n, std::string __type) : 
        tree(tree), childs(n), __type(__type) {}
    DFANode_1::DFANode_1(ASTree *tree, std::string __type) : 
        IDFANode(tree, 1, __type) {}
    DFANode_2::DFANode_2(ASTree *tree, std::string __type) : 
        IDFANode(tree, 2, __type) {}
    ExprDFANode::ExprDFANode(ASTree *tree) : 
        DFANode_1(tree, "ExprDFANode") {}
    BlockBeginDFANode::BlockBeginDFANode(int pos) : 
        DFANode_1(nullptr, "BlockBeginDFANode"), __pos(pos) {}
    BlockEndDFANode::BlockEndDFANode(int pos) : 
        DFANode_1(nullptr, "BlockEndDFANode"), __pos(pos) {}
    ConditionDFANode::ConditionDFANode(ASTree *tree) : 
        DFANode_2(tree, "ConditionDFANode") {}
    NopeDFANode::NopeDFANode(int pos) : 
        DFANode_1(nullptr, "NopeDFANode"), pos(pos) {}
    JumpDFANode::JumpDFANode(IDFANode *direct_node) : 
        DFANode_1(nullptr, "JumpDFANode"), direct_node(direct_node) {}
    DefFunctionDFANode::DefFunctionDFANode(ASTree *tree, DFA *dfa) : 
        DFANode_1(nullptr, "DefFunctionDFANode"),
        func(std::make_shared<FunctionValue>(tree, dfa)) {}
    ReturnDFANode::ReturnDFANode(IDFANode *direct_node, ASTree *tree) :
        JumpDFANode(direct_node), tree(tree) {}

    IDFANode *&IDFANode::at(int ind) {
        return this->childs[ind];
    }
    
    std::string IDFANode::type() {
        return this->__type;
    } 

    IDFANode *ExprDFANode::next(SParameter &p) {
        //用master系统处理当前抽象语法树
        RunTimeEnv::GetValue(this->tree, p);
        //返回下一个状态
        return this->at(0);
    }
    
    IDFANode *BlockBeginDFANode::next(SParameter &p) {
        //给当前作用域添加新作用域
        p = std::make_shared<Parameter>(p, this->pos());
        return this->at(0);
    }
    int BlockBeginDFANode::pos() { return this->__pos; }

    IDFANode *BlockEndDFANode::next(SParameter &p) {
        //不断减少作用域，
        //直到减少到当前标记的作用域
        while (p->pos() != this->pos()) p = p->next();
        p = p->next();
        return this->at(0);
    }
    int BlockEndDFANode::pos() { return this->__pos; }
    
    IDFANode *ConditionDFANode::next(SParameter &p) {
        //用master系统处理当前抽象语法树
        SIValue val = RunTimeEnv::GetValue(this->tree, p);
        //根据真值来状态转移
        if (val->isTrue()) return this->at(0);
        return this->at(1);
    }
    
    IDFANode *NopeDFANode::next(SParameter &p) {
        //for,while,do-while中的pos > 0
        if (this->pos > 0) {
            while (p->pos() != this->pos) { p = p->next(); }
        }
        return this->at(0);
    }
    void NopeDFANode::set_pos(int pos) {
        this->pos = pos;
        return ;
    }
    
    IDFANode *JumpDFANode::next(SParameter &p) {
        return this->direct_node;
    }

    IDFANode *DefFunctionDFANode::next(SParameter &p) {
        SFunctionValue copy_func = std::make_shared<FunctionValue>(*(this->func));
        //在作用域中创建function变量
        p->define_param(copy_func->name());
        copy_func->set_init_param(p); 
        p->set(copy_func->name(), copy_func);
        return this->at(0);
    }
    
    IDFANode *ReturnDFANode::next(SParameter &p) {
        SIValue ret = haizei::null_val;
        //函数运行
        if (this->tree) ret = RunTimeEnv::GetValue(this->tree, p);
        //获得匿名返回值
        p->set(haizei::ReturnValueName, ret);
        return JumpDFANode::next(p);
    }

// end of namespace haizei
}
