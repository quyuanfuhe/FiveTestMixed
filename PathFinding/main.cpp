#include <iostream>

// 各模块函数声明
void run_pathfinding();
void run_scheduler();
void run_polynomial();
void run_vocabulary();
void run_huffman();

int main() {
    while (true) {
        std::cout << "\n=== 主菜单 ===" << std::endl;
        std::cout << "1. 启动路径查找" << std::endl;
        std::cout << "2. 启动调度器" << std::endl;
        std::cout << "3. 启动多项式计算" << std::endl;
        std::cout << "4. 启动词表处理" << std::endl;
        std::cout << "5. 启动Huffman压缩" << std::endl;
        std::cout << "0. 退出程序" << std::endl;
        std::cout << "请输入选择：";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            run_pathfinding();
        }
        else if (choice == 2) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清理输入
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
            std::cout << "退出程序。" << std::endl;
            break;
        }
        else {
            std::cout << "无效选择，请重试！" << std::endl;
        }
    }

    return 0;
}
