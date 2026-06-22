// *********************************************************
// Program: heap_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 243UC247DQ | Ajjay Naidu a/l Naidu | ajjay.naidu.naidu@student.mmu.edu.my | 012-721 7199
// Member_2: 243UC247DM | Harvind a/l Sethu Pathy | harvind.sethu.pathy@student.mmu.edu.my | 019-454 6875
// Member_3: 243UC2466L | Isaiah Naden a/l Felix Arokianathan | isaiah.naden.felix@student.mmu.edu.my | 010-212 2720
// Member_4: 243UC245LQ | Sanjeevan a/l Rames | SANJEEVAN.RAMES@student.mmu.edu.my | 017- 687 4937
// *********************************************************
// Task Distribution
// Member_1: Q1,Q5,Q6,Q7
// Member_2: Q3,Q5,Q6,Q7
// Member_3: Q2,Q5,Q6,Q7 
// Member_4: Q4,Q5,Q6,Q7
// *********************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// data type use unsigned long long with variable name 'id'
struct Record {
    unsigned long long id;  
    string text;
};

// line parser that safely handles "1000000197, ufnja" and "1000000197/ufnja", whichever is provided in the dataset
vector<Record> readDataset(const string& filename) {
    vector<Record> dataset;
    ifstream file(filename);
    string line;
    if (!file.is_open()) return dataset;
    while (getline(file, line)) {
        if (line.empty()) continue;
        // turn commas and slashes into blank spaces
        for (char &c : line) { if (c == ',' || c == '/') c = ' '; }
        
        stringstream ss(line);
        string id_str, text;
        if (ss >> id_str >> text) {
            dataset.push_back({stoull(id_str), text});
        }
    }
    return dataset;
}

// formats array into correct syntax [1000000197/ufnja, 1000000155/gslag] initial
void logArrayState(const vector<Record>& arr, const string& stepLabel, ofstream& logFile) {
    logFile << "[";
    for (size_t k = 0; k < arr.size(); ++k) {
        logFile << arr[k].id << "/" << arr[k].text;
        if (k < arr.size() - 1) logFile << ", ";
    }
    logFile << "] " << stepLabel << "\n";
}

// max heapify which pushes smaller parent values down the tree
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

void heap_sort_step(vector<Record>& arr, ofstream& logFile) {
    int n = arr.size();
    // step 1 = build max heap by rearranging array so the root is absolute largest
    for (int i = n / 2 - 1; i >= 0; i--) {
        heap_sort(arr, n, i);
    }
    logArrayState(arr, "initial", logFile);
    // step 2 = one by one extract the max value at root to the back of the array
    for (int i = n - 1; i > 0; i--) {
        Record temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heap_sort(arr, i, 0); // restore max heap for remaining unsorted tree
        logArrayState(arr, "i = " + to_string(i), logFile);
    }
}

int main() {
    // input filename
    string inputFile = "dataset_100000000.csv";
    vector<Record> fullData = readDataset(inputFile);
    if (fullData.empty()) {
        cout << "Error: Could not locate or read '" << inputFile << "'\n";
        return 1;
    }
    // row config
    int startRow = 1; // set start row
    int endRow = 7;   // set end row
    if (startRow < 1 || endRow > fullData.size() || startRow > endRow) {
        cout << "Error: Hardcoded rows are out of bounds for this file.\n";
        return 1;
    }
    // grabbing the specific rows requested
    vector<Record> targetSubArray(fullData.begin() + startRow - 1, fullData.begin() + endRow);
    string outName = "dataset_" + to_string(fullData.size()) + "_heap_sort_step_" + 
                     to_string(startRow) + "_" + to_string(endRow) + ".txt";
    
    ofstream logFile(outName);
    if (!logFile.is_open()) return 1;
    heap_sort_step(targetSubArray, logFile);
    logFile.close();
    cout << "Success! Output generated in: " << outName << "\n";
    return 0;
}