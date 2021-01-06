#ifndef BUCKET_H
#define BUCKET_H


#include <iostream>
using namespace std;

class Bucket{

private:
	// Record the local depth of the bucket
	int local_depth;
	// An array of keys
	int* keys;
	// The size the bucket
	int size;
	// The number of keys that is current in the bucket
	int numOfKeys;

public:
	// Create an empty bucket with the size of 4
	Bucket();
	// Create an empty bucket with the size of the parameter
	Bucket(int size);
	// Create an empty bucket with the size of the parameter and the local depth
	Bucket(int, int);
	// Copy constructor for bucket
	Bucket(const Bucket & bucket);
	// Destructor
	~Bucket();
	// Overloaded assignment operator
	Bucket& operator = (const Bucket& bucket);
	// Insert the key 
	void insert(int key);
	// Search the key
	bool find(int key);
	// Check the bucket is full
	bool isFull();
	// Remove the key
	void print();
	// Get the local depth 
	int getLocalDepth();
	// Remove the key from the bucket
	bool remove(int);
	// Increment local depth of the bucket
	void incrementLocalDepth();
	// Overload the operator []
	int operator [] (int index);
	// Get the number of keys in the bucket
	int getNumOfKeys();
	// Clear the bucket
	void clear();
	// Test runtime error
	bool test_runtime_error(int);
	// Reorganize the bucket
	void reorganize(int);
};

#endif