//
// Created by zhandong on 2019/1/14.
//
#include "work_list.h"
#include <string>
using namespace std;

string WorkList::GetWillDo() {
    set<string>::iterator cursor = this->will_do_list.begin();
    string url = (*cursor);
    this->will_do_list.erase(cursor);
    return url;
}

bool WorkList::PutHasdone(string url) {
    this->has_done_list.insert(url);
    return true;
}

bool WorkList::PutWillDo(string url) {
    set<string>::iterator cursor = this->has_done_list.find(url);
    if (cursor != this->has_done_list.end()) {
        return false;
    }
    this->will_do_list.insert(url);
    return true;
}

bool WorkList::WillDoEmpty() {
    return this->will_do_list.empty();
}
