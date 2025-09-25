/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#include <iostream>
#include <omp.h>
#include <chrono>
#include "state.h"
#include "sim_anneal.h"

using namespace std;

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    pair<double, state> res = simAnneal();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    
    double E_best = res.first;
    state best = res.second;

    cout << "Execution time: " << elapsed.count() << " ms\n";
    
    cout << "Length of shortest path found: " << E_best << "\n";
    cout << "Order of points in shortest path:\n";
    for(auto x : best.points) {
        cout << x.first << " " << x.second << "\n";
    }

    return 0;
}
