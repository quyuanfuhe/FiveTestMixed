#pragma once
#include <unordered_map>
#include <vector>
#include <string>

// ���������ڵ�
struct HuffmanNode {
    float value;  // ��Ҷ�ڵ���Ч
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(float val, int f) : value(val), freq(f), left(nullptr), right(nullptr) {}
};

// �����������������ظ��ڵ�
HuffmanNode* buildHuffmanTree(const std::unordered_map<float, int>& frequencies);

// ���ɱ����
void generateCodebook(HuffmanNode* node, const std::string& code,
    std::unordered_map<float, std::string>& codebook);

// ���ӻ������
void visualizeCodebook(const std::unordered_map<float, std::string>& codebook);

// �ͷŹ��������ڴ�
void freeHuffmanTree(HuffmanNode* node);

// �����԰���ѹ���ļ�
bool saveCompressedFile(const std::string& filename,
    const std::unordered_map<float, std::string>& codebook,
    const std::vector<float>& data);

// ���԰����ļ��н���
bool decompressFile(const std::string& filename, std::vector<float>& restored);

// ģ�黯�ӿ�
bool compressWeights(const std::vector<float>& weights, const std::string& compressedFile);
bool decompressWeights(const std::string& compressedFile, std::vector<float>& restoredWeights);
