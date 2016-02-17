#include <iostream>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

#pragma omp parallel num_threads(4)
{
    std::cout << 1 << std::endl;
    #pragma omp single nowait
    {
        std::cout << 2 << std::endl;
    }
    std::cout << 3 << std::endl;
}
    return 0;
}
//-------------------------------------------------------------------
