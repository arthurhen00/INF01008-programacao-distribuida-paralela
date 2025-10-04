#include "state.hpp"
#include <cmath>
#include <algorithm>
#include <chrono>
#include <omp.h>

state::state() : mt(921) {
    points = {{0,0},{2,2},{0,2},{2,0},{0,1},{1,2},{2,1},{1,0}};
}

state::state(const std::vector<std::pair<int,int>> &pts) : points(pts), mt(921) {}

state state::next() {
    state s_next;
    s_next.points = points;
    std::uniform_int_distribution<> choose(0, points.size()-1);
    int a = choose(mt);
    int b = choose(mt);
    std::swap(s_next.points[a], s_next.points[b]);
    return s_next;
}

double state::euclidean(std::pair<int,int> a, std::pair<int,int> b) {
    return std::hypot(a.first - b.first, a.second - b.second);
}

double state::E() {
    double dist = 0;
    int n = points.size();
    #pragma omp parallel for reduction(+:dist)
    for (int i = 0; i < n; i++)
        dist += euclidean(points[i], points[(i+1)%n]);
    return dist;
}
