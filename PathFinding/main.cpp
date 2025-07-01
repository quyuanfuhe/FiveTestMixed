#include <iostream>

// ��ģ�麯������
void run_pathfinding();
void run_scheduler();
void run_polynomial();
void run_vocabulary();
void run_huffman();

int main() {
    while (true) {
        std::cout << "\n=== ���˵� ===" << std::endl;
        std::cout << "1. ����·������" << std::endl;
        std::cout << "2. ����������" << std::endl;
        std::cout << "3. ��������ʽ����" << std::endl;
        std::cout << "4. �����ʱ���" << std::endl;
        std::cout << "5. ����Huffmanѹ��" << std::endl;
        std::cout << "0. �˳�����" << std::endl;
        std::cout << "������ѡ��";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            run_pathfinding();
        }
        else if (choice == 2) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ��������
            run_scheduler();
        }
        else if (choice == 3) {
            run_polynomial();
        }
        else if (choice == 4) {
            run_vocabulary();
        }
        else if (choice == 5) {
            run_huffman();
        }
        else if (choice == 0) {
            std::cout << "�˳�����" << std::endl;
            break;
        }
        else {
            std::cout << "��Чѡ�������ԣ�" << std::endl;
        }
    }

    return 0;
}
