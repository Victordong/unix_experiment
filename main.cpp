#include <iostream>
#include <fstream>
#include "parser.h"

int main() {
    char swap[1000000];
    char c;
    int i=0;
    ifstream readFile("/Users/zhandong/CLionProjects/untitled7/data.txt");
    while(!readFile.eof()) {
        readFile>>c;
        swap[i] = c;
        i++;
    }
    Parser p = Parser("test", string(swap));
    p.Parse();
    return 0;
}