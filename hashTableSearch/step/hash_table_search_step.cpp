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
	string dataFileName;
	cout << "Enter data file name: ";
	getline(cin,dataFileName);
	ifstream dataFile(dataFileName);
	if (!dataFile.is_open()){
		cerr << "File not found. Please try again.";
		return 0;
	}

	vector<string> inputData = getLines(dataFile);
	vector<Data> finalInputData = parseData(inputData);
	HashTable<Data> ht(finalInputData.size());

	for(Data d : finalInputData){
		ht.insert(d);
	}
	dataFile.close();

	string retrieveFileName;
	cout << "Enter target file name: ";
	getline(cin,retrieveFileName);
	ifstream retrieveFile(retrieveFileName);
	if (!retrieveFile.is_open()){
		cerr << "File not found. Please try again.";
		return 0;	
	}

	vector<string> retrievedData = getLines(retrieveFile);
	vector<Data> finalRetrievedData = parseData(retrievedData);
	retrieveFile.close();

	for(Data d : finalRetrievedData){
		string outputFileName = "dataset_1000_hash_table_search_step_" + to_string(d.number) + ".txt";
		ofstream outputFile(outputFileName);
		bool found = ht.retrieve(d);
		if (found == true){
			cout << d.number << " = " << d.number << "/" << d.fiveLetter << endl;
			outputFile << d.number << " = " << d.number << "/" << d.fiveLetter << endl;
		}
		else {
			cout << "-1 = " << d.number << endl;
			outputFile << "-1 = " << d.number << endl;
		}
		outputFile.close();
	}
	return 0;
}
