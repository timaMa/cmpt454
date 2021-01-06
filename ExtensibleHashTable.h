#ifndef EXTENSIBLEHASHTABLE_H
#define EXTENSIBLEHASHTABLE_H


#include <cmath>
#include "Bucket.h"


class ExtensibleHashTable{
// 记得写deconstructor
private:
	// Directory
	Bucket** directory;
	// Record the global depth of the hash table
	int global_depth;
	// Bucket size
	int bucket_size;
	// Use the hash function to find the directory index
	int hash_function(int key, int n);
	// Split bucket
	void split_bucket(int, int, int);
	// Double the size of the directory
	void double_directory();
	// Update pointers in the directory
	void update_pointers(int, int);
	// Whether the bucket should be printed
	bool isPrint(int);
	// Test the runtime error exception
	bool test_runtime_error(int, int);
	// Make pointers to NULL
	void setNull(int);

public:
	// Creates an empty hash table
	// the number of keys that can be stored in a bucket size should be set to 4
	// the directory should initially consist of two entries
	// the hash function should use only the last bit of the hash value
	ExtensibleHashTable();
	// the parameter sets the number of keys that can be stored in each bucket
	ExtensibleHashTable(int);
	// Copy constructor
	ExtensibleHashTable(const ExtensibleHashTable& exHashTable);
	// Destructor
	~ExtensibleHashTable();
	// Overloaded assignment operator 
	ExtensibleHashTable& operator = (const ExtensibleHashTable& exHashTable);
	// searches the hash table for the key; if found returns true, otherwise returns false
	bool find(int);
	// inserts the key into the hash table in the appropriate bucket
	void insert(int);
	// searches the hash table for the key;
	// if found, removes all matching values (as there may be duplicate keys) from the hash table; 
	// returns true if the key was found and removed, false otherwise
	bool remove(int);
	// prints the contents of the directory and buckets;
	void print();



};



#endif