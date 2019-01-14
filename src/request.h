//
// Created by zhandong on 2019/1/11.
//

#ifndef UNTITLED7_REQUEST_H
#define UNTITLED7_REQUEST_H

#endif //UNTITLED7_REQUEST_H

#include <iostream>
#include "response.h"
#include <string>
using namespace std;

class Request {
private:
    string header; //请求头
    string domain;
    string path;
    string sin_addr;
    int port;
public:
    Request(string url);

    ~Request();

    Response Get(); //爬取这个request类所对应url的数据

    bool SetUrl(string url);  //设置url 成功返回 true， 失败返回 false

    bool SetHeader(string header); //设置header 成功返回true， 失败返回 false

};
