#include <iostream>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

#pragma omp sections
{

#pragma omp section
{
    for(int i=0;i<100000;i++) {
        std::cout << "section 1 :" << i << std::endl;
    }
}

#pragma omp section
{
    for(int i=0;i<100000;i++) {
        std::cout << "section 2 :" << i << std::endl;
    }
}

}
    return 0;
}
//-------------------------------------------------------------------
