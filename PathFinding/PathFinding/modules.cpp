#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <fstream>
#include <regex>
#include <opencv2/opencv.hpp>
#include "PathFinding.h"
#include "MinHeap.h"
#include "Polynomial.h"
#include "HashTable.h"
#include "Huffman.h"

// 全局路径查找对象
const int rows = 30, cols = 30;
PathFinding pf(rows, cols);
cv::Mat canvas;

void mouseCallback(int event, int x, int y, int flags, void*) {
    int cx = y / 20;
    int cy = x / 20;
    if (event == cv::EVENT_LBUTTONDOWN) {
        if (pf.start.x == -1)
            pf.setStart(cx, cy);
        else if (pf.end.x == -1)
            pf.setEnd(cx, cy);
    }
    else if (event == cv::EVENT_RBUTTONDOWN) {
        pf.resetStartEnd();
    }
}

void run_pathfinding() {
    pf.generateObstacles(0.3f);
    cv::namedWindow("PathFinding");
    cv::setMouseCallback("PathFinding", mouseCallback);

    std::cout << "\n[路径查找模式] 按键说明：\n";
    std::cout << "  d - Dijkstra算法\n";
    std::cout << "  a - A*算法\n";
    std::cout << "  r - 重置障碍\n";
    std::cout << "  q - 退出路径查找\n";

    while (true) {
        pf.drawMap(canvas);
        cv::imshow("PathFinding", canvas);
        char key = (char)cv::waitKey(30);
        if (key == 'q') break;
        else if (key == 'r') pf.resetObstacles();
        else if (key == 'd') {
            if (!pf.dijkstra(canvas))
                std::cout << "未找到路径\n";
        }
        else if (key == 'a') {
            if (!pf.aStar(canvas))
                std::cout << "未找到路径\n";
        }
    }
}

void run_scheduler() {
    MinHeap scheduler;

    std::cout << "\n--- 调度器 ---" << std::endl;
    std::cout << "请输入任务：name priority，每行输入一个任务。" << std::endl;
    std::cout << "输入 down ，开始执行调度。" << std::endl;
    std::cout << "输入 0 ，返回主菜单。" << std::endl;

    std::string line;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, line);

        if (line == "0") return;
        if (line == "down") {
            scheduler.build_heap();
            while (!scheduler.is_empty()) {
                Process p = scheduler.extract_min();
                std::cout << "执行任务：" << p.name << "（优先级：" << p.priority << "）" << std::endl;

                std::cout << "按 Enter 执行下一个任务，输入 add 进入添加模式，输入 0 返回主页面：" << std::endl;
                std::getline(std::cin, line);

                if (line == "add") break;
                if (line == "0") return;
            }
            if (scheduler.is_empty()) {
                std::cout << "任务队列已为空，回到输入模式。" << std::endl;
            }
        }
        else if (!line.empty()) {
            std::stringstream ss(line);
            std::string name;
            int priority;
            ss >> name >> priority;
            if (!name.empty() && ss) {
                scheduler.insert_without_heapify(Process(priority, name));
            }
            else {
                std::cout << "输入格式错误，应为 name priority 或 down/0" << std::endl;
            }
        }
    }
}

void run_polynomial() {
    std::cout << "\n--- 多项式计算 ---" << std::endl;

    std::vector<std::pair<int, int>> p1_terms, p2_terms;
    std::string line;
    std::cout << "输入第一个多项式：系数 指数，每项一行，输入 next 切换到下一个多项式。" << std::endl;
    while (true) {
        std::cout << "P1>> ";
        std::cin >> line;
        if (line == "next") break;
        int coef = std::stoi(line);
        int exp;
        std::cin >> exp;
        p1_terms.emplace_back(coef, exp);
    }

    std::cout << "输入第二个多项式：系数 指数，每项一行，输入 done 结束输入。" << std::endl;
    while (true) {
        std::cout << "P2>> ";
        std::cin >> line;
        if (line == "done") break;
        int coef = std::stoi(line);
        int exp;
        std::cin >> exp;
        p2_terms.emplace_back(coef, exp);
    }

    TermNode* P1 = createPolynomialFromPairs(p1_terms);
    TermNode* P2 = createPolynomialFromPairs(p2_terms);

    TermNode* sum = addPolynomialsLinkedList(P1, P2);
    TermNode* product = multiplyPolynomialsLinkedList(P1, P2);

    visualizePolynomialsTogether(P1, P2, sum, product);

    deletePolynomial(P1);
    deletePolynomial(P2);
    deletePolynomial(sum);
    deletePolynomial(product);
}

class Vocabulary {
private:
    HashTable hashTable;
    int next_id = 0;

    std::string to_lower(const std::string& s) const {
        std::string result = s;
        for (char& c : result) c = std::tolower(c);
        return result;
    }

    std::vector<std::string> tokenize(const std::string& text) const {
        std::vector<std::string> tokens;
        std::regex word_re(R"(\b\w+\b)");
        auto words_begin = std::sregex_iterator(text.begin(), text.end(), word_re);
        auto words_end = std::sregex_iterator();
        for (auto it = words_begin; it != words_end; ++it) {
            tokens.push_back(to_lower(it->str()));
        }
        return tokens;
    }

public:
    void build_from_corpus(const std::string& filepath) {
        std::ifstream infile(filepath);
        if (!infile.is_open()) {
            std::cerr << "无法打开文件: " << filepath << std::endl;
            return;
        }
        std::string line;
        while (getline(infile, line)) {
            for (const auto& word : tokenize(line)) {
                if (hashTable.get(word) == -1) {
                    hashTable.insert(word, next_id++);
                }
            }
        }
        infile.close();
    }

    std::vector<int> encode_sentence(const std::string& sentence) const {
        std::vector<int> ids;
        for (const auto& word : tokenize(sentence)) {
            ids.push_back(hashTable.get(word));
        }
        return ids;
    }

    void save_vocab(const std::string& output) const {
        cv::FileStorage fs(output, cv::FileStorage::WRITE);
        if (!fs.isOpened()) {
            std::cerr << "保存文件失败: " << output << std::endl;
            return;
        }
        fs << "vocab_size" << next_id;
        fs.release();
    }

    void print_vocab() const {
        std::cout << "词表大小: " << next_id << std::endl;
        hashTable.forEach([](const std::string& key, int value) {
            std::cout << key << " -> " << value << std::endl;
            });
    }
};

void run_vocabulary() {
    Vocabulary vocab;
    vocab.build_from_corpus("corpus.txt");

    std::string sentence;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "输入一句话进行编码: ";
    std::getline(std::cin, sentence);

    std::vector<int> encoded = vocab.encode_sentence(sentence);

    std::cout << "编码结果: ";
    for (int id : encoded) std::cout << id << " ";
    std::cout << std::endl;

    vocab.save_vocab("vocab.yml");
    vocab.print_vocab();
}

void run_huffman() {
    std::vector<float> weights = { 1.0, 2.0, 1.0, 3.0, 2.0, 1.0 };

    if (!compressWeights(weights, "model.huff")) {
        std::cerr << "压缩失败。\n";
        return;
    }
    std::cout << "压缩完成！\n";

    std::vector<float> restored;
    if (!decompressWeights("model.huff", restored)) {
        std::cerr << "解压失败。\n";
        return;
    }
    std::cout << "解压成功！还原权重:\n";
    for (float f : restored) std::cout << f << " ";
    std::cout << std::endl;
}
