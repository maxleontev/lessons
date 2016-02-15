#include <iostream>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>

//-------------------------------------------------------------------
using namespace std;

vector<pair<string, string>> parse_input(istream & input_stream) {
    string str;
    getline(input_stream, str);
    vector<string> inp;

    int i = 0;
    while(1) {
        int k = str.find('|', i);
        if(k == string::npos) {
            inp.push_back(str.substr(i, str.length()-i));
            break;
        }
        inp.push_back(str.substr(i, k-i));
        i = k+1;
    }
    vector<pair<string, string>> res;

    for(auto it : inp) {
        stringstream ss(it);
        if( ss >> str ) {
            string st1;
            string st2;
            while(ss >> st1) {
                if(st2 == "")
                    st2 = st1;
                else
                    st2 = st2 + " " + st1;
            }
            res.push_back(pair<string, string>(str, st2));
        }
    }

    return res;
}
//-------------------------------------------------------------------
void ex_prg(pair<string,string> & pair)
{
    stringstream params_s(pair.second);
    string s_tmp;
    vector<string> params_v;
    while(params_s >> s_tmp)
        params_v.push_back(s_tmp);

    char ** arg = new char*[params_v.size()+2];

    arg[0] = new char[pair.first.size() + 1];
    for(int i = 0; i < pair.first.size(); i ++)
        arg[0][i] = pair.first[i];
    arg[0][pair.first.size()] = '\0';

    for(int i=0;i<params_v.size();i++) {
        arg[i+1] = new char[params_v[i].size() + 1];
        for(int j = 0; j < params_v[i].size(); j ++)
            arg[i+1][j] = params_v[i][j];
        arg[i+1][params_v[i].size()] = '\0';
    }
    arg[params_v.size()+1] = (char *) 0;

    execvp(pair.first.c_str(), arg);
}
//-------------------------------------------------------------------
int main(int argc, char *argv[]) {

    vector<pair<string,string>> comm_v = parse_input(cin);
    if(comm_v.size() == 0)
        return 0;

    int ** pfd = new int*[comm_v.size()];
    for(int i=0;i<comm_v.size();i++)
        pfd[i] = new int[2];

    for(int i=0; i<comm_v.size(); i++)
    {
        pipe( pfd[i] );
        if( fork() == 0) {
            close(STDOUT_FILENO);
            dup2( pfd[i][1], STDOUT_FILENO);
            close( pfd[i][0] );
            close( pfd[i][1] );

            if(i != 0) {
                close(STDIN_FILENO);
                dup2( pfd[i-1][0], STDIN_FILENO);
                close(pfd[i-1][0] );
                close(pfd[i-1][1] );
            }
            ex_prg(comm_v[i]);
        }
        else {
            if(i!=0) {
                close( pfd[i-1][0]);
                close( pfd[i-1][1]);
            }
        }
    }
    close(pfd[comm_v.size()-1][1]);

    ofstream ofile ("/media/sf_mint/projects/qt/lessons/01/result.out");
    int ln;
    char buf2[1];
    while ((ln = read(pfd[comm_v.size()-1][0], buf2, 1)) != 0)
        ofile.write(buf2, ln);

    return 0;
}
//-------------------------------------------------------------------
