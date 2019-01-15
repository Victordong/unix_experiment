//
// Created by zhandong on 2019/1/14.
//

#include "data.h"
#include <string>
#include <iostream>
#include <fstream>
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

string MakePathName(string domain, string sub_time, string title) {
    return "/Users/zhandong/CLionProjects/untitled7/data/"+ domain + "_" + sub_time + "_" +title;
}

bool Data::ToFile(){
    string pathname = MakePathName(this->domain_name, this->sub_time, this->title);
    ofstream fout(pathname);
    if(!fout)
    {
        cout<<"error"<<endl;
        return false;
    }
    fout<<"title: "<<this->title.c_str()<<endl<<"author: "<< this->author.c_str()<<endl<<"summary: "<<this->summary<<endl<<"charset: "<<this->charset.c_str()<<endl<<"sub_time: "<<this->sub_time<<endl<<"domain_name: "<<this->domain_name<<endl<<"url: "<<this->url<<endl<<"main text: "<<this->text<<endl;
    fout.close();
    return true;
}