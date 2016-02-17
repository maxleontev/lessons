#include <iostream>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

#pragma omp parallel num_threads(2)
{
    #pragma omp for
    for(int i=0; i<30; i++) {
    #pragma omp critical
    {
        std::cout << i << std::endl;
    }
    }

}
    return 0;
}
//-------------------------------------------------------------------
