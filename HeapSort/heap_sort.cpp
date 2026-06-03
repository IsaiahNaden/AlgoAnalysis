#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

void writeDataset(const string& filename, const vector<Record>& dataset) {
    ofstream file(filename);
    for (const auto& record : dataset) {
        file << record.id << ", " << record.text << "\n";
    }
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

void heapSort(vector<Record>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        Record temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./heap_sort <dataset_n.csv>\n";
        return 1;
    }

    string inputFile = argv[1];

    vector<Record> dataset = readDataset(inputFile);
    if (dataset.empty()) return 1;

    int n = dataset.size();

    auto start = high_resolution_clock::now();

    heapSort(dataset);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    double seconds = duration.count() / 1000000.0;

    string outputFile = "heap_sorted_dataset_" + to_string(n) + ".csv";
    writeDataset(outputFile, dataset);

    cout << "Input Size (n): " << n << "\n";
    cout << "Heap Sort Running Time: " << seconds << " seconds\n";

    ofstream timeFile("heap_sort_time_" + to_string(n) + ".txt");
    timeFile << "Input Size (n): " << n << "\n";
    timeFile << "Heap Sort Running Time: " << seconds << " seconds\n";
    timeFile.close();

    cout << "Sorted data saved to " << outputFile << "\n";

    return 0;
}