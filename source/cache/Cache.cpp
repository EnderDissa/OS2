#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include "Cache.hpp"

BlockCache::BlockCache(size_t cache_size) : cache_size_(cache_size) {}

BlockCache::~BlockCache() {
    for (auto& block : cache_) {
        if (block.dirty) {
            lseek(block.fd, block.offset, SEEK_SET);
            write(block.fd, block.data.data(), block.data.size());
        }
    }
}

int BlockCache::openFile(const std::string& path) {
    int fd = open(path.c_str(), O_RDWR | O_DIRECT); 
    if (fd < 0) {
        if (errno != EACCES && errno != EISDIR) {
            perror("open");
        }
        return -1;
    }
    file_map_[fd] = path;
    return fd;
}

int BlockCache::closeFile(int fd) {
    auto it = file_map_.find(fd);
    if (it == file_map_.end()) {
        return -1; 
    }

    for (auto cache_it = cache_.begin(); cache_it != cache_.end();) {
        if (cache_it->fd == fd) {
            cache_it = cache_.erase(cache_it); 
        } else {
            ++cache_it; 
        }
    }

    file_map_.erase(it);
    close(fd);

    return 0;
}



ssize_t BlockCache::read(int fd, void* buf, size_t count) {
    if (file_map_.find(fd) == file_map_.end()) {
        std::cerr << "FD not found\n";
        return -1;
    }

    off_t offset = lseek(fd, 0, SEEK_CUR);
    if (offset < 0) {
        perror("lseek failed");
        return -1;
    }

    int block_index = findBlock(fd, offset);

    if (block_index != -1) {
        memcpy(buf, cache_[block_index].data.data(), count);
        cache_[block_index].second_chance = true;
        return count;
    } else {
        ssize_t bytes = loadBlock(fd, offset);
        if (bytes > 0) {
            memcpy(buf, cache_.back().data.data(), count);
        }
        return bytes;
    }
}

ssize_t BlockCache::write(int fd, const void* buf, size_t count) {
    off_t offset = lseek(fd, 0, SEEK_CUR);
    if (offset < 0) {
        perror("lseek failed");
        return -1;
    }

    int block_index = findBlock(fd, offset);

    if (block_index != -1) {
        memcpy(cache_[block_index].data.data(), buf, count);
        cache_[block_index].second_chance = true;
        cache_[block_index].dirty = true; // Mark block as dirty
        return count;
    } else {
        evictBlock();
        CacheBlock new_block;
        new_block.data.resize(count);
        memcpy(new_block.data.data(), buf, count);
        new_block.fd = fd;
        new_block.offset = offset;
        new_block.dirty = true; // New block is dirty
        new_block.second_chance = false;
        cache_.push_back(new_block);
        return count;
    }
}

int BlockCache::fsync(int fd) {
    for (auto& block : cache_) {
        if (block.fd == fd && block.dirty) {
            lseek(block.fd, block.offset, SEEK_SET);
            write(block.fd, block.data.data(), block.data.size());
            block.dirty = false; // Clear dirty flag
        }
    }
    return ::fsync(fd);
}

int BlockCache::findBlock(int fd, off_t offset) {
    for (size_t i = 0; i < cache_.size(); ++i) {
        if (cache_[i].fd == fd && cache_[i].offset == offset) {
            return i;
        }
    }
    return -1;
}

void BlockCache::evictBlock() {
    for (size_t i = 0; i < cache_.size(); ++i) {
        if (!cache_[i].second_chance) {
            if (cache_[i].dirty) {
                lseek(cache_[i].fd, cache_[i].offset, SEEK_SET);
                write(cache_[i].fd, cache_[i].data.data(), cache_[i].data.size());
            }
            cache_.erase(cache_.begin() + i);
            return;
        } else {
            cache_[i].second_chance = false;
        }
    }
}

ssize_t BlockCache::loadBlock(int fd, off_t offset) {
    evictBlock();
    CacheBlock new_block;
    new_block.data.resize(4096); 
    new_block.fd = fd;
    new_block.offset = offset;
    new_block.dirty = false; // New block is clean
    new_block.second_chance = false;

    ssize_t bytes = pread(fd, new_block.data.data(), 4096, offset);
    if (bytes > 0) {
        cache_.push_back(new_block);
    }
    return bytes;
}

void BlockCache::recordStatistics(int fd, const std::string& info) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    for (auto& block : cache_) {
        if (block.fd == fd) {
            std::string time_str = std::ctime(&now); 
            time_str.pop_back(); 
            std::cout << "Block FD: " << fd << ", Info: " << info << ", Time: " << time_str << "\n";
            return;
        }
    }
    std::cerr << "Block not found for FD: " << fd << "\n";
}
