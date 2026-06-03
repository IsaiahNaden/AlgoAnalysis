#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Record {
    unsigned long long id;
    string text;
};

vector<Record> readDataset(const string& filename) {
    vector<Record> dataset;
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << "\n";
        return dataset;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id_str, text;
        
        if (getline(ss, id_str, ',') && getline(ss, text)) {
            size_t start = text.find_first_not_of(" \t");
            if (start != string::npos) text = text.substr(start);
            dataset.push_back({stoull(id_str), text});
        }
    }
    return dataset;
}

void printState(const vector<Record>& arr, const string& label, ofstream& logFile) {
    logFile << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        logFile << arr[i].id << "/" << arr[i].text;
        if (i < arr.size() - 1) {
            logFile << ", ";
        }
    }
    logFile << "] " << label << "\n";
}

void heapify(vector<Record>& arr, int n, int i) {
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
        heapify(arr, n, largest);
    }
}

void heapSortSteps(vector<Record>& arr, ofstream& logFile) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    printState(arr, "initial", logFile);

    for (int i = n - 1; i > 0; i--) {
        Record temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);

        printState(arr, "i = " + to_string(i), logFile);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: ./heap_sort_step <dataset_n.csv> <start_row> <end_row>\n";
        return 1;
    }

    string inputFile = argv[1];
    int startRow = stoi(argv[2]);
    int endRow = stoi(argv[3]);

    vector<Record> fullDataset = readDataset(inputFile);
    if (fullDataset.empty()) return 1;

    if (startRow < 1 || endRow > fullDataset.size() || startRow > endRow) {
        cerr << "Error: Invalid start or end row.\n";
        return 1;
    }

    vector<Record> targetArray(fullDataset.begin() + startRow - 1, fullDataset.begin() + endRow);
    int totalElements = fullDataset.size();

    string logFileName = "dataset_" + to_string(totalElements) + "_heap_sort_step_" + 
                         to_string(startRow) + "_" + to_string(endRow) + ".txt";
    
    ofstream logFile(logFileName);
    if (!logFile.is_open()) {
        cerr << "Error creating log file.\n";
        return 1;
    }
    
    heapSortSteps(targetArray, logFile);

    logFile.close();
    cout << "Step logging complete. Check file: " << logFileName << "\n";

    return 0;
}