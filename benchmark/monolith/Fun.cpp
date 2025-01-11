#include "Fun.hpp"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

std::vector<int> factorized(int number) {
  std::vector<int> factors;

  for (int i = 1; i <= number / 2; ++i) {
    if (number % i == 0) {
      factors.push_back(i);
    }
  }
  factors.push_back(number); 
  return factors;
}

bool searchFile(const fs::path& directory, const std::string& filename) {
  try {
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
      return false;
    }

    for (auto& entry : fs::recursive_directory_iterator(directory)) {
      if (entry.path().filename() == filename) {
        std::cout << "Found: " << entry.path() << std::endl;
        return true;
      }
    }
  } catch (const fs::filesystem_error& e) {
    std::cerr << "Filesystem error: " << e.what() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return false;
}
