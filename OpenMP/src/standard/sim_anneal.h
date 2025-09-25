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

pair<double, state> simAnneal() {
    state s;
    state best = s;
    double T = 10000; // Initial temperature
    double u = 0.995; // decay rate
    double E = s.E();
    double E_best = E;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    while(T > 1) {
        state next = s.next();
        double E_next = next.E();
        if(P(E, E_next, T, rng)) {
            s = next;
            E = E_next;
            if(E_next < E_best) {
                best = s;
                E_best = E_next;
            }
        }
        T *= u;
    }
    return {E_best, best};
}

#endif
