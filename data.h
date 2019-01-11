//
// Created by zhandong on 2019/1/12.
//

#ifndef UNTITLED7_DATA_H
#define UNTITLED7_DATA_H

#endif //UNTITLED7_DATA_H

#include <string>
using namespace std;
class Data{
private:
    string title;
    string author;
    string summary;
    string charset;
    string sub_time;
    string domain_name;
    string url;
    string text;
public:
    Data(string title, string author, string summary, string charset, string sub_time, string domain_name, string url, string text);//构造函数
    bool ToFile(); //将类转化成字符串存入相应的文件中
}
