#include <iostream>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

#pragma omp task
{
    for(int i=0;i<100;i++) {
        std::cout << "section 1 :" << i << std::endl;
    }
}
#pragma omp taskwait

    return 0;
}
//-------------------------------------------------------------------
