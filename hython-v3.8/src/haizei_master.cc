#include <iostream>
#include <haizei_master.h>
#include <haizei_tree.h>
#include <haizei_runtime.h>
#include <haizei_visitor.h>
#include <haizei_parameter.h>
#include <haizei_util.h>

namespace haizei {
    //constructor
    IMaster::IMaster(ASTree *tree, SParameter p) : tree(tree), p(p) 
    {}
    IMaster::~IMaster() {}
    PrintMaster::PrintMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}
    ExprMaster::ExprMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}
    BlockMaster::BlockMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}
    ConditionMaster::ConditionMaster(ASTree *tree, SParameter p) : IMaster(tree, p) {}

    void IMaster::IFactory::destory(IMaster *m) {
        delete m;
    }
    
    SIValue PrintMaster::run() {
        if (tree->type() != PRINT) {
            throw std::runtime_error("tree type is not print!");
        }
        //遍历print节点的子节点
        for (int i = 0, I = tree->size(); i < I; i++) {
            auto child_tree = tree->at(i);
            //获取该子结点的运算结果
            SIValue ret = RunTimeEnv::GetValue(child_tree, p);
            //访问者用于获取IValue子类的字符串信息
            //因为IValue子类的val()函数不是基类的虚函数
            ConvertStringVisitor vis;
            ret->accept(&vis);
            if (i) std::cout << " ";
            std::cout << vis.result;
        }
        std::cout << std::endl;
        return haizei::null_val;
    }
    
    SIValue ExprMaster::run() {
        switch (tree->type()) {
            case LIST: {
                return std::make_shared<ListValue>(this->tree, this->p);
            }
            case LIST_IND: {
                SIValue list = this->p->get(tree->at(0)->text());
                std::vector<int> index;
                for(int i = 1, I = tree->size(); i < I; i++) {
                    SIValue a = RunTimeEnv::GetValue(tree->at(i), p);
                    index.push_back(a->intVal());
                }
                ListVisitor vis(index);
                list->accept(&vis);
                return vis.result;
            }
            case FUNC_CALL: {
                //根据节点字符串信息在作用域中找到函数对象
                //并返回SFunctionValue
                auto func = this->p->get(tree->at(0)->text());
                //执行函数内部的逻辑
                return func->run(this->p, tree->at(1));
            }
            case NOPE: {
                return haizei::True_val;
            }
            case INT: {
                //string->char
                const char* s = this->tree->text().c_str();
                int val_int;
                //将char转为int
                sscanf(s, "%d", &val_int);
                //创建新IntValue
                return std::make_shared<IntValue>(val_int);
            }
            case STRING: {
                //处理转义字符
                std::string temp = ConvertStringToCString(this->tree->text());
                //丢掉头尾的"符号，只保留" "内的字符串信息
                return std::make_shared<StringValue>(temp.substr(1, temp.size() - 2));
            }
            case FLOAT: {
                const char* s = this->tree->text().c_str();
                double val_float;
                sscanf(s, "%lf", &val_float);
                return std::make_shared<FloatValue>(val_float);
            }
            case ID: {
                //返回节点相应变量的值
                return this->p->get(this->tree->text());
            }
            case PLUS: {
                //获得两个子树的值，并返回加法运算的结果SIValue
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) + (*b);
            }
            case MINUS: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) - (*b);
            }
            case TIMES: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) * (*b);
            }
            case MOD: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                return (*a) % (*b);
            }
            case ASSIGN: {
                std::string var = tree->at(0)->text();
                //获得赋值运算符之后的计算结果对象
                SIValue val = RunTimeEnv::GetValue(tree->at(1), p);
                //string-->val绑定
                this->p->set(var, val);
                return val;
            }
            case DEF : {
                for (int i = 0; i < tree->size(); i++) {
                    std::string var = tree->at(i)->text();
                    this->p->define_param(var);
                    SIValue val = haizei::null_val;
                    //赋初值(若有)
                    if (tree->at(i)->size()) {
                        val = RunTimeEnv::GetValue(tree->at(i)->at(0), this->p);
                    }
                    this->p->set(var, val);
                }
                //define没有返回值
                return haizei::null_val;
            }
            default : {
                throw std::runtime_error("tree type is not expr!");
            }
        }
        return haizei::null_val;
    }
    
    SIValue BlockMaster::run() {
        if (tree->type() != BLOCK) {
            throw std::runtime_error("tree type is not block!");
        }
        //创建新的作用域,传入老作用域
        p = std::make_shared<Parameter>(p);
        for (int i = 0, I = tree->size(); i < I; i++) {
            //在新作用域依次执行子树
            auto child_tree = tree->at(i);
            RunTimeEnv::GetValue(child_tree, p);
        }
        //新作用域会自动回收
        p = p->next();
        return haizei::null_val;
    }
    //条件表达式处理 
    SIValue ConditionMaster::run() {
        switch (tree->type()) {
            case OR: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                if (a->isTrue()) return haizei::True_val;
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if (b->isTrue()) return haizei::True_val;
                return haizei::False_val;
            }
            case AND: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                if (a->isFalse()) return haizei::False_val;
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if (b->isTrue()) return haizei::True_val;
                return haizei::False_val;
            } 
            case GE: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) >= (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case GT: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) > (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case LE: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) <= (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case LITTLE: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) < (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case EQ: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) == (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            case NE: {
                SIValue a = RunTimeEnv::GetValue(tree->at(0), p);
                SIValue b = RunTimeEnv::GetValue(tree->at(1), p);
                if ((*a) != (*b)) return haizei::True_val;
                return haizei::False_val;
            }
            default : {
                throw std::runtime_error("tree type is not condition!");
            }
        }
        return haizei::False_val;
    }
    

// end of namespace haizei
}


