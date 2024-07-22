#pragma once

#include <iostream>
#include <string>

class Task
{
public:
    Task() : elemOne_(0), elemTwo_(0), operator_('0')
    {
    }
    Task(int one, int two, char op) : elemOne_(one), elemTwo_(two), operator_(op)
    {
    }
    int operator() ()
    {
        return run();
    }
    int run()
    {
        int result = 0;
        switch (operator_)
        {
        case '+':
            result = elemOne_ + elemTwo_;
            break;
        case '-':
            result = elemOne_ - elemTwo_;
            break;
        case '*':
            result = elemOne_ * elemTwo_;
            break;
        case '/':
        {
            if (elemTwo_ == 0)
            {
                std::cout << "div zero, abort" << std::endl;
                result = -1;
            }
            else
            {
                result = elemOne_ / elemTwo_;
            }
        }

        break;
        case '%':
        {
            if (elemTwo_ == 0)
            {
                std::cout << "mod zero, abort" << std::endl;
                result = -1;
            }
            else
            {
                result = elemOne_ % elemTwo_;
            }
        }
        break;
        default:
            std::cout << "非法操作: " << operator_ << std::endl;
            break;
        }
        return result;
    }
    void get(int *e1, int *e2, char *op)
    {
        *e1 = elemOne_;
        *e2 = elemTwo_;
        *op = operator_;
    }
private:
    int elemOne_;
    int elemTwo_;
    char operator_;
};