//
// Created by zhandong on 2019/1/12.
//

#ifndef UNTITLED7_WORK_LIST_H
#define UNTITLED7_WORK_LIST_H

#endif //UNTITLED7_WORK_LIST_H

#include <set>
#include <string>
using namespace std;

class WorkList {
private:
    set<string> has_done_list;
    set<string> will_do_list;
public:
    WorkList(); //构造函数 在里面 定义 has_done_list will_do_list
    string GetWillDo();//从will do 里面获取一个并返回
    bool PutHasdone(string url); //将一个url 放入 has done list
    bool IfHasDone(string url); //判断一个url是否已经爬过
    bool PutWillDo(string url); //讲一个url 放入 will do list
};