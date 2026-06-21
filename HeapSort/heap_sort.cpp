// *********************************************************
// Program: YOUR_FILENAME.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: 243UC247DM | HARVIND A/L SETHU PATHY | harvind.sethu.pathy@student.mmu.edu.my | +60194546875
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************
// Task Distribution
// Member_1:
// Member_2:
// Member_3:
// Member_4:
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Record {
    unsigned long long id; // holds 10-digit unique integer
    string text;           // holds 5 letter lowercase string
};

vector<Record> readDataset(const string& filename) {
    vector<Record> dataset;
    ifstream file(filename);
    string line;
    if (!file.is_open()) return dataset;
    while (getline(file, line)) {
        if (line.empty()) continue;
        for (char &c : line) { if (c == ',' || c == '/') c = ' '; }
        stringstream ss(line);
        string id_str, text;
        if (ss >> id_str >> text) {
            dataset.push_back({stoull(id_str), text});
        }
    }
    return dataset;
}

// heap_sort function sorts the element named id of unsigned long long data type
void heap_sort(vector<Record>& arr, int n, int i) {
    int largest = i;          // assume current root is the largest
    int left = 2 * i + 1;     // calculate left child index in flat array
    int right = 2 * i + 2;    // calculate right child index in flat array
    // IF left child exists and id is strictly greater than current largest
    if (left < n && arr[left].id > arr[largest].id)
        largest = left;
    // IF right child exists and its id is strictly greater than current largest
    if (right < n && arr[right].id > arr[largest].id)
        largest = right;
    // IF the largest integer located in a child node, swap them
    if (largest != i) {
        Record temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        // recursively sink the demoted parent node down the tree
        heap_sort(arr, n, largest);
    }
}

// perform_heap_sort function will call heap_sort to sort integers by maxheap
void perform_heap_sort(vector<Record>& arr) {
    int n = arr.size();
    // 1 = build a max heap from bottom up
    for (int i = n / 2 - 1; i >= 0; i--)
        heap_sort(arr, n, i);
    // 2 = extract maximum integer at root, place it at the back, shrink heap size
    for (int i = n - 1; i > 0; i--) {
        Record temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        // restore max heap integrity for the remaining unsorted side of the array
        heap_sort(arr, i, 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    string inputFile = argv[1];
    vector<Record> dataset = readDataset(inputFile);
    int n = dataset.size();
    if (n == 0) return 1;
    // start clock
    auto start = high_resolution_clock::now();
    perform_heap_sort(dataset);
    // stop clock
    auto stop = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(stop - start);
    // output sorted CSV as integer,string
    string outputFile = "heap_sorted_" + to_string(n) + "_dataset" + ".csv";
    ofstream outCsv(outputFile);
    for (const auto& rec : dataset) {
        outCsv << rec.id << "," << rec.text << "\n";
    }
    outCsv.close();
    // displays input size and time taken 
    cout << "Heap sort using max heap\n";
    cout << "Input Size: " << n << "\n";
    cout << "Time: " << time_span.count() << " seconds\n";
    return 0;
}