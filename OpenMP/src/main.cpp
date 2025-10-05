/**
 * https://cp-algorithms.com/num_methods/simulated_annealing.html
 * 
 */

#include <iostream>
#include <chrono>
#include <fstream>
#include <omp.h>
#include <bits/stdc++.h>

#include "sim_anneal.hpp"
#include "state.hpp"

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

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <num_threads>\n";
        return 1;
    }

    string input_file = argv[1];
    int num_threads = stoi(argv[2]);
    Params params = parse_input(input_file);

    auto start = chrono::high_resolution_clock::now();

    state s(params.points, params.seed);
    auto result = simAnneal(params.start_temp, params.decay_rate, params.seed, s);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;

    string exe_name = argv[0];
    string output_dir = "results";

    stringstream ss;
    ss << output_dir << "/" 
       << exe_name 
       << "_SEED_" << params.seed 
       << "_INPUTS_" << params.points.size()
       << "_THREADS_" << std::setw(2) << std::setfill('0') << num_threads 
       << ".txt";

    string output_file = ss.str();

    ofstream out(output_file);
    if (!out) {
        cerr << "Could not open output file: " << output_file << "\n";
        return 1;
    }

    out << "Seed=" << params.seed << "\n";
    out << "Input_size=" << params.points.size() << "\n";
    out << "Best_energy=" << result.first << "\n";
    auto last = result.second.points.back();
    auto fst = result.second.points.front();
    out << "First_point=(" << fst.first << ", " << fst.second << ")\n";
    out << "Last_point=(" << last.first << ", " << last.second << ")\n";
    out << "Execution_time_(ms)=" << elapsed.count() << "\n";
    out << "---------------------------\n";

    out.close();

    return 0;
}
