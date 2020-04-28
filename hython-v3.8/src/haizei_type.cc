#include <iostream>
#include <sstream>
#include <haizei_type.h>
#include <haizei_visitor.h>
#include <haizei_dfa.h>
#include <haizei_tree.h>
#include <haizei_parameter.h>
#include <haizei_runtime.h>

namespace haizei {
    IValue::IValue() {}
    IValue::IValue(std::string Tname) : Tname(Tname) {}
    std::string IValue::type() { return this->Tname; }
    IValue::~IValue() {}
    IntValue::~IntValue() {}
    FloatValue::~FloatValue() {}
    StringValue::~StringValue() {}
    FunctionValue::~FunctionValue() {}
    ListValue::~ListValue() {}

    // visitor function
    void IntValue::accept(IVisitor *vis) { 
        vis->visit(this); 
    }
    void FloatValue::accept(IVisitor *vis) { 
        vis->visit(this); 
    }
    void StringValue::accept(IVisitor *vis) { 
        vis->visit(this); 
    }
    void FunctionValue::accept(IVisitor *vis) { 
        vis->visit(this); 
    }
    void ListValue::accept(IVisitor *vis) { 
        vis->visit(this); 
    }

    // convert constructor
    IntValue::IntValue(int x) : IValue("int"), __val(x) {}
    FloatValue::FloatValue(double x) : IValue("float"), __val(x) {}
    StringValue::StringValue(std::string x) : IValue("string"), __val(x) {}
    FunctionValue::FunctionValue(ASTree *tree, DFA *dfa) : 
        IValue("function"), __val(dfa) { convert(tree); }
    
    ListValue::ListValue(ASTree *tree, SParameter p) : 
        IValue("list") { convert(tree, p); }
    ListValue::ListValue(std::vector<SIValue> vec) {
        for(int i = 0; i < vec.size(); i++) {
            this->__val.push_back(vec[i]);
        }
    }
    
    // get value function
    int IntValue::val() { return __val; }
    double FloatValue::val() { return __val; }
    std::string StringValue::val() { return __val; }
    
    // IValue operator
    bool IValue::isFalse() {
        return !(this->isTrue());
    }
    int IValue::intVal() {
        std::string msg = this->type() + "is not INT type!"; 
        throw std::runtime_error(msg);
        return -1;
    }
    SIValue IValue::run(SParameter &p, ASTree *tree) {
        std::string msg = this->type() + " is not callable!";
        throw std::runtime_error(msg);
        return haizei::null_val;
    }
    SIValue IValue::operator+(IValue &obj) {
        this->operator_plus_error(obj);
        return haizei::null_val;
    }

    SIValue IValue::operator-(IValue &obj) {
        this->operator_minus_error(obj);
        return haizei::null_val;
    }
    
    SIValue IValue::operator*(IValue &obj) {
        this->operator_times_error(obj);
        return haizei::null_val;
    }
    SIValue IValue::operator%(IValue &obj) {
        this->operator_mod_error(obj);
        return haizei::null_val;
    }
    bool IValue::operator>(IValue &obj) {
        return obj < (*this);
    }
    bool IValue::operator<=(IValue &obj) {
        return !(obj < (*this));
    }
    bool IValue::operator>=(IValue &obj) {
        return !((*this) < obj);
    }
    bool IValue::operator==(IValue &obj) {
        return !((*this) != obj);
    }
    bool IValue::operator!=(IValue &obj) {
        return ((*this) < obj || obj < (*this));
    }
    bool IValue::operator<(IValue &obj) {
        operator_compare_error(obj);
        return false;
    }
    
