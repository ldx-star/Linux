#pragma once
#include<iostream>
#include<string>

using namespace std;

class Task{
public:
    Task():elem_one_(0),elem_two_(0),operator_('0') {}
    Task(double a,double b, char op):elem_one_(a),elem_two_(b),operator_(op) {}


    int run(){
        double result = 0;
        switch (operator_)
        {
        case '+':
            result = elem_one_ + elem_two_;
            break;
        
        case '-':
            result = elem_one_ - elem_two_;
            break;

        case '*':
            result = elem_one_ * elem_two_;
            break;

        case '/':
        {
            if(elem_two_ == 0){
                cerr << "div by zero" << endl;
                result = -1;
            }else{
                result = elem_one_ / elem_two_;
            }
        }
            
            break;
        default:
            cout << "非法操作" << endl;
            break;
        }
        return result;
    }



private:
    double elem_one_;
    double elem_two_;
    char operator_;
};