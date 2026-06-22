// *********************************************************
// Program: dataset_generator.cpp
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
#include <unordered_set>
#include <random>
#include <string>
#include <filesystem>

using namespace std;

string generateRandomString(mt19937_64& rng)
{
    string str = "";

    for (int i = 0; i < 5; i++)
    {
        char c = 'a' + (rng() % 26);
        str += c;
    }

    return str;
}

void generateDataset(long long n, mt19937_64& rng)
{
    // Create Datasets folder if it doesn't exist
    filesystem::create_directories("Datasets");

    string filename =
        "Datasets/dataset_" + to_string(n) + ".csv";

    ofstream file(filename);

    if (!file.is_open())
    {
        cout << "Error creating file." << endl;
        return;
    }

    unordered_set<unsigned long long> usedNumbers;

    uniform_int_distribution<unsigned long long>
        dist(1000000000ULL, 9999999999ULL);

    while ((long long)usedNumbers.size() < n)
    {
        unsigned long long number = dist(rng);

        if (usedNumbers.find(number) == usedNumbers.end())
        {
            usedNumbers.insert(number);

            file << number
                 << ","
                 << generateRandomString(rng)
                 << "\n";
        }
    }

    file.close();

    cout << "\nFile saved as: "
         << filename << endl;
}

int main()
{

    cout << "Current directory: "
     << filesystem::current_path()
     << endl;

    long long n;

    cout << "Enter dataset size: ";
    cin >> n;

    // Group Leader ID: 243UC2466L
    // Seed: 2431324662

    mt19937_64 rng(2431324662ULL);

    generateDataset(n, rng);

    return 0;
}