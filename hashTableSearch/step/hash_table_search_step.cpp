// *********************************************************
// Program: hash_table_search_step.cpp
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
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct Data{
	long long number;
	string fiveLetter;
};

//LinkedList implementation for Hash table
template <typename T>
struct Node {
	T info;
	Node<T>* next;
};

template <typename T>
class LinkedList {
private: 
	Node<T>* start;
public:
	LinkedList() { start = nullptr; }
	~LinkedList() { makeEmpty(); }
	// inserts at the beginning of the linked list
	void insertFront(T& element) {
		Node<T>* newNode = new Node<T>;
		newNode->info = element;
		newNode->next = start;
		start = newNode;
		}
  	// returns true if element is found; returns false if element is not found
	
  	bool find(T& target) {
    	bool found = false;
    	Node<T>* ptr = start;
    	while (ptr != nullptr && !found) {
      		if ((ptr->info.number == target.number) && (ptr->info.fiveLetter == target.fiveLetter)) {
        	found = true;
      	}
      	else
        	ptr = ptr->next;
    	}
    	return found;
  	}

  	bool isEmpty() { return start == nullptr; }
  	void makeEmpty() {
    	while (start != nullptr) {
      		Node<T>* ptr  = start;
      		start = start->next;
      		delete ptr;
		}
  	}

  	friend ostream& operator<< (ostream& os, LinkedList<T>& list) {
    	Node<T>* ptr = list.start;
    	while (ptr != nullptr) {
      		os << ptr->info.number << "/" << ptr->info.fiveLetter << " ";
      		ptr = ptr->next;
    	}
	    return os;
  	}
};

//Hashtable implementation
template <typename T>
class HashTable{
    vector< LinkedList<T> > table;
    int hashfunction (int hashitem){// hash function
        return hashitem % table.size();
    }
    public:

        HashTable (int size){
            table.resize (size); // resize vector to support size elements.
        }
        ~HashTable(){
            for (int i = 0; i < table.size(); i++)
                table[i].makeEmpty();
        }
        int size(){
            return table.size();
        }
        void insert (T newItem){
            int location = hashfunction(newItem.number);
            table[location].insertFront(newItem);
        }
		// In hashtable class
        bool retrieve (T & target){
            int location = hashfunction(target.number);
            if (table[location].find(target) == false){return false;}
            else {return true;}
        }

        friend ostream& operator<< (ostream& os, HashTable<T>& ht){
            for (int i = 0; i < ht.size(); i++)
                os << i << " = " << ht.table[i] << endl;
            return os;
        }
};

vector<string> getLines(ifstream& file){
	vector<string> arr;
	if (!file.is_open()){
		cout << "File not open\n";
		return arr;
	}

	string line;
	while (getline(file,line)){
		if (!line.empty()){
			arr.push_back(line);
		}
	}
	return arr;
}

vector<Data> parseData(vector<string> arr){
	Data d;
	vector<Data> finalArr;
	for (string s : arr){
		d.number = stoll(s.substr(0,s.find(',')));
		d.fiveLetter = s.substr(s.find(',')+1);
		finalArr.push_back(d);
	}
	return finalArr;
}

int main(){
	//get and open dataset csv file
	ifstream dataFile("dataset_10000.csv");
	if (!dataFile.is_open()){
		cerr << "File not found. Please try again.";
		return 0;
	}
	//read lines from dataset file
	vector<string> inputData = getLines(dataFile);
	vector<Data> finalInputData = parseData(inputData);
	HashTable<Data> ht(finalInputData.size());

	for(Data d : finalInputData){
		ht.insert(d);
	}
	dataFile.close();
	// get and open target file (found)
	ifstream retrieveFileFound("anything.csv");
	if (!retrieveFileFound.is_open()){
		cerr << "File not found. Please try again.";
		return 0;	
	}
	//read target data from file
	vector<string> retrievedDataFound = getLines(retrieveFileFound);
	vector<Data> finalRetrievedDataFound = parseData(retrievedDataFound);
	retrieveFileFound.close();
	
	// get and open target file (Not found)
	ifstream retrieveFileNotFound("new.csv");
	if (!retrieveFileNotFound.is_open()){
		cerr << "File not found. Please try again.";
		return 0;	
	}
	//read target data from file
	vector<string> retrievedDataNotFound = getLines(retrieveFileNotFound);
	vector<Data> finalRetrievedDataNotFound = parseData(retrievedDataNotFound);
	retrieveFileNotFound.close();

	for(Data d : finalRetrievedDataFound){
		string outputFileName = "dataset_1000_hash_table_search_step_" + to_string(d.number) + ".txt";
		ofstream outputFile(outputFileName);
		bool found = ht.retrieve(d);
		if (found == true){ //if found
			cout << d.number << " = " << d.number << "/" << d.fiveLetter << endl;
			outputFile << d.number << " = " << d.number << "/" << d.fiveLetter << endl;
		}
		else { //if not found
			cout << "-1 = " << d.number << endl;
			outputFile << "-1 = " << d.number << endl;
		}
		outputFile.close();
	}

	for(Data d : finalRetrievedDataNotFound){
		string outputFileName = "dataset_1000_hash_table_search_step_" + to_string(d.number) + ".txt";
		ofstream outputFile(outputFileName);
		bool found = ht.retrieve(d);
		if (found == true){ //if found
			cout << d.number << " = " << d.number << "/" << d.fiveLetter << endl;
			outputFile << d.number << " = " << d.number << "/" << d.fiveLetter << endl;
		}
		else { //if not found
			cout << "-1 = " << d.number << endl;
			outputFile << "-1 = " << d.number << endl;
		}
		outputFile.close();
	}
	return 0;
}
