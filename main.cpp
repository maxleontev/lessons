#include <iostream>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

int n=0;
#pragma omp parallel num_threads(2)
{

    // atomic operation
    #pragma omp atomic
        n++;

    std::cout << n << std::endl;
}
    return 0;
}
//-------------------------------------------------------------------
