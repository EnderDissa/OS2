#include <iostream>
#include <chrono>
#include <string>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include "../../source/cache/Cache.hpp"

namespace fs = std::filesystem;

const size_t BLOCK_SIZE = 4096;

// Функция для создания больших тестовых файлов
void CreateLargeTestFiles(const std::string& directory, size_t fileCount, size_t fileSize) {
    for (size_t i = 0; i < fileCount; ++i) {
        std::string fileName = directory + "/large_file_" + std::to_string(i) + ".txt";
        int fd = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("Failed to create test file");
            continue;
        }

        std::string content = "Sample data block\n";
        for (size_t j = 0; j < fileSize / content.size(); ++j) {
            write(fd, content.c_str(), content.size());
        }

        close(fd);
    }
}

// Функция для случайного чтения файлов с использованием кэша
void RandomReadFilesWithCache(BlockCache& cache, const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry.path())) continue;

        int fd = cache.openFile(entry.path());
        if (fd < 0) {
            perror("Failed to open file");
            continue;
        }

        char buf[BLOCK_SIZE];
        for (size_t offset = 0; offset < 10 * BLOCK_SIZE; offset += BLOCK_SIZE) {
            cache.read(fd, buf, sizeof(buf));
        }

        cache.closeFile(fd);
    }
}

int main() {
    const std::string directory = "/home/enderdissa/Documents/test_files";
    const size_t fileCount = 10;
    const size_t fileSize = 10 * BLOCK_SIZE; // 10 блоков по 4096 байт

    // Создаём директорию для тестов
    fs::create_directory(directory);

    // Создание больших тестовых файлов
    CreateLargeTestFiles(directory, fileCount, fileSize);

    BlockCache cache(BLOCK_SIZE); // Создаём блочный кэш

    // Чтение файлов без кэша
    auto startNoCache = std::chrono::high_resolution_clock::now();
    for (const auto& entry : fs::directory_iterator(directory)) {
        int fd = open(entry.path().c_str(), O_RDONLY);
        if (fd < 0) continue;

        char buf[BLOCK_SIZE];
        for (size_t offset = 0; offset < 10 * BLOCK_SIZE; offset += BLOCK_SIZE) {
            lseek(fd, offset, SEEK_SET);
            read(fd, buf, sizeof(buf));
        }
        close(fd);
    }
    auto endNoCache = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedNoCache = endNoCache - startNoCache;
    std::cout << "Random reading without cache completed in " << elapsedNoCache.count() << " seconds\n";

    // Чтение файлов с использованием блочного кэша
    auto startWithCache = std::chrono::high_resolution_clock::now();
    RandomReadFilesWithCache(cache, directory);
    auto endWithCache = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedWithCache = endWithCache - startWithCache;
    std::cout << "Random reading with cache completed in " << elapsedWithCache.count() << " seconds\n";

    return 0;
}
