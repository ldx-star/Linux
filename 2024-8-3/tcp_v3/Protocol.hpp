#pragma once

#include<iostream>
#include<string>
#include<cstring>
#include<assert.h>
using namespace std;

#define SPACE " "
#define SPACE_LEN strlen(SPACE)
#define CRLF "\r\n"
#define CRLF_LEN strlen(CRLF)
#define OPS "+-*/"
// 9\r\n100 + 200\r\n
string decode(string &in,uint32_t *len){
    assert(len);
    *len = 0;
    size_t pos = in.find(CRLF);
    if(string::npos == pos){
        return "";
    }
    string str_inLen = in.substr(0,pos);
    int int_inLne = atoi(str_inLen.c_str());
    *len = int_inLne;
    // 确认有效载荷是符合要求的
    int surplus = in.size() - 2 * CRLF_LEN - pos;
    if(surplus < int_inLne){
        return "";
    }
    // 获取报文
    string package = in.substr(pos + CRLF_LEN, int_inLne);
    return package;
}
string encode(const string & in, u_int32_t len){
    string encodein = to_string(len);
    encodein += CRLF;
    encodein += in;
    encodein += CRLF;
    return encodein;
}

class Request{
public:
    Request(){}
    ~Request(){}
    // 序列化
    void serialize(string *out){
        string xStr = to_string(x_);
        string yStr = to_string(y_);
        
        *out = xStr;
        *out += SPACE;
        *out += op_;
        *out += SPACE;
        *out += yStr;
    }
    // 反序列化
    bool deserialize(string &in){
        size_t spaceOne = in.find(SPACE);//找到第一个空格
        if(string::npos == spaceOne){
            return false;
        }
        size_t spaceTwo = in.rfind(SPACE);//找到最后一个空格
        if(string::npos == spaceTwo){
            return false;
        }
        string dataOne = in.substr(0,spaceOne);
        string dataTwo = in.substr(spaceTwo+ SPACE_LEN);
        string op = in.substr(spaceOne+SPACE_LEN,spaceTwo - (spaceOne + SPACE_LEN));
        if(op.size() != 1){
            return false;
        }
        x_ = atoi(dataOne.c_str());
        y_ = atoi(dataTwo.c_str());
        op_ = op[0];
        return true;
    }
    void debug(){
        cout <<"======================================" << endl;
        cout << "x_:" << x_ << endl;
        cout << "op_:" << op_ << endl;
        cout << "y_:" << y_ << endl;
        cout <<"======================================" << endl;
    }

public:
    int x_;
    int y_;
    char op_;
};

class Response
{
public:
    Response():exitCode_(0),result_(0){}
    ~Response(){}
    void serialize(string *out){
        string ec = to_string(exitCode_);
        string res = to_string(result_);
        *out = ec;
        *out += SPACE;
        *out +=res;
    }
    bool deserialize(string &in){
        //"0 100"
        size_t pos = in.find(SPACE);
        if(string::npos == pos){
            return false;
        }
        string codeStr = in.substr(0, pos);
        string resStr = in.substr(pos + SPACE_LEN);

        exitCode_ = atoi(codeStr.c_str());
        result_ =  atoi(resStr.c_str());
        return true;
    }
    void debug(){
        cout << "#################################" << endl;
        cout << "exitCode_: " << exitCode_ << endl;
        cout << "result_: " << result_ << endl;
        cout << "#################################" << endl;
    }
public:
    int exitCode_;
    int result_;

};
bool makeRequest(const std::string &str, Request *req)
{
    // 123+1  1*1 1/1
    char strtmp[BUFFER_SIZE];
    snprintf(strtmp, sizeof strtmp, "%s", str.c_str());
    char *left = strtok(strtmp, OPS);
    if (!left)
        return false;
    char *right = strtok(nullptr, OPS);
    if (!right)
        return false;
    char mid = str[strlen(left)];

    req->x_ = atoi(left);
    req->y_ = atoi(right);
    req->op_ = mid;
    return true;
}