//
// Created by zhandong on 2019/1/14.
//


#include "response.h"

Response::Response(string header, string body) {
    this->header = header;
    this->body = body;
}

string Response::GetBody() {
    return this->body;
}

string Response::GetHeader() {
    return this->header;
}