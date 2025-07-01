#include "Polynomial.h"
#include <iostream>
#include <queue>
#include <map>

TermNode* createPolynomialFromPairs(const std::vector<std::pair<int, int>>& terms) {
    TermNode* head = nullptr;
    TermNode* tail = nullptr;
    for (auto& term : terms) {
        TermNode* newNode = new TermNode(term.first, term.second);
        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

TermNode* addPolynomialsLinkedList(TermNode* p1, TermNode* p2) {
    TermNode dummy(0, 0);
    TermNode* current = &dummy;

    while (p1 && p2) {
        if (p1->exponent == p2->exponent) {
            int coef = p1->coefficient + p2->coefficient;
            if (coef != 0)
                current->next = new TermNode(coef, p1->exponent), current = current->next;
            p1 = p1->next;
            p2 = p2->next;
        }
        else if (p1->exponent > p2->exponent) {
            current->next = new TermNode(p1->coefficient, p1->exponent);
            p1 = p1->next;
            current = current->next;
        }
        else {
            current->next = new TermNode(p2->coefficient, p2->exponent);
            p2 = p2->next;
            current = current->next;
        }
    }

    while (p1) current->next = new TermNode(p1->coefficient, p1->exponent), current = current->next, p1 = p1->next;
    while (p2) current->next = new TermNode(p2->coefficient, p2->exponent), current = current->next, p2 = p2->next;

    return dummy.next;
}

struct HeapNode {
    int coefficient;
    int exponent;
    HeapNode(int c, int e) : coefficient(c), exponent(e) {}
    bool operator>(const HeapNode& other) const { return exponent > other.exponent; }
};

TermNode* multiplyPolynomialsLinkedList(TermNode* p1, TermNode* p2) {
    if (!p1 || !p2) return nullptr;

    std::map<int, int, std::greater<>> resultMap;

    for (TermNode* i = p1; i != nullptr; i = i->next) {
        for (TermNode* j = p2; j != nullptr; j = j->next) {
            int newCoef = i->coefficient * j->coefficient;
            int newExp = i->exponent + j->exponent;
            resultMap[newExp] += newCoef;
        }
    }

    TermNode* result = nullptr;
    TermNode* tail = nullptr;
    for (auto& [exp, coef] : resultMap) {
        if (coef == 0) continue;
        TermNode* node = new TermNode(coef, exp);
        if (!result) result = tail = node;
        else tail->next = node, tail = node;
    }

    return result;
}

void drawPolynomial(cv::Mat& img, TermNode* poly, cv::Point origin, const std::string& label, cv::Scalar color) {
    std::string text = label + ": ";
    for (TermNode* p = poly; p != nullptr; p = p->next) {
        text += std::to_string(p->coefficient) + "x^" + std::to_string(p->exponent);
        if (p->next) text += " + ";
    }
    cv::putText(img, text, origin, cv::FONT_HERSHEY_SIMPLEX, 0.6, color, 1);
}

void visualizePolynomialOperation(TermNode* p1, TermNode* p2, TermNode* result, const std::string& title) {
    cv::Mat canvas(600, 1400, CV_8UC3, cv::Scalar(255, 255, 255));
    int y = 80;
    drawPolynomial(canvas, p1, cv::Point(50, y), "P1", cv::Scalar(255, 0, 0));
    y += 100;
    drawPolynomial(canvas, p2, cv::Point(50, y), "P2", cv::Scalar(0, 255, 0));
    y += 100;
    drawPolynomial(canvas, result, cv::Point(50, y), title, cv::Scalar(0, 0, 255));

    cv::imshow(title, canvas);
    cv::waitKey(0);
}

void deletePolynomial(TermNode* head) {
    while (head) {
        TermNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

void visualizePolynomialsTogether(TermNode* p1, TermNode* p2, TermNode* sum, TermNode* product) {
    cv::Mat canvas(600, 1200, CV_8UC3, cv::Scalar(255, 255, 255));
    int y = 80;
    drawPolynomial(canvas, p1, cv::Point(50, y), "P1", cv::Scalar(255, 0, 0));
    y += 100;
    drawPolynomial(canvas, p2, cv::Point(50, y), "P2", cv::Scalar(0, 255, 0));
    y += 100;
    drawPolynomial(canvas, sum, cv::Point(50, y), "P1 + P2", cv::Scalar(0, 0, 255));
    y += 100;
    drawPolynomial(canvas, product, cv::Point(50, y), "P1 * P2", cv::Scalar(0, 0, 0));

    cv::imshow("Polynomial Addition & Multiplication", canvas);
    cv::waitKey(0);
}
