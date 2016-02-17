#include <iostream>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

#pragma omp parallel num_threads(4)
{
#pragma omp for ordered
    for(int i=0; i< 10000000;i++) {
        std::cout << "Cycle " << i << std::endl;
        #pragma omp ordered
        {
            std::cout << "             " << i << std::endl;
        }
    }
}

    return 0;
}
//-------------------------------------------------------------------
