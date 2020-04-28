#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <haizei_visitor.h>


namespace haizei {
    ListVisitor::ListVisitor(std::vector<int> index) : result(haizei::null_val), index(index){}
    
    void ListVisitor::visit(IntValue *obj) {
        std::string msg = obj->type() + " is not a list type";
        throw std::runtime_error(msg);
    }
    void ListVisitor::visit(FloatValue *obj) {
        std::string msg = obj->type() + " is not a list type";
        throw std::runtime_error(msg);
    }
    void ListVisitor::visit(StringValue *obj) {
        std::string msg = obj->type() + " is not list type";
        throw std::runtime_error(msg);
    }
    void ListVisitor::visit(FunctionValue *obj) {
        std::string msg = obj->type() + " is not list type";
        throw std::runtime_error(msg);
    }
    void ListVisitor::visit(ListValue *obj) {
        std::vector<SIValue> list = obj->val();
        int tmp = index[0];
        //std::cout << "list size : " << list.size() << std::endl;
        if(tmp > list.size() - 1) {
            std::string msg = "index [" + std::to_string(tmp) + "] is invalid!";
            throw std::runtime_error(msg);
            return ;
        }
        if(this->index.size() <= 1) {
            this->result = list[tmp];
            return ;
        }
        
        index.erase(index.begin());
        ListVisitor vis(index);
        list[tmp]->accept(&vis);
        this->result = vis.result;
    }
    
    
    void ConvertStringVisitor::visit(IntValue *obj) {
        char str[100];
        sprintf(str, "%d", obj->val());
        this->result = str;
        return ;
    }
    void ConvertStringVisitor::visit(FloatValue *obj) {
        char str[100];
        sprintf(str, "%lf", obj->val());
        this->result = str;
        return ;
    }
    void ConvertStringVisitor::visit(StringValue *obj) {
        this->result = obj->val();
        return ;
    }
    void ConvertStringVisitor::visit(FunctionValue *obj) {
        this->result = obj->val();
        return ;
    }
    void ConvertStringVisitor::visit(ListValue *obj) {
        std::vector<SIValue> list = obj->val();
        std::string ret = "[";
        //std::cout << "list size : " << list.size()<< std::endl;
        for(int i = 0; i < list.size(); i++) {
            if(i) ret += ",";
            ConvertStringVisitor vis;
            list[i]->accept(&vis);
            ret += vis.result;
        }
        ret += "]";
        this->result = ret;
        return ;
    }

    // value operator
    ValueOperator::ValueOperator(IValue *lvalue, op_type op) : 
        lvalue(lvalue), op(op) {}
    
    void ValueOperator::visit(IntValue *obj) { 
        //给lvalue的成员函数*_error()
        //传入IntValue用于报错
        (lvalue->*op)(*obj); 
    }
    void ValueOperator::visit(FloatValue *obj) { 
        (lvalue->*op)(*obj); 
    }
    void ValueOperator::visit(StringValue *obj) { 
        (lvalue->*op)(*obj); 
    }
    void ValueOperator::visit(FunctionValue *obj) { 
        (lvalue->*op)(*obj); 
    }
    void ValueOperator::visit(ListValue *obj) { 
        (lvalue->*op)(*obj); 
    }

    // type value operator
    // 调用基类ValueOperator的构造函数
    IntValueOperator::IntValueOperator(IntValue *left, ValueOperator::op_type op) : 
        ValueOperator(left, op), left(left) {}
    FloatValueOperator::FloatValueOperator(FloatValue *left, ValueOperator::op_type op) : 
        ValueOperator(left, op), left(left) {}
    StringValueOperator::StringValueOperator(StringValue *left, ValueOperator::op_type op) : 
        ValueOperator(left, op), left(left) {}
    ListValueOperator::ListValueOperator(ListValue *left, ValueOperator::op_type op) : 
        ValueOperator(left, op), left(left) {}
    
    // IntValue operator
    // 调用基类IntValueOperator的构造函数
    IntValuePlusOpVisitor::IntValuePlusOpVisitor(IntValue *left) : 
        IntValueOperator(left, &IValue::operator_plus_error) {}
    
