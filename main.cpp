#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

//-------------------------------------------------------------------
void Merge(vector <int>& v, int a_bg, int a_ln, int b_bg, int b_ln, vector <int>& c) {
    int i=a_bg;
    int j=b_bg;
    while( i < (a_bg+a_ln) && j < (b_bg+b_ln) ) {
        if( v[i] <= v[j] ) {
            c[(i-a_bg)+(j-b_bg)] = v[i];
            i++;
        } else {
            c[(i-a_bg)+(j-b_bg)] = v[j];
            j++;
        }
    }

    if( i == (a_bg+a_ln)) {
        for(; j<b_bg+b_ln; j++)
            c[(i-a_bg)+(j-b_bg)] = v[j];
    } else if( j == (b_bg+b_ln) ) {
        for(; i<a_bg+a_ln; i++)
            c[(i-a_bg)+(j-b_bg)] = v[i];
    }
}
//-------------------------------------------------------------------
void MergeSort(std::vector <int> & v, int bg, int length) {
    if(length <= 1)
        return;
    int fL = length / 2;
    int sL = length - fL;

#pragma omp parallel
{
    #pragma omp sections
    {
        #pragma omp section
        {
            MergeSort(v, bg, fL);
        }
        #pragma omp section
        {
            MergeSort(v, bg + fL, sL);
        }
    }
}

    std::vector <int> c(length);

    Merge(v, bg, fL, bg+fL, sL, c);

    for(int i=0; i<length; i++)
        v[bg + i] = c[i];
}
//-------------------------------------------------------------------
int main(int argc, char ** argv) {

    if(argc < 2) {
        cout << "argument is absent" << endl;
        return 0;
    }
    int f;
    if( (f = open(argv[1], O_RDWR)) == -1) {
        perror("open file error : ");
        return 0;
    }
    if( close(f) == -1 ) {
        perror("error closing input file : ");
        return 0;
    }

    ifstream if_str(argv[1]);

    int k;
    vector <int> data;
    while(if_str >> k)
        data.push_back(k);

    MergeSort(data, 0, data.size());

    ofstream of_str("out.txt");
    for(auto it : data) {
        of_str << it;
        if(it != data.back())
            of_str << " ";
    }
    of_str << endl;
    cout << "result has been saved" << endl;

    return 0;
}
//-------------------------------------------------------------------
