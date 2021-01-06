#include "Bucket.h"

// Default constructor
// Create an empty bucket with the size of 4
Bucket::Bucket(){
	local_depth = 1;
	size = 4;
	keys = new int[4];
	numOfKeys = 0;
}

// Create an empty bucket with the size of the parameter
Bucket::Bucket(int size){
	local_depth = 1;
	this->size = size;
	keys = new int[size];
	numOfKeys = 0;
}

// Create an empty bucket with the size of the parameter and the local depth
Bucket::Bucket(int size, int local_depth){
	this->local_depth = local_depth;
	this->size = size;
	keys = new int[size];
	numOfKeys = 0;
}

// Copy constructor
Bucket::Bucket(const Bucket & bucket){
	local_depth = bucket.local_depth;
	size = bucket.size;
	keys = new int[size];
	for (int i = 0; i < size; i++){
		keys[i] = bucket.keys[i];
	}
	numOfKeys = bucket.numOfKeys;
}

// Destructor
Bucket::~Bucket(){
	delete []keys;
}

// Overloaded assignment operator
Bucket& Bucket::operator = (const Bucket& bucket){
	local_depth = bucket.local_depth;
	size = bucket.size;
	keys = new int[size];
	for (int i = 0; i < size; i++){
		keys[i] = bucket.keys[i];
	}
	numOfKeys = bucket.numOfKeys;
	return *this;
}

// Insert the key
void Bucket::insert(int key){
	// insert the key into the bucket
	keys[numOfKeys] = key;
	// increment the number of keys in the bucket
	numOfKeys++;
}

// Search the key
bool Bucket::find(int key){
	for (int i = 0; i < size; i++){
		if (keys[i] == key)
			return true;
	}
	return false;
}

// Check the bucket is full
bool Bucket::isFull(){
	return size == numOfKeys;
}

// Print the content of the bucket
void Bucket::print(){
	cout << "[";
	for (int i = 0; i < size; i++){
		// print ',' if it is between two numbers 
		// and it is not the end of the print process
		if (i != 0)
			cout << ",";
		// print the numbers in the bucket
		if (i < numOfKeys){
			cout << std::dec << keys[i];
		}
		else{ // print  '-' if there is no more number in the bucket
			cout << "-";
		}
	}
	cout << "]" << " (" << local_depth << ")";

	
}

// Get the local depth 
int Bucket::getLocalDepth(){
	return local_depth;
}

// Remove the key
bool Bucket::remove(int key){
	bool isFound = false;
	// Check if there is any value in the bucket equals the key to be remove
	for (int i = 0; i < numOfKeys; i++){
		if (key == keys[i]){
			reorganize(i);
			numOfKeys--;
			i--;
			isFound = true;
		}
	}
	return isFound;
}

// Increment local depth of the bucket
void Bucket::incrementLocalDepth(){
	local_depth++;
}

// Overload the operator [] for class Bucket
int Bucket::operator [] (int index){
	return keys[index];
}

// Get the number of keys in the bucket
int Bucket::getNumOfKeys(){
	return numOfKeys;
}

// Clear the bucket
void Bucket::clear(){
	numOfKeys = 0;
}

// Test runtime error
// return true if every value in the bucket is the same as the inserted key
bool Bucket::test_runtime_error(int key){
	for (int i = 0; i < size; i++){
		if (key != keys[i])
			return false;
	}
	return true;
}

// Reorganize the bucket
void Bucket::reorganize(int curr_idx){
	for (int i = curr_idx + 1; i < numOfKeys; i++)
		keys[i-1] = keys[i];
}