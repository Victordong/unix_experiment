//
// Created by zhandong on 2019/1/12.
//

#include "parser.h"
#include <list>
#include <vector>
#include <iostream>
#include <set>
#include <map>

using namespace std;

Parser::Parser(string url, string text) {
    origin_url = url;
    raw_text = text;
}

Parser::~Parser() {
    all_urls.clear();
}


bool BeginWith(list<char>::iterator cursor, string words) {
    for (int i = 0; i < words.length(); i++) {
        if (*cursor != words[i]) {
            return false;
        }
        ++cursor;
    }
    return true;
}


bool RemoveDivNoteScript(list<char> *handle_text) {
    list<char>::iterator cursor;
    bool div_del = false;
    bool note_del = false;
    bool script_del = false;
    bool script_end = false;
    for (cursor = (*handle_text).begin(); cursor != (*handle_text).end();) {
        if (BeginWith(cursor, "<div") || BeginWith(cursor, "</div>"))
            div_del = true;
        else if (BeginWith(cursor, "<!--"))
            note_del = true;
        else if (BeginWith(cursor, "<script")) {
            script_del = true;
            script_end = false;
        } else if (BeginWith(cursor, "</script>"))
            script_end = true;
        if (div_del || note_del || script_del) {
            (*handle_text).erase(cursor);
        }
        if (div_del && *cursor == '>')
            div_del = false;
        if (note_del && *cursor == '>')
            note_del = false;
        if (script_del && *cursor == '>' && script_end)
            script_del = false;
        ++cursor;

    }
    return true;
}

bool InitHandleText(list<char> *handle_text, list<char> *handle_header, string raw_text) {
    list<char>::iterator cursor;
    int body_begin = int(raw_text.find("<body", 0));
    for (int i = int(raw_text.find("<head>", 0)); i < body_begin; i++) {
        (*handle_header).push_back(raw_text[i]);
    }
    for (int i = int(raw_text.find(">", body_begin) + 1); i < raw_text.find("</body>", body_begin); i++) {
        (*handle_text).push_back(raw_text[i]);
    } //获取body标签内的内容
    return true;
}

bool ResetSwap(char *swap, int n) {
    for (int i = 0; i < n; i++) {
        swap[i] = '\0';
    }
    return true;
}


list<char>::iterator GetData(list<char>::iterator cursor, char *data, char begin, char end) {
    while (*cursor != begin) {
        ++cursor;
    }
    ++cursor;
    int i = 0;
    while (*cursor != end) {
        data[i] = *cursor;
        i++;
        ++cursor;
    }
    return cursor;
}

bool ParseHeader(list<char> *header_text, map<string, string> *header) {
    list<char>::iterator cursor;
    bool in_meta = false;
    char key[20];
    char value[100];
    char title[100];
    char charset[20];
    bool has_name = false;
    bool has_value = false;
    for (cursor = header_text->begin(); cursor != header_text->end();) {
        if (BeginWith(cursor, "<meta") || BeginWith(cursor, "<META")) {
            in_meta = true;
        }
        if (BeginWith(cursor, "<title")) {
            cursor = GetData(cursor,title ,'>', '<');
            (*header).insert(pair<string, string>("title", string(title)));
            ResetSwap(title, 100);
        }
        if (in_meta && BeginWith(cursor, "name")) {
            has_name = true;
            cursor = GetData(cursor, key, '\"', '\"');
        } else if (in_meta && BeginWith(cursor, "content")) {
            has_value = true;
            cursor = GetData(cursor, value, '\"', '\"');
        } else if (in_meta && BeginWith(cursor, "charset")) {
            cursor = GetData(cursor, charset, '\"', '\"');
            (*header).insert(pair<string, string>("charset", string(charset)));
            ResetSwap(charset, 20);
        }
        if (in_meta && *cursor == '>') {
            in_meta = false;
        }
        if (has_name && has_value) {
            (*header).insert(pair<string, string>(string(key), string(value)));
        } else if (!in_meta) {
            ResetSwap(key, 20);
            ResetSwap(value, 100);
        }
        ++cursor;
    }
}

