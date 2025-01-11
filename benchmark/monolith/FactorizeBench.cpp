#include <iostream>
#include <chrono>
#include <vector>

void Factorize(int n) {
    for (int i = 1; i <= n; ++i) {
        if (n % i == 0) {
            // Simulate work
        }
    }
}

int main() {
    const int iterations = 1000;
    const int number = 123456;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        Factorize(number);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Factorization completed in " << elapsed.count() << " seconds\n";
    return 0;
}
