/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#ifndef STATE_PARALLEL_H
#define STATE_PARALLEL_H

#include <vector>
#include <utility>
#include <random>

class state {
public:
    std::vector<std::pair<int,int>> points;
    std::mt19937 mt;

    state();
    state(const std::vector<std::pair<int,int>> &pts);

    state next();
    double euclidean(std::pair<int,int> a, std::pair<int,int> b);
    double E();
};

#endif // STATE_PARALLEL_H
