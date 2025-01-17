#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include <unordered_map>
#include <string>

struct CacheBlock{
    std::vector<char> data;
    bool second_chance;
    bool dirty;
    int fd;
    off_t offset;
};

class BlockCache{
public:
    BlockCache(size_t cache_size);
    ~BlockCache();

    int openFile(const std::string& path);
    int closeFile(int fd);
    ssize_t read(int fd, void* buf, size_t count);
    ssize_t write(int fd, const void* bug, size_t count);
    int fsync(int fd);
    void recordStatistics(int fd, const std::string& info);

private:
    size_t cache_size_;
    std::vector<CacheBlock> cache_;
    std::unordered_map<int, std::string> file_map_;

    int findBlock(int fd, off_t offset);
    void evictBlock();
    ssize_t loadBlock(int fd, off_t offset);
};



#endif //CACHE_HPP