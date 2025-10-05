/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#ifndef STATE_SERIAL_H
#define STATE_SERIAL_H

#include <vector>
#include <utility>
#include <random>

class state {
public:
    std::vector<std::pair<int,int>> points;
    std::mt19937 mt;

    state();
    state(const std::vector<std::pair<int,int>> &pts);
    state(const std::vector<std::pair<int,int>> &pts, unsigned int seed);

    state next();
    double euclidean(std::pair<int,int> a, std::pair<int,int> b);
    double E();
};

#endif // STATE_SERIAL_H
