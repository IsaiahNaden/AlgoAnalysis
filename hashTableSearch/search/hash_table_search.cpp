#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
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
	// open csv file
	ifstream inputFile("dataset_1000.csv");
	if (!inputFile.is_open()){
		cerr << "File not found. Please try again.";
		return 0;
	}
	// Get lines from csv file and insert in hash table
	vector<string> arr = getLines(inputFile);
	vector<Data> finalArr = parseData(arr);
	HashTable<Data> ht(finalArr.size());
	inputFile.close();
	for(Data d : finalArr){
		ht.insert(d); 
	}
	//create data not in csv file
	Data targetNotFound; 
	targetNotFound.number = -1;
	targetNotFound.fiveLetter = "eeeee";
	finalArr.push_back(targetNotFound);

	vector<long long> listOfRetrieveTime;
	long double largest;

	for(Data d : finalArr){
		auto startTimer = std::chrono::high_resolution_clock::now();
		bool res = ht.retrieve(d); //hashtable retrieve returns boolean value
		auto endTimer = std::chrono::high_resolution_clock::now() - startTimer;
		long double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTimer).count();
		if (res == true){
			listOfRetrieveTime.push_back(duration); //to compile search time of all values in hashtable
		}
		else {
			largest = duration; //to get search time of item not in hashtable
		}
	}

	long double average = listOfRetrieveTime.at(0);
	long double smallest = listOfRetrieveTime.at(0);
	for (int i = 0; i < listOfRetrieveTime.size();i++){
		if (listOfRetrieveTime.at(i) > average){
			average = listOfRetrieveTime.at(i); //get longest search time
		}
		if (listOfRetrieveTime.at(i) < smallest){
			smallest = listOfRetrieveTime.at(i); //get shortest search time
		}
	}
	// Issue casting <seconds> to <nanoseconds> that makes the value 0.
	// Manual conversion used instead.
	smallest = smallest / 1000000000;
	average = average / 1000000000;
	largest = largest / 1000000000;	

	ofstream outputFile("hash_table_search_dataset.txt");
	cout << "Best case time: " << smallest << " seconds\n";
	cout << "Average case time: " << average << " seconds\n";
	cout << "Worst case time: " << largest << " seconds\n";
	outputFile << "Best case time: " << smallest << " seconds\n";
	outputFile << "Average case time: " << average << " seconds\n";
	outputFile << "Worst case time: " << largest << " seconds\n";

	outputFile.close();
	return 0;
}
