#include <iostream>

int main(){

#ifdef PRL
    std::cout << "Paralell" << std::endl;
#else
    std::cout << "serial" << std::endl;
#endif

    return 0;
}
