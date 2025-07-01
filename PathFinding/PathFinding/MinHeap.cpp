#include "MinHeap.h"
#include <iostream>
#include <algorithm>

using namespace std;

// ��ӡ�ѵĵ�ǰ״̬
void MinHeap::print_heap() {
    cout << "�ѵĵ�ǰ״̬��";
    for (const auto& p : heap) {
        cout << "(" << p.name << ", " << p.priority << ") ";
    }
    cout << endl;
}

// �³�����
void MinHeap::sift_down(int index) {
    int size = heap.size();
    while (2 * index + 1 < size) {
        int child = 2 * index + 1;
        if (child + 1 < size && heap[child + 1] < heap[child]) {
            child++;
        }
        if (heap[child] < heap[index]) {
            cout << "�³�����: ���� (" << heap[index].name << ", " << heap[index].priority << ") �� ("
                << heap[child].name << ", " << heap[child].priority << ")\n";
            swap(heap[child], heap[index]);
            print_heap();  // ����ѵĵ�ǰ״̬
            index = child;
        }
        else {
            break;
        }
    }
}

// �ϸ�����
void MinHeap::sift_up(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index] < heap[parent]) {
            cout << "�ϸ�����: ���� (" << heap[index].name << ", " << heap[index].priority << ") �� ("
                << heap[parent].name << ", " << heap[parent].priority << ")\n";
            swap(heap[index], heap[parent]);
            print_heap();  // ����ѵĵ�ǰ״̬
            index = parent;
        }
        else {
            break;
        }
    }
}

// �������
void MinHeap::insert(const Process& p) {
    heap.push_back(p);
    cout << "����: (" << p.name << ", " << p.priority << ")\n";
    print_heap();  // ����ѵĵ�ǰ״̬
    sift_up(heap.size() - 1);
}

// ��ȡ��Сֵ����
Process MinHeap::extract_min() {
    if (heap.empty()) {
        throw out_of_range("Heap is empty");
    }
    Process min_proc = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) {
        cout << "��ȡ��Сֵ�����µ�����: ";
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

// ���������
void MinHeap::heap_sort() {
    int size = heap.size();
    // Build the heap
    for (int i = size / 2 - 1; i >= 0; --i) {
        sift_down(i);
    }

    // Heap sort process
    for (int i = size - 1; i > 0; --i) {
        swap(heap[0], heap[i]);
        cout << "�������ڵ� (" << heap[i].name << ", " << heap[i].priority << ") �� ("
            << heap[0].name << ", " << heap[0].priority << ")\n";
        print_heap();
        sift_down(0);
    }
}

void MinHeap::insert_without_heapify(const Process& p) {
    heap.push_back(p);
    std::cout << "���������: (" << p.name << ", " << p.priority << ") \n";
    print_heap();
}

void MinHeap::build_heap() {
    int size = heap.size();
    for (int i = size / 2 - 1; i >= 0; --i) {
        sift_down(i);
    }
    std::cout << "����ɶѻ�\n";
    print_heap();
}
