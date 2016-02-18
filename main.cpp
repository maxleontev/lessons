#include <iostream>
#include <list>

#include <tbb/parallel_do.h>

//-------------------------------------------------------------------
void Foo(int k) {
    std::cout << k << std::endl;
}
//-------------------------------------------------------------------
void SerialApplyFooToList(const std::list <int> & list) {
    for( std::list<int>::const_iterator it = list.begin(); it != list.end() ; it++  ){
        Foo(*it);
    }
}
//-------------------------------------------------------------------
class ApplyFoo {
public:
    void operator()(const int & item) const { Foo(item); }
};
//-------------------------------------------------------------------
void ParallelApplyFooToList(const std::list <int> & list) {
    tbb::parallel_do(list.begin(), list.end(), ApplyFoo());
}
//-------------------------------------------------------------------
int main() {
    std::list <int> list;
    for(int i=0;i<500;i++)
        list.push_back(i);

//    SerialApplyFooToList(list);
    ParallelApplyFooToList(list);

    return 0;
}
//-------------------------------------------------------------------
