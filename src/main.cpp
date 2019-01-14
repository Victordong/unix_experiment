#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include "request.h"
#include "work_list.h"
#include "data.h"
using namespace std;

int main() {
    WorkList work_list = WorkList();
    work_list.PutWillDo("http://politics.gmw.cn/2019-01/14/content_32349183.htm");
    int sum = 0;
    while (!work_list.WillDoEmpty() && sum < 10) {
        string url = work_list.GetWillDo();
        Request req = Request(url);
        Response res = req.Get();
        if (res.GetStatus() != "200 OK") {
            continue;
        }
        string body = res.GetBody();
        Parser parser = Parser(url, body);
        bool success = parser.Parse();
        set<string> all_urls = parser.GetUrlsFilter("politics.gmw.cn");
        set<string>::iterator url_cursor;
        for (url_cursor = all_urls.begin(); url_cursor != all_urls.end();) {
            work_list.PutWillDo((*url_cursor));
            url_cursor++;
        }
        cout << "put successful" << endl;
        work_list.PutHasdone(url);
        if (!success) {
            continue;
        }
        string title = parser.GetArticleTitle();
        string author = parser.GetAuthor();
        string summary = parser.GetSummary();
        string charset = parser.GetCharset();
        string sub_time = parser.GetSubTime();
        string domain = req.GetDomainName();
        string text = parser.GetMainBody();
        Data data = Data(title, author, summary, charset, sub_time, domain, url, text);
        data.ToFile();
        sum++;
    }
}