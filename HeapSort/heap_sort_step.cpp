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
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left].id > arr[largest].id)
        largest = left;
    if (right < n && arr[right].id > arr[largest].id)
        largest = right;
    if (largest != i) {
        Record temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heap_sort(arr, n, largest); // recursively fix the affected subtree
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

int main(int argc, char* argv[]) {
    // takes argument filename
    if (argc < 2) {
        cout << "Usage: ./heap_sort_step <dataset.csv>\n";
        return 1;
    }
    string inputFile = argv[1];
    vector<Record> fullData = readDataset(inputFile);
    if (fullData.empty()) return 1;
    int startRow = 1;
    int endRow = fullData.size();
    string outName = "dataset_" + to_string(fullData.size()) + "_heap_sorted_step" + ".txt";
    ofstream logFile(outName);
    if (!logFile.is_open()) return 1;
    heap_sort_step(fullData, logFile);
    logFile.close();
    return 0;
}