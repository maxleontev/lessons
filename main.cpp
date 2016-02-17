#include <iostream>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

    long long sum = 0;
    for(int i=0; i<500000; i++) {
        sum = sum + i;
    }
    std::cout << sum << std::endl;


#pragma omp parallel num_threads(3)
{
    sum = 0;
#pragma opm parallel for reduction(+:sum)
    for(int i=0; i<500000; i++) {
        sum = sum + i;
    }
}
    std::cout << sum << std::endl;
    return 0;
}
//-------------------------------------------------------------------