    void IntValuePlusOpVisitor::visit(IntValue *obj) {
        //具体操作
        this->result = std::make_shared<IntValue>(left->val() + obj->val());
        return ;
    }
    void IntValuePlusOpVisitor::visit(FloatValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() + obj->val());
        return ;
    }
    
    IntValueMinusOpVisitor::IntValueMinusOpVisitor(IntValue *left) :
        IntValueOperator(left, &IValue::operator_minus_error) {}
    void IntValueMinusOpVisitor::visit(IntValue *obj) {
        this->result = std::make_shared<IntValue>(left->val() - obj->val());
        return ;
    }
    void IntValueMinusOpVisitor::visit(FloatValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() - obj->val());
        return ;
    }

    IntValueTimesOpVisitor::IntValueTimesOpVisitor(IntValue *left) :
        IntValueOperator(left, &IValue::operator_times_error) {}
    void IntValueTimesOpVisitor::visit(IntValue *obj) {
        this->result = std::make_shared<IntValue>(left->val() * obj->val());
        return ;
    }
    void IntValueTimesOpVisitor::visit(FloatValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() * obj->val());
        return ;
    }

    IntValueLittleOpVisitor::IntValueLittleOpVisitor(IntValue *left) : 
        IntValueOperator(left, &IValue::operator_compare_error){}
    void IntValueLittleOpVisitor::visit(IntValue *obj) {
        this->result = haizei::False_val;
        if ((left->val() < obj->val())) {
            this->result = haizei::True_val;
        }
        return ;
    }
    void IntValueLittleOpVisitor::visit(FloatValue *obj) {
        this->result = haizei::False_val;
        if ((left->val() < obj->val())) {
            this->result = haizei::True_val;
        }
        return ;
    }
    
    IntValueModOpVisitor::IntValueModOpVisitor(IntValue *left) :
        IntValueOperator(left, &IValue::operator_mod_error) {}
    void IntValueModOpVisitor::visit(IntValue *obj) {
        this->result = std::make_shared<IntValue>(left->val() % obj->val());
        return ;
    }

    // FloatValue operator
    FloatValuePlusOpVisitor::FloatValuePlusOpVisitor(FloatValue *left) :
        FloatValueOperator(left, &IValue::operator_plus_error) {}
    void FloatValuePlusOpVisitor::visit(IntValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() + obj->val());
        return ;
    }
    void FloatValuePlusOpVisitor::visit(FloatValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() + obj->val());
        return ;
    }

    FloatValueMinusOpVisitor::FloatValueMinusOpVisitor(FloatValue *left) :
        FloatValueOperator(left, &IValue::operator_minus_error) {}
    void FloatValueMinusOpVisitor::visit(IntValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() - obj->val());
        return ;
    }
    void FloatValueMinusOpVisitor::visit(FloatValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() - obj->val());
        return ;
    }
    
    FloatValueTimesOpVisitor::FloatValueTimesOpVisitor(FloatValue *left) :
        FloatValueOperator(left, &IValue::operator_times_error) {}
    void FloatValueTimesOpVisitor::visit(IntValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() * obj->val());
        return ;
    }
    void FloatValueTimesOpVisitor::visit(FloatValue *obj) {
        this->result = std::make_shared<FloatValue>(left->val() * obj->val());
        return ;
    }

    FloatValueLittleOpVisitor::FloatValueLittleOpVisitor(FloatValue *left) : 
        FloatValueOperator(left, &IValue::operator_compare_error){}
    void FloatValueLittleOpVisitor::visit(IntValue *obj) {
        this->result = haizei::False_val;
        if ((left->val() < obj->val())) {
            this->result = haizei::True_val;
        }
        return ;
    }
    void FloatValueLittleOpVisitor::visit(FloatValue *obj) {
        this->result = haizei::False_val;
        if ((left->val() < obj->val())) {
            this->result = haizei::True_val;
        }
        return ;
    }

    // StringValue operator
    StringValuePlusOpVisitor::StringValuePlusOpVisitor(StringValue *left) :
        StringValueOperator(left, &IValue::operator_plus_error) {}
    void StringValuePlusOpVisitor::visit(IntValue *obj) {
        ConvertStringVisitor vis;
        obj->accept(&vis);
        this->result = std::make_shared<StringValue>(left->val() + vis.result);
        return ;
    }
    void StringValuePlusOpVisitor::visit(FloatValue *obj) {
        ConvertStringVisitor vis;
        obj->accept(&vis);
        this->result = std::make_shared<StringValue>(left->val() + vis.result);
        return ;
    }
    void StringValuePlusOpVisitor::visit(StringValue *obj) {
        this->result = std::make_shared<StringValue>(left->val() + obj->val());
        return ;
    }

    StringValueTimesOpVisitor::StringValueTimesOpVisitor(StringValue *left) :
        StringValueOperator(left, &IValue::operator_times_error) {}
    void StringValueTimesOpVisitor::visit(IntValue *obj) {
        std::string ret = ""; 
        for (int i = 0, I = obj->val(); i < I; i++) {
            ret += left->val();
        }
        this->result = std::make_shared<StringValue>(ret);
        return ;
    }
    
    StringValueLittleOpVisitor::StringValueLittleOpVisitor(StringValue *left) : 
        StringValueOperator(left, &IValue::operator_compare_error){}
    void StringValueLittleOpVisitor::visit(StringValue *obj) {
        this->result = haizei::False_val;
        if ((left->val() < obj->val())) {
            this->result = haizei::True_val;
        }
        return ;
    }
    

    //ListValue operator
    ListValuePlusOpVisitor::ListValuePlusOpVisitor(ListValue *left) : 
        ListValueOperator(left, &IValue::operator_plus_error) {}
    
    void ListValuePlusOpVisitor::visit(IntValue *obj) {
        std::vector<SIValue> tmp = this->left->val();
        tmp.push_back(std::make_shared<IntValue>(obj->val()));
        this->result = std::make_shared<ListValue>(tmp);
        return ;
    } 
    void ListValuePlusOpVisitor::visit(FloatValue *obj) {
        std::vector<SIValue> tmp = this->left->val();
        tmp.push_back(std::make_shared<FloatValue>(obj->val()));
        this->result = std::make_shared<ListValue>(tmp);
        return ;
    }
    void ListValuePlusOpVisitor::visit(StringValue *obj) {
        std::vector<SIValue> tmp = this->left->val();
        tmp.push_back(std::make_shared<StringValue>(obj->val()));
        this->result = std::make_shared<ListValue>(tmp);
        return ;
    }
    void ListValuePlusOpVisitor::visit(ListValue *obj) {
        std::vector<SIValue> tmp = this->left->val();
        tmp.push_back(std::make_shared<ListValue>(obj->val()));
        this->result = std::make_shared<ListValue>(tmp);
        return ;
    }


    ListValueMinusOpVisitor::ListValueMinusOpVisitor(ListValue *left) : 
        ListValueOperator(left, &IValue::operator_minus_error) {}
    
    void ListValueMinusOpVisitor::visit(IntValue *obj) {
        std::vector<SIValue> tmp = this->left->val();
        //直接通过字符串信息来查找
        for(int i = 0; i < tmp.size(); i++) {
            
            ConvertStringVisitor vis1, vis2;
            tmp[i]->accept(&vis1);
            obj->accept(&vis2);
            if(vis1.result == vis2.result) {
                tmp.erase(tmp.begin() + i);
                this->result = std::make_shared<ListValue>(tmp);
                return ;
            }
        }
        this->result = std::make_shared<ListValue>(tmp);
        return ;
    }

    void ListValueMinusOpVisitor::visit(FloatValue *obj) {
        std::vector<SIValue> tmp = this->left->val();
        for(int i = 0; i < tmp.size(); i++) {
            
            ConvertStringVisitor vis1, vis2;
            tmp[i]->accept(&vis1);
            obj->accept(&vis2);
            if(vis1.result == vis2.result) {
                tmp.erase(tmp.begin() + i);
                this->result = std::make_shared<ListValue>(tmp);
                return ;
            }
        }
        this->result = std::make_shared<ListValue>(tmp);
        return ;
    }
    
    void ListValueMinusOpVisitor::visit(StringValue *obj) {
        std::vector<SIValue> tmp = this->left->val();
        for(int i = 0; i < tmp.size(); i++) {
            
            ConvertStringVisitor vis1, vis2;
            tmp[i]->accept(&vis1);
            obj->accept(&vis2);
            if(vis1.result == vis2.result) {
                tmp.erase(tmp.begin() + i);
                this->result = std::make_shared<ListValue>(tmp);
                return ;
            }
        }
        this->result = std::make_shared<ListValue>(tmp);
        return ;
    }
    void ListValueMinusOpVisitor::visit(ListValue *obj) {
        std::vector<SIValue> tmp = this->left->val();
        for(int i = 0; i < tmp.size(); i++) {
            
            ConvertStringVisitor vis1, vis2;
            tmp[i]->accept(&vis1);
            obj->accept(&vis2);
            if(vis1.result == vis2.result) {
                tmp.erase(tmp.begin() + i);
                this->result = std::make_shared<ListValue>(tmp);
                return ;
            }
        }
        this->result = std::make_shared<ListValue>(tmp);
        return ;
    }
}