list<char>::iterator GetUnHandleUrl(char *url, list<char>::iterator cursor) {
    ResetSwap(url, 100);
    while (!BeginWith(cursor, "href=\"")) {
        ++cursor;
    }
    cursor = GetData(cursor, url, '\"', '\"');
    return cursor;
}

bool GetUrlsUnHandle(list<char> *handle_text, vector<string> *unhandle_urls) {
    list<char>::iterator cursor;
    bool begin_a = false;
    char swap[100];
    int url_cursor = 0;
    for (cursor = (*handle_text).begin(); cursor != (*handle_text).end();) {
        if (BeginWith(cursor, "<a")) {
            begin_a = true;
        }
        if (begin_a) {
            cursor = GetUnHandleUrl(swap, cursor);
            string url = string(swap);
            unhandle_urls->push_back(url);
        }
        if (begin_a && *cursor == '>') {
            begin_a = false;
        }
        ++cursor;
    }
    return true;
}

string HandleUrl(string origin_url, string *url) {
    if (url->find('.') == url->npos) {
        return "";
    } else if (url->find("..") != url->npos) {
        return "";
    } else {
        return *url;
    }
}

string GetFromMap(map<string, string> *headers, string key) {
    map<string, string>::iterator iter;
    iter = (*headers).find(key);
    if (iter != (*headers).end()) {
        return iter->second;
    } else {
        return "";
    }
}

bool HandleUrls(string origin, vector<string> *unhandle_urls, set<string> *handled_urls) {
    vector<string>::iterator cursor;

    for (cursor = unhandle_urls->begin(); cursor != unhandle_urls->end();) {
        string handled_url = HandleUrl(origin, &(*cursor));
        handled_urls->insert(handled_url);
    }
    return true;
}

list<char>::iterator GetMidCursor(list<char>::iterator cursor, float value) {

}


bool Parser::Parse() {
    list<char> handle_text;
    list<char> handle_header;
    map<string, string> headers;
    list<char>::iterator cursor;
    bool success = ParseHeader(&handle_header, &headers);
    author = GetFromMap(&headers, "author");
    title = GetFromMap(&headers, "title");
    summary = GetFromMap(&headers, "description");
    sub_time = GetFromMap(&headers, "publishdate");
    charset = GetFromMap(&headers, "charset");
    success = InitHandleText(&handle_text, &handle_header, raw_text);
    success = RemoveDivNoteScript(&handle_text);
    for (cursor = handle_header.begin(); cursor != handle_header.end();) {
        cout << *cursor;
        ++cursor;
    }
    cout << endl;
    for (cursor = handle_text.begin(); cursor != handle_text.end();) {
        cout << *cursor;
        ++cursor;
    }
    cout << endl;
    vector<string> unhandle_urls;
    set<string> handled_urls;
    success = GetUrlsUnHandle(&handle_text, &unhandle_urls);
    success = HandleUrls(origin_url, &unhandle_urls, &handled_urls);
    set<string>::iterator url_cursor;
    for (url_cursor = handled_urls.begin(); url_cursor != handled_urls.end();) {
        all_urls.insert(*url_cursor);
    }
    return true;
}


string Parser::GetMainBody() {
    return main_text;
}

set<string> Parser::GetAllUrls() {
    set<string> url_filter;
    set<string> urls;
    set<string>::iterator url_cursor;
    for (url_cursor = all_urls.begin(); url_cursor != all_urls.end();) {
        urls.insert(*url_cursor);
    }
    return urls;
}

string Parser::GetAuthor() {
    return author;
}

string Parser::GetArticleTitle() {
    return title;
}

string Parser::GetSummary() {
    return summary;
}

set<string> Parser::GetUrlsFilter(string filter_text) {
    set<string> url_filter;
    set<string>::iterator url_cursor;
    for (url_cursor = all_urls.begin(); url_cursor != all_urls.end();) {
        if ((*url_cursor).find(filter_text) != (*url_cursor).npos) {
            url_filter.insert(*url_cursor);
        }
    }
    return url_filter;
}

string Parser::GetSubTime() {
    return sub_time;
}

string Parser::GetCharset() {
    return charset;
}