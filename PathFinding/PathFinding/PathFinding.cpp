#include "PathFinding.h"
#include <random>
#include <cmath>

PathFinding::PathFinding(int r, int c) : rows(r), cols(c) {
    resetMap();
}

void PathFinding::resetMap() {
    dist = std::vector<std::vector<float>>(rows, std::vector<float>(cols, FLT_MAX));
    prev = std::vector<std::vector<cv::Point>>(rows, std::vector<cv::Point>(cols, { -1, -1 }));
    visited = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false));
    path.clear();
    searchNodes.clear();
}

void PathFinding::generateObstacles(float ratio) {
    grid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist01(0.0, 1.0);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (dist01(rng) < ratio)
                grid[i][j] = 1;
}

void PathFinding::setStart(int x, int y) { start = { x, y }; }
void PathFinding::setEnd(int x, int y) { end = { x, y }; }
void PathFinding::resetStartEnd() { start = { -1, -1 }; end = { -1, -1 }; }
void PathFinding::resetObstacles() { generateObstacles(0.2f); }
bool PathFinding::inBounds(int x, int y) { return x >= 0 && x < rows && y >= 0 && y < cols; }

std::vector<cv::Point> PathFinding::getNeighbors(int x, int y) {
    std::vector<cv::Point> neighbors;
    const int dx[4] = { -1, 0, 1, 0 };
    const int dy[4] = { 0, -1, 0, 1 };
    for (int k = 0; k < 4; ++k) {
        int nx = x + dx[k], ny = y + dy[k];
        if (inBounds(nx, ny) && grid[nx][ny] == 0)
            neighbors.emplace_back(nx, ny);
    }
    return neighbors;
}

float PathFinding::heuristic(const cv::Point& a, const cv::Point& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y); // Âü¹þ¶Ù¾àÀë
}

bool PathFinding::dijkstra(cv::Mat& canvas) {
    if (start.x == -1 || end.x == -1) return false;
    resetMap();
    dist[start.x][start.y] = 0;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    pq.emplace(start.x, start.y, 0);

    cv::namedWindow("PathFinding");
    while (!pq.empty()) {
        Node node = pq.top(); pq.pop();
        int x = node.x, y = node.y;
        if (visited[x][y]) continue;
        visited[x][y] = true;
        searchNodes.emplace_back(x, y);

        drawMap(canvas, true);
        cv::imshow("PathFinding", canvas);
        char key = (char)cv::waitKey(20);
        if (key == 27) return false; // °´EscÍË³ö

        if (cv::Point(x, y) == end) {
            reconstructPath();
            drawMap(canvas, false);
            cv::imshow("PathFinding", canvas);
            cv::waitKey(0);
            return true;
        }

        for (auto& n : getNeighbors(x, y)) {
            float alt = dist[x][y] + 1;
            if (alt < dist[n.x][n.y]) {
                dist[n.x][n.y] = alt;
                prev[n.x][n.y] = { x, y };
                pq.emplace(n.x, n.y, alt);
            }
        }
    }
    return false;
}

bool PathFinding::aStar(cv::Mat& canvas) {
    if (start.x == -1 || end.x == -1) return false;
    resetMap();
    dist[start.x][start.y] = 0;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    pq.emplace(start.x, start.y, heuristic(start, end));

    cv::namedWindow("PathFinding");
    while (!pq.empty()) {
        Node node = pq.top(); pq.pop();
        int x = node.x, y = node.y;
        if (visited[x][y]) continue;
        visited[x][y] = true;
        searchNodes.emplace_back(x, y);

        drawMap(canvas, true);
        cv::imshow("PathFinding", canvas);
        char key = (char)cv::waitKey(20);
        if (key == 27) return false; // °´EscÍË³ö

        if (cv::Point(x, y) == end) {
            reconstructPath();
            drawMap(canvas, false);
            cv::imshow("PathFinding", canvas);
            cv::waitKey(0);
            return true;
        }

        for (auto& n : getNeighbors(x, y)) {
            float g = dist[x][y] + 1;
            if (g < dist[n.x][n.y]) {
                dist[n.x][n.y] = g;
                prev[n.x][n.y] = { x, y };
                float f = g + heuristic(n, end);
                pq.emplace(n.x, n.y, f);
            }
        }
    }
    return false;
}

void PathFinding::reconstructPath() {
    path.clear();
    cv::Point p = end;
    while (p != cv::Point(-1, -1)) {
        path.push_back(p);
        p = prev[p.x][p.y];
    }
}

void PathFinding::drawMap(cv::Mat& img, bool showSearch) {
    img = cv::Mat(rows * 20, cols * 20, CV_8UC3, cv::Scalar(255, 255, 255));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (grid[i][j])
                cv::rectangle(img, { j * 20, i * 20 }, { (j + 1) * 20, (i + 1) * 20 }, { 0, 0, 0 }, -1);

    if (showSearch) {
        for (const auto& p : searchNodes)
            cv::rectangle(img, { p.y * 20, p.x * 20 }, { (p.y + 1) * 20, (p.x + 1) * 20 }, { 200, 200, 200 }, -1);
    }

    for (const auto& p : path)
        cv::rectangle(img, { p.y * 20, p.x * 20 }, { (p.y + 1) * 20, (p.x + 1) * 20 }, { 0, 255, 0 }, -1);

    if (start.x != -1)
        cv::rectangle(img, { start.y * 20, start.x * 20 }, { (start.y + 1) * 20, (start.x + 1) * 20 }, { 0, 0, 255 }, -1);
    if (end.x != -1)
        cv::rectangle(img, { end.y * 20, end.x * 20 }, { (end.y + 1) * 20, (end.x + 1) * 20 }, { 255, 0, 0 }, -1);
}
