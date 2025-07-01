#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <opencv2/opencv.hpp>

// ����ڵ㶨��
struct TermNode {
    int coefficient;
    int exponent;
    TermNode* next;
    TermNode(int c, int e) : coefficient(c), exponent(e), next(nullptr) {}
};

// �������鷨
struct PolyArray {
    std::vector<int> data; // [coef1, exp1, coef2, exp2, ...]
};

// ����ӷ�
TermNode* addPolynomialsLinkedList(TermNode* p1, TermNode* p2);

// ����˷�����С���Ż���
TermNode* multiplyPolynomialsLinkedList(TermNode* p1, TermNode* p2);

// ���ӻ���ͼ
void visualizePolynomialOperation(TermNode* p1, TermNode* p2, TermNode* result, const std::string& title);

// ����������ʽ����ʽ
TermNode* createPolynomialFromPairs(const std::vector<std::pair<int, int>>& terms);

// ���ߺ���
void deletePolynomial(TermNode* head);

// ͬʱ���ӻ� P1, P2, P1+P2, P1*P2
void visualizePolynomialsTogether(TermNode* p1, TermNode* p2, TermNode* sum, TermNode* product);

#endif
