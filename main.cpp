#include <vector>
#include <iostream>

#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>

//-------------------------------------------------------------------
class SumFoo {
    const int * my_arr;
public:
    int my_sum;
    void operator()(const tbb::blocked_range<size_t> & r)  {
        int * a = (int *) my_arr;
        int sum = my_sum;
        size_t end = r.end();
        for(size_t i=r.begin(); i!=r.end(); ++i)
            sum += a[i];
        my_sum = sum;
    }

    SumFoo(SumFoo &x, tbb::split) : my_arr(x.my_arr), my_sum(0) {
    }

    void join(const SumFoo &y) {
        my_sum += y.my_sum;
    }
    SumFoo(const int a[]) : my_arr(a), my_sum(0) {}
};
//-------------------------------------------------------------------
int ParallelSumFoo(const int arr[], size_t size) {
    SumFoo sf(arr);
    tbb::parallel_reduce(tbb::blocked_range<size_t>(0, size), sf);
    return sf.my_sum;
}
//-------------------------------------------------------------------
int main() {
    std::vector <int> data;
    for(int i=0;i<500;i++)
        data.push_back(i);

    int res = ParallelSumFoo( data.data(), data.size() );
    std::cout << res << std::endl;

    return 0;
}
//-------------------------------------------------------------------