    //左右类型不支持运算时输出运算错误信息
    void IValue::operator_plus_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " + " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ; 
    }
    
    void IValue::operator_minus_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " - " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ; 
    }

    void IValue::operator_times_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " * " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ;
    }
    
    void IValue::operator_mod_error(IValue &obj) {
        std::string msg = "\nnot support operator (";
        msg += this->type() + " % " + obj.type() + ")";
        throw std::runtime_error(msg);
        return ;
    }
    
    void IValue::operator_compare_error(IValue &obj) {
        std::string msg = "\n(" + this->type() + " vs " + obj.type();
        msg += ") can not be compared!";
        throw std::runtime_error(msg);
        return ;
    }
    
    // IntValue operator
    // int型真值只要不为0则为真
    bool IntValue::isTrue() {
        return this->val() != 0;
    }
    int IntValue::intVal() {
        return this->__val;
    }
    //用访问者去根据不同类型做不同操作
    SIValue IntValue::operator+(IValue &obj) {
        IntValuePlusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    SIValue IntValue::operator-(IValue &obj) {
        IntValueMinusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    SIValue IntValue::operator*(IValue &obj) {
        IntValueTimesOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    SIValue IntValue::operator%(IValue &obj) {
        IntValueModOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    bool IntValue::operator<(IValue &obj) {
        IntValueLittleOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result->isTrue();
    }
    
    // FloatValue operator
    // double型真值只要不为0.0则为真
    bool FloatValue::isTrue() {
        return this->val() != 0.0;
    }
    SIValue FloatValue::operator+(IValue &obj) {
        FloatValuePlusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    SIValue FloatValue::operator-(IValue &obj) {
        FloatValueMinusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    SIValue FloatValue::operator*(IValue &obj) {
        FloatValueTimesOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }

    bool FloatValue::operator<(IValue &obj) {
        FloatValueLittleOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result->isTrue();
    }

    //StringValue operator
    // string型真值只要不为空则为真
    bool StringValue::isTrue() {
        return this->val() != "";
    }
    SIValue StringValue::operator+(IValue &obj) {
        StringValuePlusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    SIValue StringValue::operator*(IValue &obj) {
        StringValueTimesOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    
    bool StringValue::operator<(IValue &obj) {
        StringValueLittleOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result->isTrue();
    }

    // FunctionValue operator
    bool FunctionValue::isTrue() {
        return true;
    }
    std::string FunctionValue::val() {
        std::string ret = "function " + this->func_name; 
        ret += "(";
        for (int i = 0; i < this->param.size(); i++) {
            if (i) ret += ", ";
            ret += this->param[i];
        }
        ret += ");";
        return ret;
    }
    std::string FunctionValue::name() { return this->func_name; }
    void FunctionValue::convert(ASTree *tree) {
        this->func_name = tree->at(0)->text();
        this->param.clear();
        for (int i = 0, I = tree->at(1)->size(); i < I; i++) {
            this->param.push_back(tree->at(1)->at(i)->text());
        }
        return ;
    }
    void FunctionValue::set_init_param(SParameter init_param) {
        this->init_param = init_param;
        return ;
    }
    SIValue FunctionValue::run(SParameter &p, ASTree *param) {
        this->init_param = std::make_shared<Parameter>(this->init_param);
        if (param->size() != this->param.size()) {
            std::string msg; 
            std::stringstream out(msg);
            out << this->name() << " call is wrong" << std::endl;
            out << "\texpect " << this->param.size() << " params, but ";
            out << param->size() << " given." << std::endl;
            throw std::runtime_error(msg);
            return haizei::null_val;
        }
        for (int i = 0; i < param->size(); i++) {
            this->init_param->define_param(this->param[i]);
            this->init_param->set(
                this->param[i], 
                RunTimeEnv::GetValue(param->at(i), p->next())
            );
        }
        this->init_param->define_param(haizei::ReturnValueName);
        this->init_param->set(haizei::ReturnValueName, haizei::null_val);
        this->__val->run(this->init_param);
        SIValue retValue = this->init_param->get(haizei::ReturnValueName);
        this->init_param = this->init_param->next();
        return retValue;
    }
    
    //ListValue

    bool ListValue::isTrue() {
        return !(this->val().empty());
    }
    std::vector<SIValue> ListValue::val() {
        return __val;
    }

    SIValue ListValue::operator+(IValue &obj) {
        ListValuePlusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    SIValue ListValue::operator-(IValue &obj) {
        ListValueMinusOpVisitor vis(this);
        obj.accept(&vis);
        return vis.result;
    }
    
    void ListValue::convert(ASTree *tree, SParameter p) {
        for(int i = 0, I = tree->size(); i < I; i++) {
            auto child_tree = tree->at(i);
            SIValue tmp = RunTimeEnv::GetValue(child_tree, p);
            this->__val.push_back(tmp);
        }
        return ;
    }

}
