/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#ifndef SIM_ANNEAL_H
#define SIM_ANNEAL_H

#include "state.h"
#include <random>
#include <chrono>
#include <cmath>
#include <utility>
#include <iostream>

using namespace std;

bool P(double E, double E_next, double T, mt19937 &rng) {
    double prob = exp(-(E_next-E)/T);
    if(prob > 1) return true;
    else {
        bernoulli_distribution d(prob);
        return d(rng);
    }
}

pair<double, state> simAnneal(double temperature = 10000, double decay_rate = 0.995, unsigned int seed = chrono::steady_clock::now().time_since_epoch().count()) {
    state s;
    state best = s;
    double E = s.E();
    double E_best = E;

    mt19937 rng(seed);

    while(temperature > 1) {
        state next = s.next();
        double E_next = next.E();
        if(P(E, E_next, temperature, rng)) {
            s = next;
            E = E_next;
            if(E_next < E_best) {
                best = s;
                E_best = E_next;
            }
        }
        temperature *= decay_rate;
    }
    return {E_best, best};
}

#endif
