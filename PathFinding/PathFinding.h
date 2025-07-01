#pragma once
#include <opencv2/opencv.hpp>
#include <queue>
#include <vector>
#include <unordered_map>

struct Node {
    int x, y;
    float cost;
    Node(int x_, int y_, float c_) : x(x_), y(y_), cost(c_) {}
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

class PathFinding {
public:
    PathFinding(int rows, int cols);
    void resetMap();
    void generateObstacles(float obstacleRatio);
    void setStart(int x, int y);
    void setEnd(int x, int y);
    bool dijkstra(cv::Mat& canvas);
    bool aStar(cv::Mat& canvas);
    void reconstructPath();
    void drawMap(cv::Mat& image, bool showSearch = false);
    void resetStartEnd();
    void resetObstacles();

    cv::Point start = { -1, -1 };
    cv::Point end = { -1, -1 };

private:
    int rows, cols;
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<float>> dist;
    std::vector<std::vector<cv::Point>> prev;
    std::vector<std::vector<bool>> visited;
    std::vector<cv::Point> path;
    std::vector<cv::Point> searchNodes;

    bool inBounds(int x, int y);
    std::vector<cv::Point> getNeighbors(int x, int y);
    float heuristic(const cv::Point& a, const cv::Point& b);
};
