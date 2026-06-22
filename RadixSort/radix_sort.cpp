// *********************************************************
// Program: radix_sort.cpp
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
#include <sstream>
#include <string>
#include <chrono>

struct DataRow {
    long long intKey;
    std::string strKey;
};

//  function to count rows in the CSV file
int countLines(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return 0;
    
    int lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) lines++;
    }
    return lines;
}

void countingSortForRadix(DataRow arr[], int n, long long placement) {
    DataRow* output = new DataRow[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++) {
        int digit = (arr[i].intKey / placement) % 10;
        count[digit]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i].intKey / placement) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    delete[] output;
}

void radixSort(DataRow arr[], int n) {
    long long placement = 1;
    for (int d = 1; d <= 10; d++) {
        countingSortForRadix(arr, n, placement);
        placement *= 10;
    }
}

int main() {
    std::string inputFilename = "dataset_100000000.csv";

    int n = countLines(inputFilename);
    if (n == 0) {
        std::cerr << "Error: File empty or could not be opened.\n";
        return 1;
    }

    // Dynamically allocate array memory to support big input sizes
    DataRow* dataset = new DataRow[n];

    std::ifstream inFile(inputFilename);
    std::string line;
    int idx = 0;

    // Load data from file
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string part1, part2;
        if (std::getline(ss, part1, ',') && std::getline(ss, part2)) {
            dataset[idx].intKey = std::stoll(part1);
            dataset[idx].strKey = part2;
            idx++;
        }
    }
    inFile.close();

    std::cout << "Successfully loaded " << n << " records. Starting Radix Sort...\n";

    auto start = std::chrono::high_resolution_clock::now();
    
    radixSort(dataset, n);
    
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\n=========================================\n";
    std::cout << "Sorting completed!\n";
    std::cout << "Running time: " << elapsed.count() << " seconds\n";
    std::cout << "=========================================\n";
    std::cout << "(Take your command prompt screenshot now!)\n\n";

    // Write sorted output data back to disk
    std::string outputFilename = "radix_sorted_" + inputFilename;
    std::ofstream outFile(outputFilename);
    for (int i = 0; i < n; i++) {
        outFile << dataset[i].intKey << "," << dataset[i].strKey << "\n";
    }
    outFile.close();
    std::cout << "Sorted dataset written to: " << outputFilename << "\n";

    delete[] dataset;
    return 0;
}