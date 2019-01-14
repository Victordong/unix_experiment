//
// Created by zhandong on 2019/1/14.
//

#include "data.h"
#include <string>
#include <iostream>
using namespace std;

Data::Data(string title, string author, string summary, string charset, string sub_time, string domain_name, string url, string text) {
    this->title = title;
    this->author = author;
    this->summary = summary;
    this->charset = charset;
    this->sub_time = sub_time;
    this->domain_name = domain_name;
    this->url = url;
    this->text = text;
}

bool Data::ToFile(){
    FILE *fp;
    fp=fopen("file.txt","W+");
    if(!fp)
    {
        cout<<"error"<<endl;
        return false;
    }
    fprintf(fp,"title:%s author:%s summary:%s charset:%s sub_time:%s domain_name:%s url:%s text:%s", this->title.c_str(),this->author.c_str(),this->summary.c_str(),this->charset.c_str(),this->sub_time.c_str(),this->domain_name.c_str(),this->url.c_str(),this->text.c_str());
    fclose(fp);
    return true;
}