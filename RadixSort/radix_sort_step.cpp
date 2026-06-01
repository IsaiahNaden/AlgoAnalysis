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
    outFile << "] original\n";

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
    std::string csvFilename;
    int startRow, endRow;

    std::cout << "Enter target dataset CSV file name: ";
    std::cin >> csvFilename;
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
                                 + std::to_string(endRow) + ".txt";

    radixSortStep(dataset, n, startRow, endRow, stepOutputName);

    delete[] dataset;
    return 0;
}