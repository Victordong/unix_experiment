//
// Created by zhandong on 2019/1/14.
//
#include "request.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>

using namespace std;

#define BUFFERSIZE 1024

vector<string> split(const string &str, const string &delim) {
    vector<string> res;
    if ("" == str)
        return res;
    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());
    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());
    char *p = strtok(strs, d);
    while (p) {
        string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }
    return res;
}

typedef struct Url {//解析后的url
    string domain;//域名
    string path;//路径
    int port;//端口
} Url;


Url ParseUrl(string url) {
    Url parsed;
    vector<string> url_parts = split(url, "/");
    if (url_parts[0].find("http", 0) != url_parts[0].npos) {
        url_parts.erase(url_parts.begin());
    }
    parsed.domain = url_parts[0];
    url_parts.erase(url_parts.begin());
    string path = "";
    for (vector<string>::iterator cursor = url_parts.begin(); cursor != url_parts.end();) {
        path = path + "/" + (*cursor);
        cursor++;
    }
    parsed.port = 80;
    parsed.path = path;
    return parsed;
}

Request::Request(string url) {
    Url parsed = ParseUrl(url);
    this->domain = parsed.domain;
    this->path = parsed.path;
    this->port = 80;
    hostent *ip = gethostbyname(this->domain.c_str());  //根据域名得到ip
    string sin_addr = ip->h_addr_list[0];
    this->sin_addr = sin_addr;
}

Request::~Request() {}

string MakeHeader(string domain, string path) {
    string h1 = "GET " + path + " HTTP/1.1\r\n";
    string h2 = "Accept: */*\r\n";
    string h3 = "Host: " + domain + "\r\n";
    string h4 = "Accept-Language: zh-cn\r\n";
    string h5 = "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36";
    string h6 = "Connection: close\r\n\r\n";
    return h1 + h2 + h3 + h4 + h5 + h6;
}


Response Request::Get() {//爬取这个request类所对应url的数据
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    string header = MakeHeader(string(this->domain), this->path);
    this->header = header;
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);//主机字节序转化为网络字节序
    memcpy(&(server_addr.sin_addr), this->sin_addr.c_str(), sizeof(server_addr.sin_addr));
    if (connect(sockfd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {//连接
        close(sockfd);
        return Response("", "");
    }
    if (send(sockfd, this->header.c_str(), this->header.length(), 0) == -1) {//发送数据
        cout << "发送数据失败\n";
        return Response("", "");
    }
    //接收数据存到recvdata中
    string recvdata = "";
    char buffer[BUFFERSIZE];
    bzero(buffer, sizeof(buffer));
    while (recv(sockfd, buffer, BUFFERSIZE, 0) > 0) {
        recvdata += buffer;
        memset(buffer, 0, sizeof(buffer));
    }
    close(sockfd);
    char resp_header[1000]= {0};
    char resp_body[20000]={0};
    unsigned long where = recvdata.find("\r\n\r\n",0);
    for (int i=0;i<where;i++) {
        resp_header[i] = recvdata[i];
        resp_body[i+1] = '\0';
    }
    while(recvdata[where]!= '<') {
        where++;
    }
    for (int i =0; where<recvdata.length(); i++) { //保存body内容
        resp_body[i] = recvdata[where];
        resp_body[i+1] = '\0';
        where ++;
    }
    return Response(resp_header, resp_body);
}

bool Request::SetUrl(string url) {
    Url parsed = ParseUrl(url);
    this->domain = parsed.domain;
    this->path = parsed.path;
    return true;
}

bool Request::SetHeader(string header) {
    this->header = header;
    return true;
}
