#include "HashTable.h"

HashNode::HashNode(const std::string& k, int v) : key(k), value(v) {}

HashTable::HashTable(size_t sz) : size(sz), table(sz) {}

size_t HashTable::hash(const std::string& key) const {
    const int p = 31;
    size_t hash_val = 0;
    size_t pow_p = 1;
    for (char c : key) {
        hash_val = (hash_val + (c - 'a' + 1) * pow_p) % size;
        pow_p = (pow_p * p) % size;
    }
    return hash_val;
}

void HashTable::insert(const std::string& key, int value) {
    size_t idx = hash(key);
    for (const auto& node : table[idx]) {
        if (node.key == key) return;  // already exists
    }
    table[idx].emplace_back(key, value);
}

int HashTable::get(const std::string& key) const {
    size_t idx = hash(key);
    for (const auto& node : table[idx]) {
        if (node.key == key) return node.value;
    }
    return -1;  // not found
}

void HashTable::forEach(std::function<void(const std::string&, int)> callback) const {
    for (const auto& bucket : table) {
        for (const auto& node : bucket) {
            callback(node.key, node.value);
        }
    }
}
