#include "Huffman.h"
#include <queue>
#include <fstream>
#include <opencv2/opencv.hpp>

// �Ƚ�����������С��
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) { return a->freq > b->freq; }
};

HuffmanNode* buildHuffmanTree(const std::unordered_map<float, int>& frequencies) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> minHeap;
    for (const auto& kv : frequencies) {
        minHeap.push(new HuffmanNode(kv.first, kv.second));
    }

    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();
        HuffmanNode* parent = new HuffmanNode(0.0f, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        minHeap.push(parent);
    }
    return minHeap.top();
}

void generateCodebook(HuffmanNode* node, const std::string& code,
    std::unordered_map<float, std::string>& codebook) {
    if (!node) return;
    if (!node->left && !node->right) {
        codebook[node->value] = code;
        return;
    }
    generateCodebook(node->left, code + "0", codebook);
    generateCodebook(node->right, code + "1", codebook);
}

void visualizeCodebook(const std::unordered_map<float, std::string>& codebook) {
    int height = 40 * (int)codebook.size();
    int width = 600;
    cv::Mat img(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    int y = 30;
    for (const auto& kv : codebook) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Value: %.6f -> Code: %s", kv.first, kv.second.c_str());
        cv::putText(img, buf, cv::Point(10, y), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 1);
        y += 40;
    }
    cv::imshow("Huffman Codebook", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void freeHuffmanTree(HuffmanNode* node) {
    if (!node) return;
    freeHuffmanTree(node->left);
    freeHuffmanTree(node->right);
    delete node;
}

bool saveCompressedFile(const std::string& filename,
    const std::unordered_map<float, std::string>& codebook,
    const std::vector<float>& data) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;

    // д�������С
    uint32_t tableSize = codebook.size();
    out.write(reinterpret_cast<const char*>(&tableSize), sizeof(uint32_t));

    // д����������
    for (const auto& kv : codebook) {
        out.write(reinterpret_cast<const char*>(&kv.first), sizeof(float));           // д�븡��ֵ
        uint8_t codeLen = kv.second.size();
        out.write(reinterpret_cast<const char*>(&codeLen), 1);                        // д����볤��
        out.write(kv.second.c_str(), codeLen);                                        // д������ַ���
    }

    // ��������Ϊ������
    std::string bitstream;
    for (float f : data) bitstream += codebook.at(f);

    // �ѱ�������8λ���������ֽ�
    std::vector<uint8_t> packed;
    uint8_t currentByte = 0;
    int bitCount = 0;
    for (char bit : bitstream) {
        currentByte <<= 1;
        if (bit == '1') currentByte |= 1;
        bitCount++;
        if (bitCount == 8) {
            packed.push_back(currentByte);
            currentByte = 0;
            bitCount = 0;
        }
    }
    if (bitCount != 0) { // �����8λ���ֽ�
        currentByte <<= (8 - bitCount);
        packed.push_back(currentByte);
    }

    uint64_t bitsTotal = bitstream.size();
    out.write(reinterpret_cast<const char*>(&bitsTotal), sizeof(uint64_t));  // д��������ܳ���
    out.write(reinterpret_cast<const char*>(packed.data()), packed.size());  // д�����������
    return true;
}

bool decompressFile(const std::string& filename, std::vector<float>& restored) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    // ��ȡ������С
    uint32_t tableSize = 0;
    in.read(reinterpret_cast<char*>(&tableSize), sizeof(uint32_t));

    // ��ȡ���������
    std::unordered_map<std::string, float> codebook;
    for (uint32_t i = 0; i < tableSize; ++i) {
        float value;
        uint8_t codeLen;
        in.read(reinterpret_cast<char*>(&value), sizeof(float));
        in.read(reinterpret_cast<char*>(&codeLen), 1);
        std::string code(codeLen, ' ');
        in.read(&code[0], codeLen);
        codebook[code] = value;
    }

    // ��ȡ�������ܳ���
    uint64_t bitsTotal = 0;
    in.read(reinterpret_cast<char*>(&bitsTotal), sizeof(uint64_t));

    // ��ȡ����������
    std::vector<uint8_t> packed((bitsTotal + 7) / 8);
    in.read(reinterpret_cast<char*>(packed.data()), packed.size());

    // ������������
    std::string currentCode;
    uint64_t bitsRead = 0;
    for (uint8_t byte : packed) {
        for (int i = 7; i >= 0; --i) {
            if (bitsRead >= bitsTotal) break;
            char bit = ((byte >> i) & 1) ? '1' : '0';
            currentCode += bit;
            if (codebook.count(currentCode)) {
                restored.push_back(codebook[currentCode]);
                currentCode.clear();
            }
            bitsRead++;
        }
    }
    return true;
}

bool compressWeights(const std::vector<float>& weights, const std::string& compressedFile) {
    if (weights.empty()) return false;
    std::unordered_map<float, int> freqMap;
    for (float v : weights) freqMap[v]++;
    HuffmanNode* root = buildHuffmanTree(freqMap);
    std::unordered_map<float, std::string> codebook;
    generateCodebook(root, "", codebook);
    visualizeCodebook(codebook);
    bool ok = saveCompressedFile(compressedFile, codebook, weights);
    freeHuffmanTree(root);
    return ok;
}

bool decompressWeights(const std::string& compressedFile, std::vector<float>& restoredWeights) {
    return decompressFile(compressedFile, restoredWeights);
}
