/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#ifndef STATE_H
#define STATE_H

#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <utility>

using namespace std;

class state {
public:
    vector<pair<int,int>> points;
    mt19937 mt{ static_cast<mt19937::result_type>(
        chrono::steady_clock::now().time_since_epoch().count()
    )};

    state() {
        points = {{0,0},{2,2},{0,2},{2,0},{0,1},{1,2},{2,1},{1,0}};
    }

    state next() {
        state s_next;
        s_next.points = points;
        uniform_int_distribution<> choose(0, points.size()-1);
        int a = choose(mt);
        int b = choose(mt);
        swap(s_next.points[a], s_next.points[b]);
        return s_next;
    }

    double euclidean(pair<int,int> a, pair<int,int> b) {
        return hypot(a.first - b.first, a.second - b.second);
    }

    double E() {
        double dist = 0;
        int n = points.size();
        #pragma omp parallel for reduction(+:dist)
        for(int i=0;i<n;i++)
            dist += euclidean(points[i], points[(i+1)%n]);
        return dist;
    }
};

#endif
