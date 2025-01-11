#ifndef FUN_HPP
#define FUN_HPP

#include <filesystem>
#include <vector>
#include <string>

// Function declarations
std::vector<int> factorized(int number);
bool searchFile(const std::filesystem::path& directory, const std::string& filename);

#endif
