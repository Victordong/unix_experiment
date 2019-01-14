//
// Created by zhandong on 2019/1/11.
//

#ifndef UNTITLED7_RESPONSE_H
#define UNTITLED7_RESPONSE_H

#endif //UNTITLED7_RESPONSE_H

#include <iostream>
#include <string>
using namespace std;

class Response {
private:
    string header; //响应头
    string body; //响应实体
public:
    Response(string header, string body); //构造函数

    string GetHeader(); //获取响应头

    string GetBody(); //获取响应实体
};