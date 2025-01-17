#include <iostream>
#include <cstring>
#include "../../source/cache/Cache.hpp"

int main() {
    BlockCache cache(4096); // Инициализация блочного кэша с размером 4096 байт

    // Открываем тестовый файл
    int fd = cache.openFile("test_file.txt");
    if (fd < 0) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    // Пишем данные в файл
    const char* write_data = "Hello, Block Cache!";
    if (cache.write(fd, write_data, strlen(write_data)) < 0) {
        std::cerr << "Write operation failed.\n";
    } else {
        std::cout << "Write operation succeeded.\n";
    }

    // Читаем данные из файла
    char read_data[50] = {0}; // Буфер для чтения
    if (cache.read(fd, read_data, strlen(write_data)) < 0) {
        std::cerr << "Read operation failed.\n";
    } else {
        std::cout << "Read data: " << read_data << "\n";
    }

    // Закрываем файл
    if (cache.closeFile(fd) < 0) {
        std::cerr << "Failed to close file.\n";
    } else {
        std::cout << "File closed successfully.\n";
    }

    return 0;
}
