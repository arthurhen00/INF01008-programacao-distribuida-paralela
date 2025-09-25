/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#include <iostream>
#include <omp.h>
#include "state.h"
#include "sim_anneal.h"

using namespace std;

int main() {
    pair<double, state> res = simAnneal();
    double E_best = res.first;
    state best = res.second;

    cout << "Length of shortest path found: " << E_best << "\n";
    cout << "Order of points in shortest path:\n";
    for(auto x : best.points) {
        cout << x.first << " " << x.second << "\n";
    }

    return 0;
}
