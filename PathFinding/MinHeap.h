#ifndef MINHEAP_H
#define MINHEAP_H

#include <string>
#include <vector>

// ���̽ṹ��
struct Process {
    int priority;
    std::string name;

    Process(int p, const std::string& n) : priority(p), name(n) {}

    // Ϊ��С�Ѷ�����ıȽϺ���
    bool operator<(const Process& other) const {
        return priority > other.priority; // priorityԽС���ȼ�Խ��
    }
};

class MinHeap {
private:
    std::vector<Process> heap;

    // ��ӡ�ѵĵ�ǰ״̬
    void print_heap();

    // �³�����
    void sift_down(int index);

    // �ϸ�����
    void sift_up(int index);

public:
    // �������
    void insert(const Process& p);

    // ��ȡ��Сֵ����
    Process extract_min();

    bool is_empty() const;

    int size() const;

    // ���������
    void heap_sort();

    // �����жѻ��Ĳ���
    void insert_without_heapify(const Process& p);

    // һ���Խ���
    void build_heap();

};

#endif // MINHEAP_H


