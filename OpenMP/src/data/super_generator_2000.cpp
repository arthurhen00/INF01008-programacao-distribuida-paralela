#include <iostream>
#include <fstream>
#include <unordered_set>
#include <random>
#include <sstream>

using namespace std;

struct PairHash {
    size_t operator()(const pair<int,int>& p) const {
        return ((size_t)p.first << 32) ^ (size_t)p.second;
    }
};

int main() {
    ofstream out("points.txt");
    if (!out) return 1;

    int N = 100000;
    int RANGE = 1000000;
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, RANGE);

    unordered_set<pair<int,int>, PairHash> used;
    used.reserve(N * 2);

    int count = 0;
    while (count < N) {
        int x = dist(rng);
        int y = dist(rng);
        pair<int,int> p = {x,y};

        if (used.insert(p).second) {
            out << x << " " << y;
            if (count != N-1) out << " ";
            count++;
        }
    }

    out.close();
    return 0;
}
