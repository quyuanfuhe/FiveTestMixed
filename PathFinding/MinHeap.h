#ifndef MINHEAP_H
#define MINHEAP_H

#include <string>
#include <vector>

// 进程结构体
struct Process {
    int priority;
    std::string name;

    Process(int p, const std::string& n) : priority(p), name(n) {}

    // 为最小堆而定义的比较函数
    bool operator<(const Process& other) const {
        return priority > other.priority; // priority越小优先级越高
    }
};

class MinHeap {
private:
    std::vector<Process> heap;

    // 打印堆的当前状态
    void print_heap();

    // 下沉操作
    void sift_down(int index);

    // 上浮操作
    void sift_up(int index);

public:
    // 插入操作
    void insert(const Process& p);

    // 提取最小值操作
    Process extract_min();

    bool is_empty() const;

    int size() const;

    // 堆排序过程
    void heap_sort();

    // 不进行堆化的插入
    void insert_without_heapify(const Process& p);

    // 一次性建堆
    void build_heap();

};

#endif // MINHEAP_H


