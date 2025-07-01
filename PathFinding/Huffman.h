#pragma once
#include <unordered_map>
#include <vector>
#include <string>

// 哈夫曼树节点
struct HuffmanNode {
    float value;  // 仅叶节点有效
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(float val, int f) : value(val), freq(f), left(nullptr), right(nullptr) {}
};

// 构建哈夫曼树并返回根节点
HuffmanNode* buildHuffmanTree(const std::unordered_map<float, int>& frequencies);

// 生成编码表
void generateCodebook(HuffmanNode* node, const std::string& code,
    std::unordered_map<float, std::string>& codebook);

// 可视化编码表
void visualizeCodebook(const std::unordered_map<float, std::string>& codebook);

// 释放哈夫曼树内存
void freeHuffmanTree(HuffmanNode* node);

// 保存自包含压缩文件
bool saveCompressedFile(const std::string& filename,
    const std::unordered_map<float, std::string>& codebook,
    const std::vector<float>& data);

// 从自包含文件中解码
bool decompressFile(const std::string& filename, std::vector<float>& restored);

// 模块化接口
bool compressWeights(const std::vector<float>& weights, const std::string& compressedFile);
bool decompressWeights(const std::string& compressedFile, std::vector<float>& restoredWeights);
