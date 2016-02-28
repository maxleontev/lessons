#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "html_parcer.h"

//-------------------------------------------------------------------
using namespace std;
//-------------------------------------------------------------------
#define DEFAULT_INDEX_FILE_NAME_1 "index.html"
#define DEFAULT_INDEX_FILE_NAME_2 "index.htm"

string htmlFilesDir;

//-------------------------------------------------------------------
int HtmlParcer(char * buff, ssize_t size) {

    string input (buff, size);
    istringstream str_stream(input);

    vector <string> inp_cmd;
    string s;
    while(str_stream >> s)
        inp_cmd.push_back(s);

    string res;

    if(inp_cmd[0] == "GET") {
        res = "HTTP/1.0 404 Not Found\nConnection: close\n\n";

        string full_path = htmlFilesDir;
        if(inp_cmd[1] == "/") {
            full_path = htmlFilesDir + string("/") + string(DEFAULT_INDEX_FILE_NAME_1);
            FILE * f = fopen(full_path.c_str(), "r");
            if(f == NULL) {
                full_path = htmlFilesDir + string("/") + string(DEFAULT_INDEX_FILE_NAME_2);
                f = fopen(full_path.c_str(), "r");
            }
            if(f != NULL) {
                fclose(f);
                std::ifstream file(full_path.c_str());
                std::string str_file( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()) );
                res = "HTTP/1.0 200 Ok\nContent-length:" + std::to_string( str_file.length() );
                res += "\nConnection: close\nContent-Type: text/html\n\n";
                res += str_file;
            }
        } else {
            size_t found = inp_cmd[1].rfind(".html");
            size_t sum = found + 5;
            if(found == string::npos) {
                found = inp_cmd[1].rfind(".htm");
                sum = found + 4;
            }
            if(found != string::npos)
            {
                full_path = htmlFilesDir + inp_cmd[1].substr(0, sum);
                FILE * f = fopen(full_path.c_str(), "r");
                if(f != NULL) {
                    fclose(f);
                    std::ifstream file(full_path.c_str());
                    std::string str_file( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()) );
                    res = "HTTP/1.0 200 Ok\nContent-length:" + std::to_string( str_file.length() );
                    res += "\nConnection: close\nContent-Type: text/html\n\n";
                    res += str_file;
                }
            }
        }
    } else {
        res = "HTTP/1.0 403 Forbidden\nConnection: close\n\n";
    }

    sprintf(buff, "%s", res.c_str() );

    return res.length();
}
//-------------------------------------------------------------------
