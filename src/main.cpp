#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include "request.h"
#include "work_list.h"
using namespace std;

int main() {
    WorkList work_list = WorkList();
    work_list.PutWillDo("http://www.gmw.cn/content/2019-01/11/content_32335393.htm");
    int sum=0;
    while(!work_list.WillDoEmpty() && sum<10) {
        string url = work_list.GetWillDo();
        Request req = Request(url);
        Response res = req.Get();
        string body = res.GetBody();
        Parser parser = Parser(url, body);
        parser.Parse();
        set<string> all_urls= parser.GetAllUrls();
        set<string>::iterator url_cursor;
        for (url_cursor = all_urls.begin();url_cursor != all_urls.end();) {
            work_list.PutWillDo((*url_cursor));
        }
        work_list.PutHasdone(url);
        cout<<parser.GetAuthor()<<" "<<parser.GetCharset()<<" "<<parser.GetSubTime()<<endl;
        sum++;
    }
}