#include <iostream>
#include <chrono>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

bool SearchFile(const fs::path& directory, const std::string& filename) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.path().filename() == filename) {
            return true;
        }
    }
    return false;
}

int main() {
    const std::string directory = "/home/enderdissa/Documents/";
    const std::string filename = "target_file.txt";

    auto start = std::chrono::high_resolution_clock::now();

    bool found = SearchFile(directory, filename);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Search completed in " << elapsed.count() << " seconds\n";
    if (found) {
        std::cout << "File found.\n";
    } else {
        std::cout << "File not found.\n";
    }

    return 0;
}
