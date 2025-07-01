#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <opencv2/opencv.hpp>

// 链表节点定义
struct TermNode {
    int coefficient;
    int exponent;
    TermNode* next;
    TermNode(int c, int e) : coefficient(c), exponent(e), next(nullptr) {}
};

// 交替数组法
struct PolyArray {
    std::vector<int> data; // [coef1, exp1, coef2, exp2, ...]
};

// 链表加法
TermNode* addPolynomialsLinkedList(TermNode* p1, TermNode* p2);

// 链表乘法（最小堆优化）
TermNode* multiplyPolynomialsLinkedList(TermNode* p1, TermNode* p2);

// 可视化绘图
void visualizePolynomialOperation(TermNode* p1, TermNode* p2, TermNode* result, const std::string& title);

// 构造链表形式多项式
TermNode* createPolynomialFromPairs(const std::vector<std::pair<int, int>>& terms);

// 工具函数
void deletePolynomial(TermNode* head);

// 同时可视化 P1, P2, P1+P2, P1*P2
void visualizePolynomialsTogether(TermNode* p1, TermNode* p2, TermNode* sum, TermNode* product);

#endif
