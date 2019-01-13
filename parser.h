//
// Created by zhandong on 2019/1/12.
//

#ifndef UNTITLED7_PARSER_H
#define UNTITLED7_PARSER_H

#endif //UNTITLED7_PARSER_H

#include <string>
#include <set>
#include <list>

using namespace std;

class Parser {
private:
    string origin_url;
    string raw_text;
    string charset;
    string author;
    string sub_time;
    string summary;
    string title;
    string main_text;
    set<string> all_urls;
public:
    Parser(string url, string text);
    ~Parser();
    bool Parse(); //解析原文
    string GetArticleTitle(); //获取标题
    string GetSummary(); //获取摘要
    string GetCharset(); //获取编码方式
    string GetSubTime(); //获取发布时间
    string GetAuthor(); // 获取作者
    string GetMainBody(); // 获取正文
    set<string> GetAllUrls(); //获取全部url
    set<string> GetUrlsFilter(string filter_text); //通过filter_text进行筛选过的url

};
