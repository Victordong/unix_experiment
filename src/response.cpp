//
// Created by zhandong on 2019/1/14.
//


#include "response.h"

Response::Response(string header, string body, string status) {
    this->header = header;
    this->body = body;
    this->status = status;
}

string Response::GetStatus() {
    return this->status;
}

string Response::GetBody() {
    return this->body;
}

string Response::GetHeader() {
    return this->header;
}