#include <iostream>
#include <cstdlib>
#include <matrix.hpp>
int main() {

    Matrix m = {1,2,3};

    std::cout << (m + 10);
    return 0;
}