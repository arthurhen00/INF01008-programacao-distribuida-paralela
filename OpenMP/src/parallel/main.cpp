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

int main(int argc, char** argv) {
    int seed = 921;
    double initial_temperature = 100000;
    double decay_rate = 0.998;
    if (argc > 1 )
    {
        try {
            initial_temperature = stod(argv[1]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number: " << argv[1] << ", expected a double" << std::endl;
            return 1;
        }
    }

    if (argc > 2 )
    {
        try {
            decay_rate = stod(argv[2]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number: " << argv[2] << ", expected a double" << std::endl;
            return 1;
        }
    }

    if (argc > 3 )
    {
        try {
            seed = stoi(argv[3]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number: " << argv[3] << ", expected a integer" << std::endl;
            return 1;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    pair<double, state> res = simAnneal(initial_temperature, decay_rate, seed);
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
