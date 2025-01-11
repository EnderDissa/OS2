#include <iostream>
#include <stdexcept>
#include "ShellTest.cpp"

int main() {
    try {
        RunAllTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
