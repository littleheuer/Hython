#include <string>
#include <exception>
#include <haizei_handler.h>
#include <haizei_tree.h>
#include <haizei_factory.h>

namespace haizei {
    FactoryHandler* FactoryHandler::get() {
        //单例模式，保证全局只有一条责任链
        if (head == nullptr) {
            head = new PrintFactoryHandler();
            head->next = new ExprFactoryHandler();
            head->next->next = new BlockFactoryHandler();
            head->next->next->next = new ConditionFactoryHandler();
            //head->next->next->next->next = new ControlFactoryHandler();
        }
        return head;
    }

    void FactoryHandler::init_factory(ASTree *tree) {
        //获取责任链头节点
        auto p = FactoryHandler::get();
        //给根结点挂工厂
        for (; p != nullptr; p = p->next) {
            if (!p->is_valid(tree)) continue;
            //挂工厂
            p->do_factory(tree);
            break;
        }
        //责任链无法处理报错
        if (p == nullptr) {
            char str[100];
            sprintf(str, "%d", tree->type());
            throw std::runtime_error(std::string("there is no handler with ") + str);
        }
        //递归的给子节点挂工厂
        for (int i = 0; i < tree->size(); i++) {
            FactoryHandler::init_factory(tree->at(i));
        }
        return ;
    }
    
    bool PrintFactoryHandler::is_valid(ASTree *tree) {
        switch (tree->type()) {
            case PRINT: return true;
            default : return false;
        }
        return false;
    }
    
    bool ExprFactoryHandler::is_valid(ASTree *tree) {
        switch (tree->type()) {
            case LIST:
            case LIST_IND: 
            case FUNC_CALL:
            case NOPE:
            case INT:
            case STRING:
            case FLOAT:
            case ID:
            case PLUS:
            case MINUS:
            case TIMES:
            case MOD:
            case ASSIGN:
            case DEF:
                return true;
            default:
                return false;
        }
        return false;
    }
    
    bool BlockFactoryHandler::is_valid(ASTree *tree) {
        switch (tree->type()) {
            case RETURN:
            case PARAM:
            case FUNCTION:
            case BREAK:
            case CONTINUE:
            case IF:
            case FOR:
            case WHILE:
            case DOWHILE:
            case BLOCK: return true;
            default : return false;
        }
        return false;
    }
    
    bool ConditionFactoryHandler::is_valid(ASTree *tree) {
        switch (tree->type()) {
            case OR:
            case GE:
            case GT:
            case LE:
            case LITTLE:
            case EQ:
            case NE:
            case AND: return true;
            default : return false;
        }
        return false;
    }
    
    //挂工厂: 就是指针赋值
    void PrintFactoryHandler::do_factory(ASTree *tree) {  
        tree->factory = factory;
        return ;
    }
    void ExprFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }
    
    void BlockFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }
    
    void ConditionFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }
    
    /*
    void ControlFactoryHandler::do_factory(ASTree *tree) {
        tree->factory = factory;
        return ;
    }
    */

    //静态变量初始化，每个工厂也是单例模式
    BlockFactory *BlockFactoryHandler::factory = new BlockFactory();
    ExprFactory *ExprFactoryHandler::factory = new ExprFactory();
    PrintFactory *PrintFactoryHandler::factory = new PrintFactory();
    ConditionFactory *ConditionFactoryHandler::factory = new ConditionFactory();
    //ControlFactory *ControlFactoryHandler::factory = new ControlFactory();
    FactoryHandler *FactoryHandler::head = nullptr;
}
