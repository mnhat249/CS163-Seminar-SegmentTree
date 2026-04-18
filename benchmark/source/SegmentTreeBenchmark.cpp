#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <cmath>
#include <cassert>

using namespace std;
using namespace chrono;

// ============ NAIVE SOLUTION ============
class NaiveSolution{
public:
    int numOfUnplacedFruits(vector<int>& fruits, vector<int>& baskets){
        int n = static_cast<int>(fruits.size());
        vector<bool> used(n, false);
        int unplaced = 0;
        
        for (int fruit : fruits){
            bool placed = false;
            for (int i = 0; i<n; i++){
                if (!used[i] and baskets[i] >= fruit){
                    used[i] = true;
                    placed = true;
                    break;
                }
            }
            
            if (!placed){
                unplaced++;
            }
        }
        return unplaced;
    }
};

// ============ SEGMENT TREE SOLUTION ============
class SegmentTreeSolution{
private:
    vector<int> tree;
    
    void build(int node, int left, int right, vector<int>& baskets){
        if (left == right){
            tree[node] = baskets[left];
            return;
        }
        
        int mid = (left + right) / 2;
        build(node * 2, left, mid, baskets);
        build(node * 2 + 1, mid + 1, right, baskets);
        
        tree[node] = max(tree[node * 2], tree[node * 2 + 1]);
    }
    
    void update(int node, int left, int right, int index){
        if (left == right) {
            tree[node] = 0;
            return;
        }
        int mid = (left + right) / 2;
        if (index <= mid){
            update(node * 2, left, mid, index);
        } else{
            update(node * 2 + 1, mid + 1, right, index);
        }
        tree[node] = max(tree[node * 2], tree[node * 2 + 1]);
    }
    
    int query(int node, int left, int right, int target){
        if (tree[node] < target){
            return -1;
        }
        
        if (left == right){return left;}
        
        int mid = (left + right) / 2;
        if (tree[node * 2] >= target){
            return query(node * 2, left, mid, target);
        }
        
        return query(node * 2 + 1, mid + 1, right, target);
    }
    
public:
    int numOfUnplacedFruits(vector<int>& fruits, vector<int>& baskets){
        int n = static_cast<int>(baskets.size());
        tree.assign(n * 4, 0);
        build(1, 0, n - 1, baskets);
        
        int unplaced = 0;
        for (int fruit : fruits){
            int pos = query(1, 0, n - 1, fruit);
            if (pos == -1){
                unplaced++;
            } else{
                update(1, 0, n - 1, pos);
            }
        }
        return unplaced;
    }
};

// ============ UTILITIES ============
vector<int> generateData(int n, int minVal, int maxVal){
    vector<int> data(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(minVal, maxVal);
    
    for (int i = 0; i<n; i++){
        data[i] = dist(gen);
    }
    return data;
}

template <typename T>
double measureTime(T& solution, vector<int>& fruits, vector<int>& baskets){
    auto start = high_resolution_clock::now();
    solution.numOfUnplacedFruits(fruits, baskets);
    auto end = high_resolution_clock::now();
    return duration<double, milli>(end - start).count();
}

size_t estimateMemoryNaive(int n){
    size_t mem = 0;
    mem += sizeof(vector<int>) * 2;
    mem += n * sizeof(int) * 2;
    mem += sizeof(vector<bool>);
    mem += n / 8;
    return mem;
}

size_t estimateMemorySegmentTree(int n){
    size_t mem = 0;
    mem += sizeof(vector<int>) * 2;
    mem += n * sizeof(int);
    mem += 4 * n * sizeof(int);
    return mem;
}

// ============ BENCHMARK EXECUTION ============
void runBenchmark() {
    // Sanity check before running large benchmarks
    cout << "Performing sanity check... ";
    vector<int> testFruits = generateData(100, 1, 200);
    vector<int> testBaskets = generateData(100, 1, 200);
    NaiveSolution testNaive;
    SegmentTreeSolution testSeg;
    assert(testNaive.numOfUnplacedFruits(testFruits, testBaskets) == testSeg.numOfUnplacedFruits(testFruits, testBaskets));
    cout << "PASSED!" << endl;

    vector<int> sizes = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000};
    
    ofstream outFile("benchmark_results.csv");
    outFile << "n,NaiveTime(ms),SegTreeTime(ms),Speedup,NaiveMemory(KB),SegTreeMemory(KB),MemoryRatio(%)\n";
    
    cout << "Running benchmark..." << endl;
    cout << setw(10) << "n" << setw(15) << "Naive(ms)" << setw(15) << "SegTree(ms)"
         << setw(12) << "Speedup" << setw(15) << "NaiveMem(KB)"
         << setw(15) << "SegMem(KB)" << setw(12) << "MemRatio%\n";
    cout << string(94, '-') << "\n";
    
    for (int n : sizes){
        int runs = 3;
        if (n > 100000){runs = 2;}
        if (n > 500000){runs = 1;}
        
        // Skip Naive approach for large N to prevent excessive execution time
        bool skipNaive = (n > 50000);
        
        double naiveTotal = 0, segTreeTotal = 0;
        
        if (skipNaive){
            cout << "  Skipping Naive for n = " << n << " (would take too long)" << endl;
        }
        
        for (int run = 0; run<runs; run++){
            vector<int> fruits = generateData(n, 1, n * 2);
            vector<int> baskets = generateData(n, 1, n * 2);
            
            if (!skipNaive){
                NaiveSolution naive;
                naiveTotal += measureTime(naive, fruits, baskets);
            }
            
            SegmentTreeSolution segTree;
            segTreeTotal += measureTime(segTree, fruits, baskets);
        }
        
        double naiveAvg = skipNaive ? -1 : naiveTotal / runs;
        double segTreeAvg = segTreeTotal / runs;
        double speedup = skipNaive ? -1 : naiveAvg / segTreeAvg;
        
        size_t naiveMem = estimateMemoryNaive(n);
        size_t segTreeMem = estimateMemorySegmentTree(n);
        double memRatio = (double)segTreeMem / naiveMem * 100;
        
        double naiveMemKB = naiveMem / 1024.0;
        double segTreeMemKB = segTreeMem / 1024.0;
        
        cout << setw(10) << n << setw(15) << fixed << setprecision(3);
        
        if (skipNaive) cout << "N/A";
        else cout << naiveAvg;
        
        cout << setw(15) << segTreeAvg << setw(12);
        
        if (skipNaive) cout << "N/A";
        else cout << fixed << setprecision(1) << speedup << "x";
        
        cout << setw(15) << fixed << setprecision(1) << naiveMemKB
             << setw(15) << segTreeMemKB
             << setw(12) << fixed << setprecision(1) << memRatio << "%" << endl;
        
        outFile << n << "," << naiveAvg << "," << segTreeAvg << ","
                << speedup << "," << naiveMemKB << "," << segTreeMemKB << "," << memRatio << endl;
    }
    
    outFile.close();
    cout << endl;
    cout << "Results saved to benchmark_results.csv" << endl;
}

int main() {
    runBenchmark();
    return 0;
}
