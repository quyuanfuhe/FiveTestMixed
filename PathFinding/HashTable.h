#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <list>
#include <functional>

struct HashNode {
    std::string key;
    int value;
    HashNode(const std::string& k, int v);
};

class HashTable {
private:
    size_t size;
    std::vector<std::list<HashNode>> table;

    size_t hash(const std::string& key) const;

public:
    explicit HashTable(size_t sz = 1009);
    void insert(const std::string& key, int value);
    int get(const std::string& key) const;

    // 安全遍历所有词元及其ID
    void forEach(std::function<void(const std::string&, int)> callback) const;
};

#endif // HASHTABLE_H
