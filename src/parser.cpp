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

bool RemoveAllTag(list<char> *handle_text, list<char>::iterator begin, list<char>::iterator end) {
    list<char>::iterator cursor;
    bool tag_del = false;
    for (cursor = begin; cursor != end;) {
        if (*cursor == '<') {
            tag_del = true;
        }
        if (tag_del) {
            handle_text->erase(cursor);
        }
        if (tag_del && *cursor == '>') {
            tag_del = false;
        }
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
    char key[20] = {0};
    char value[1000] = {0};
    char title[500] = {0};
    char charset[20] = {0};
    int header_num = 0;
    bool has_name = false;
    bool has_value = false;
    bool success;
    for (cursor = (*header_text).begin();cursor != (*header_text).end();) {
        if (BeginWith(cursor, "<meta") || BeginWith(cursor, "<META")) {
            in_meta = true;
        }
        if (BeginWith(cursor, "<title")) {
            cursor = GetData(cursor, title, '>', '<');
            string title_s = title;
            (*header).insert(pair<string, string>("title", title_s));
            success = ResetSwap(title, 200);
        }
        if (in_meta && BeginWith(cursor, "name")) {
            has_name = true;
            cursor = GetData(cursor, key, '\"', '\"');
        } else if (in_meta && BeginWith(cursor, "content")) {
            has_value = true;
            cursor = GetData(cursor, value, '\"', '\"');
        } else if (in_meta && BeginWith(cursor, "charset")) {
            cursor = GetData(cursor, charset, '\"', '\"');
            string charset_s = charset;
            (*header).insert(pair<string, string>("charset", charset_s));
            success = ResetSwap(charset, 20);
        }
        if (has_name && has_value) {
            string value_s = value;
            string key_s = key;
            (*header).insert(pair<string, string>(key_s, value_s));
            has_name = false;
            has_value = false;
        }
        if (in_meta && *cursor == '>') {
            in_meta = false;
            success = ResetSwap(key, 20);
            success = ResetSwap(value, 200);
            has_name = false;
            has_value = false;
        }
        cursor++;
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
            begin_a = false;
        }
        ++cursor;
    }
    return true;
}

string HandleUrl(string origin_url, string url) {
    if (url.find('.') == url.npos) {
        return "";
    } else if (url.find("..") != url.npos) {
        return "";
    } else {
        return url;
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
        string handled_url = HandleUrl(origin, (*cursor));
        if (handled_url != "") {
            handled_urls->insert(handled_url);
        }
        ++cursor;
    }
    set<string>::iterator set_cursor;
    return true;
}

bool MakeBlock(list<int> *block, list<char> *handle_text, int num, float value) {
    list<char>::iterator cursor;
    bool tag = false;
    int tag_num = 0;
    int all_num = 0;
    int block_number = 0;
    for (cursor = handle_text->begin(); cursor != handle_text->end();) {
        if (*cursor == '<') {
            tag = true;
        }
        if (tag) {
            tag_num++;
        }
//        cout<<*cursor;
        if (tag && *cursor == '>') {
            tag = false;
        }
        all_num++;
        if (all_num == num) {
//            cout<<endl;
            float percent = float(num - tag_num) / num;
            if (percent > value) {
                (*block).push_back(block_number);
            }
            block_number++;
            tag_num = 0;
            all_num = 0;
        }
        ++cursor;
    }
//    cout<<endl;
    return true;
}


list<char>::iterator GetMidCursor(list<char> *handle_text, int num, float value) {
    list<char>::iterator cursor;
    list<int>::iterator block_cursor;
    list<int> block;
    bool success = MakeBlock(&block, handle_text, num, value);
    int series = 1;
    int longest = 1;
    block_cursor = block.begin();
    int which = *block_cursor;
    int before = *block_cursor;
    block_cursor++;
    for (; block_cursor != block.end();) {
        if (*block_cursor - before == 1) {
            series = series + 1;
            if (series > longest) {
                longest = series;
                which = *block_cursor - series;
            }
        }
        before = *block_cursor;
        block_cursor++;
    }
    int cursor_number = int((which + longest) / 2 * num);
    for (cursor = handle_text->begin(); cursor_number >= 0; cursor_number--) {
        cursor++;
    }
    return cursor;
}

list<char>::iterator
GetBeginEnd(list<char> *handle_text, list<char>::iterator mid, bool postion, list<char>::iterator text_begin,
            list<char>::iterator text_end) {
    list<char>::iterator cursor;
    list<char>::iterator calculate;
    list<char>::iterator where;
    int cur = 0;
    int calculate_max = 0;
    bool tag = false;
    for (cursor = text_begin; cursor != text_end; cur++) {
        int calculate_num = 0;
        for (calculate = text_begin; calculate != text_end;) {
            if (calculate == cursor) {
                postion = !postion;
            }
            if (*calculate == '<') {
                tag = true;
            }
            if (tag && postion) {
                calculate_num++;
                if (calculate_num > calculate_max) {
                    calculate_max = calculate_num;
                    where = cursor;
                }
            }
            if (!tag && !postion) {
                calculate_num++;
                if (calculate_num > calculate_max) {
                    calculate_max = calculate_num;
                    where = cursor;
                }
            }
            if (tag && *calculate == '>') {
                tag = false;
            }
            ++calculate;
        }
        ++cursor;
    }
    return where;
}


bool GetMainText(list<char>* handle_text, char * swap) {
    list<char>::iterator cursor;
    list<char>::iterator mid = GetMidCursor(handle_text, 50, 0.95);
    list<char>::iterator begin = GetBeginEnd(handle_text, mid, true, handle_text->begin(), mid);
    list<char>::iterator end = GetBeginEnd(handle_text, mid, false, mid, handle_text->end());
    bool success = RemoveAllTag(handle_text, begin, end);

    int i = 0;
    for (cursor = begin; cursor != end; i++) {
        if (i>99990) {
            return false;
        }
        swap[i] = *cursor;
        ++cursor;
    }
    return true;
}


bool Parser::Parse() {
    list<char> handle_text;
    list<char> handle_header;
    map<string, string> headers;
    list<char>::iterator cursor;
    bool success = InitHandleText(&handle_text, &handle_header, this->raw_text);
    cout<<"init handle text successful" <<endl;
    ParseHeader(&handle_header, &headers);
    cout<<"parse header text successful" <<endl;
    this->author = GetFromMap(&headers, "author");
    this->title = GetFromMap(&headers, "title");
    this->summary = GetFromMap(&headers, "description");
    this->sub_time = GetFromMap(&headers, "publishdate");
    this->charset = GetFromMap(&headers, "charset");
    success = RemoveDivNoteScript(&handle_text);
    cout<<"remove tag successful" <<endl;
    vector<string> unhandle_urls;
    set<string> handled_urls;
    success = GetUrlsUnHandle(&handle_text, &unhandle_urls);
    success = HandleUrls(origin_url, &unhandle_urls, &handled_urls);
    set<string>::iterator url_cursor;
    for (url_cursor = handled_urls.begin(); url_cursor != handled_urls.end();) {
        this->all_urls.insert(*url_cursor);
        ++url_cursor;
    }
    cout<<"get urls successful" <<endl;
    if(this->author != "") {
        char swap[100000];
        success = GetMainText(&handle_text, swap);
        if (!success) {
            cout<<"get main text successful" <<endl;
            return false;
        }
        this->main_text = swap;
        cout<<"get main text successful" <<endl;
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
        url_cursor++;
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
        url_cursor++;
    }
    return url_filter;
}

string Parser::GetSubTime() {
    return sub_time;
}

string Parser::GetCharset() {
    return charset;
}