#include "MinHeap.h"
#include <iostream>
#include <algorithm>

using namespace std;

// 打印堆的当前状态
void MinHeap::print_heap() {
    cout << "堆的当前状态：";
    for (const auto& p : heap) {
        cout << "(" << p.name << ", " << p.priority << ") ";
    }
    cout << endl;
}

// 下沉操作
void MinHeap::sift_down(int index) {
    int size = heap.size();
    while (2 * index + 1 < size) {
        int child = 2 * index + 1;
        if (child + 1 < size && heap[child + 1] < heap[child]) {
            child++;
        }
        if (heap[child] < heap[index]) {
            cout << "下沉操作: 交换 (" << heap[index].name << ", " << heap[index].priority << ") 和 ("
                << heap[child].name << ", " << heap[child].priority << ")\n";
            swap(heap[child], heap[index]);
            print_heap();  // 输出堆的当前状态
            index = child;
        }
        else {
            break;
        }
    }
}

// 上浮操作
void MinHeap::sift_up(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index] < heap[parent]) {
            cout << "上浮操作: 交换 (" << heap[index].name << ", " << heap[index].priority << ") 和 ("
                << heap[parent].name << ", " << heap[parent].priority << ")\n";
            swap(heap[index], heap[parent]);
            print_heap();  // 输出堆的当前状态
            index = parent;
        }
        else {
            break;
        }
    }
}

// 插入操作
void MinHeap::insert(const Process& p) {
    heap.push_back(p);
    cout << "插入: (" << p.name << ", " << p.priority << ")\n";
    print_heap();  // 输出堆的当前状态
    sift_up(heap.size() - 1);
}

// 提取最小值操作
Process MinHeap::extract_min() {
    if (heap.empty()) {
        throw out_of_range("Heap is empty");
    }
    Process min_proc = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) {
        cout << "提取最小值并重新调整堆: ";
        print_heap();
        sift_down(0);
    }
    return min_proc;
}

bool MinHeap::is_empty() const {
    return heap.empty();
}

int MinHeap::size() const {
    return heap.size();
}

// 堆排序过程
void MinHeap::heap_sort() {
    int size = heap.size();
    // Build the heap
    for (int i = size / 2 - 1; i >= 0; --i) {
        sift_down(i);
    }

    // Heap sort process
    for (int i = size - 1; i > 0; --i) {
        swap(heap[0], heap[i]);
        cout << "交换根节点 (" << heap[i].name << ", " << heap[i].priority << ") 和 ("
            << heap[0].name << ", " << heap[0].priority << ")\n";
        print_heap();
        sift_down(0);
    }
}

void MinHeap::insert_without_heapify(const Process& p) {
    heap.push_back(p);
    std::cout << "已添加任务: (" << p.name << ", " << p.priority << ") \n";
    print_heap();
}

void MinHeap::build_heap() {
    int size = heap.size();
    for (int i = size / 2 - 1; i >= 0; --i) {
        sift_down(i);
    }
    std::cout << "已完成堆化\n";
    print_heap();
}
