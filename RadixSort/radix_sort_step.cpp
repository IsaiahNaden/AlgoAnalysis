// *********************************************************
// Program: radix_sort_step.cpp
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

struct DataRow {
    long long intKey;
    std::string strKey;
};

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

void radixSortStep(DataRow arr[], int n, int startRow, int endRow, const std::string& outFilename) {
    std::ofstream outFile(outFilename);
    if (!outFile.is_open()) {
        std::cerr << "Error establishing output step trace file.\n";
        return;
    }

    outFile << "(processing from the rightmost character)\n";
    
    outFile << "[";
    for (int i = startRow; i <= endRow && i < n; i++) {
        outFile << arr[i].intKey << "/" << arr[i].strKey;
        if (i < endRow && i < n - 1) outFile << ", ";
    }
    outFile << "] initial\n";

    long long placement = 1;
    
    for (int d = 1; d <= 10; d++) {
        countingSortForRadix(arr, n, placement);
        placement *= 10;

        outFile << "[";
        for (int i = startRow; i <= endRow && i < n; i++) {
            outFile << arr[i].intKey << "/" << arr[i].strKey;
            if (i < endRow && i < n - 1) outFile << ", ";
        }
        outFile << "] d=" << (11 - d) << "\n";
    }

    outFile.close();
    std::cout << "Step file successfully created: " << outFilename << "\n";
}

int main() {
    int startRow, endRow;

    std::string csvFilename = "dataset_10000.csv";
    std::cout << "Enter tracking start row index: ";
    std::cin >> startRow;
    std::cout << "Enter tracking end row index: ";
    std::cin >> endRow;

    int n = countLines(csvFilename);
    if (n == 0) {
        std::cerr << "Error context: empty source file map.\n";
        return 1;
    }

    DataRow* dataset = new DataRow[n];
    std::ifstream inFile(csvFilename);
    std::string line;
    int idx = 0;

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

    std::string stepOutputName = csvFilename.substr(0, csvFilename.find_last_of('.')) 
                                 + "_radix_sort_step_" 
                                 + std::to_string(startRow) + "_" 
                                 + std::to_string(endRow) + ".csv";

    radixSortStep(dataset, n, startRow, endRow, stepOutputName);

    delete[] dataset;
    return 0;
}