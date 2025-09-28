/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#include <iostream>
#include <chrono>
#include <fstream>
#include "state.h"
#include "sim_anneal.h"
#include <bits/stdc++.h>
using namespace std;

struct Params {
    double start_temp;
    double decay_rate;
    int seed;
    vector<pair<int,int>> points;
};

Params parse_input(const string &filename) {
    Params p;
    ifstream in(filename);
    if (!in) {
        throw runtime_error("Could not open file: " + filename);
    }

    string line;
    while (getline(in, line)) {
        if (line.rfind("start_temp=", 0) == 0) {
            p.start_temp = stod(line.substr(11));
        } else if (line.rfind("decay_rate=", 0) == 0) {
            p.decay_rate = stod(line.substr(11));
        } else if (line.rfind("seed=", 0) == 0) {
            p.seed = stoi(line.substr(5));
        } else if (line.rfind("points=", 0) == 0) {
            auto pos = line.find("=");
            string pts = line.substr(pos + 1);

            if (pts.front() == '{' && pts.back() == '}') {
                pts = pts.substr(1, pts.size() - 2);
            }

            stringstream ss(pts);
            string token;
            while (getline(ss, token, '}')) {
                if (token.empty()) continue;
                size_t open = token.find('{');
                if (open == string::npos) continue;
                string inside = token.substr(open + 1);
                if (inside.back() == ',') inside.pop_back();
                stringstream pair_ss(inside);
                string x_str, y_str;
                if (getline(pair_ss, x_str, ',') && getline(pair_ss, y_str, ',')) {
                    int x = stoi(x_str);
                    int y = stoi(y_str);
                    p.points.emplace_back(x, y);
                }
            }
        }
    }

    return p;
}

using namespace std;

int main(int argc, char** argv) {
   if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    Params params = parse_input(argv[1]);
 
    auto start = std::chrono::high_resolution_clock::now();

    state s(params.points);
    simAnneal(params.start_temp, params.decay_rate, params.seed, s);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    cout << "Execution time: " << elapsed.count() << " ms\n";
    return 0;
}
