#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

void ComputeTask(int id) {
    // Simulate work
    for (int i = 0; i < 1000000; ++i) {
        volatile int x = i * i;
    }
    std::cout << "Task " << id << " completed.\n";
}

int main() {
    const int numThreads = 4;
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(ComputeTask, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Multithreaded tasks completed in " << elapsed.count() << " seconds\n";
    return 0;
}
