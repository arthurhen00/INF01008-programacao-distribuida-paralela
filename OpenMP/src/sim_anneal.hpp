/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#ifndef SIM_ANNEAL_H
#define SIM_ANNEAL_H

#include <random>
#include <utility>

#include "state.hpp"

bool P(double E, double E_next, double T, std::mt19937 &rng);

std::pair<double, state> simAnneal(double temperature, double decay_rate, unsigned int seed, state s);

#endif // SIM_ANNEAL_H
