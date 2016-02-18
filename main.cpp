#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

using namespace std;
//-------------------------------------------------------------------

void Foo(int k) {
    std::cout << k << std::endl;
}
//-------------------------------------------------------------------
void SerialApplyFoo(int arr[], size_t size) {
    for(size_t i=0; i<size; i++)
        Foo(arr[i]);
}
//-------------------------------------------------------------------
class ApplyFoo {
    int * my_a;
public:
    void operator()(const tbb::blocked_range<size_t> & r) const {
        int * a = my_a;
        for(size_t i=r.begin(); i!=r.end(); ++i)
            Foo(a[i]);
    }
    ApplyFoo(int a[]) : my_a(a) {}
};
//-------------------------------------------------------------------
void ParallelApplyFoo(int arr[], size_t size) {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, size, 50), ApplyFoo(arr) );
}
//-------------------------------------------------------------------
/*void ParallelApplyFoo(int arr[], size_t size) {
    tbb::parallel_for(tbb::blocked_range<size_t>(0, size),
    [=] (const tbb::blocked_range<size_t>&r) {
        for(size_t i=r.begin(); i!=r.end(); i++)
            Foo(arr[i]);
    } );
}*/
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

    ParallelApplyFoo(data.data(), data.size());
//    SerialApplyFoo(data.data(), data.size());

    ofstream of_str("out.txt");
    for(auto it : data) {
        of_str << it;
        if(it != data.back())
            of_str << " ";
    }

    return 0;
}
//-------------------------------------------------------------------
