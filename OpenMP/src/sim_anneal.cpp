#include "sim_anneal.hpp"
#include <cmath>
#include <iostream>

bool P(double E, double E_next, double T, std::mt19937 &rng) {
    double prob = exp(-(E_next-E)/T);
    if (prob > 1) return true;
    else {
        std::bernoulli_distribution d(prob);
        return d(rng);
    }
}

std::pair<double, state> simAnneal(double temperature, double decay_rate, unsigned int seed, state s) {
    state best = s;
    double E = s.E();
    double E_best = E;

    std::mt19937 rng(seed);

    while (temperature > 1) {
        state next = s.next();
        double E_next = next.E();
        if (P(E, E_next, temperature, rng)) {
            s = next;
            E = E_next;
            if (E_next < E_best) {
                best = s;
                E_best = E_next;
            }
        }
        temperature *= decay_rate;
    }
    return {E_best, best};
}
