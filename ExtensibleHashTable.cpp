#include "ExtensibleHashTable.h"


// Creates an empty hash table
// the number of keys that can be stored in a bucket size should be set to 4
// the directory should initially consist of two entries
// the hash function should use only the last bit of the hash value
ExtensibleHashTable::ExtensibleHashTable(){
	global_depth = 1;
	bucket_size = 4;
	directory = new Bucket* [2];
	for (int i = 0; i < 2; i++){
		directory[i] = new Bucket();
	}
}

// the parameter sets the number of keys that can be stored in each bucket
ExtensibleHashTable::ExtensibleHashTable(int size){
	global_depth = 1;
	bucket_size = size;
	directory = new Bucket* [2];
	for (int i = 0; i < 2; i++){
		directory[i] = new Bucket(size);
	}

}

// Copy constructor
ExtensibleHashTable::ExtensibleHashTable(const ExtensibleHashTable& exHashTable){
	global_depth = exHashTable.global_depth;
	bucket_size = exHashTable.bucket_size;
	int numOfEntries = pow(2, global_depth);
	directory = new Bucket* [numOfEntries];
	for (int i = 0; i < numOfEntries; i++){
		directory[i] = new Bucket(bucket_size);
		*(directory[i]) = *(exHashTable.directory[i]);
		// If the entry should point to the same address as the other entry in the directory
		if (!isPrint(i)){
			int curr_local = directory[i]->getLocalDepth();
			int target_entry = hash_function(i, curr_local);
			directory[i] = directory[target_entry];
		}
	}

}

// Destructor
ExtensibleHashTable::~ExtensibleHashTable(){
	int numOfEntries = pow(2, global_depth);
	for (int i = 0; i < numOfEntries; i++){
		if (directory[i]!=NULL){
			setNull(i);
			delete directory[i];
		}
	}
	delete []directory;
}

// Overloaded assignment operator 
ExtensibleHashTable& ExtensibleHashTable::operator = (const ExtensibleHashTable& exHashTable){
	global_depth = exHashTable.global_depth;
	bucket_size = exHashTable.bucket_size;
	int numOfEntries = pow(2, global_depth);
	directory = new Bucket* [numOfEntries];
	for (int i = 0; i < numOfEntries; i++){
		directory[i] = new Bucket(bucket_size);
		*(directory[i]) = *(exHashTable.directory[i]);
		// If the entry should point to the same address as the other entry in the directory
		if (!isPrint(i)){
			int curr_local = directory[i]->getLocalDepth();
			int target_entry = hash_function(i, curr_local);
			directory[i] = directory[target_entry];
		}
	}
	return *this;
}

// searches the hash table for the key; 
// if found returns true, otherwise returns false
bool ExtensibleHashTable::find(int key){
	// Find the directory index using hash function
	int index = hash_function(key, global_depth);
	return directory[index]->find(key);
}

// inserts the key into the hash table in the appropriate bucket
// Testing runtime_error exception when the bucket splits
void ExtensibleHashTable::insert(int key){
	// Find the directory index using hash function
	int index = hash_function(key, global_depth);
	// If the bucket has room
	if (!directory[index]->isFull()){
		// Insert the key into the bucket
		directory[index]->insert(key);
	}
	else{ // If the bucket is full
		// Get the old local depth of the bucket
		int old_LD = directory[index]->getLocalDepth();
		// New local depth
		int new_LD = old_LD + 1;
		// Compare the local depth of the bucket to the global depth
		// If the global depth and local depth are the same	
		if(global_depth == old_LD){
			// Double the directory size
			double_directory();
			index = hash_function(key, global_depth);
		}
		// Throw a runtime error when it is impossible to insert a value
		if (test_runtime_error(index, key))
			throw std::runtime_error("runtime error");
		// If the global depth is greater
		// Get the bucket that needs to be split
		int originalIndex = hash_function(key, old_LD);
		// Get the pointer in the directory that needs a new bucket
		int pairIndex = originalIndex + pow(2, old_LD);
		// Create a new bucket with the bucket_size and the new local_depth
		// And assign the pointer to the new bucket based on the hash value
		directory[pairIndex] = new Bucket(bucket_size, new_LD);
		// Increment the local depth of the original bucket
		directory[originalIndex]->incrementLocalDepth();
		// Update pointers in the directory
		update_pointers(pairIndex, new_LD);
		// Distribute the values in the original bucket between the original and the new bucket
		split_bucket(originalIndex, pairIndex, new_LD);
		insert(key);
	}
}

// searches the hash table for the key;
// if found, removes all matching values (as there may be duplicate keys) from the hash table; 
// returns true if the key was found and removed, false otherwise
bool ExtensibleHashTable::remove(int key){
	// Use the hash function to find the directory index
	int index = hash_function(key, global_depth);
	// Follow the pointer in the directory to a bucket
	// Remove any incidences of the key from the bucket
	bool isRemove = directory[index]->remove(key);
	return isRemove;
}

// prints the contents of the directory and buckets;
void ExtensibleHashTable::print(){
	int numOfEntries = pow(2, global_depth);
	for(int i = 0; i < numOfEntries; i++){
		cout << std::dec << i << ": " << std::hex << static_cast<void*>(directory[i]) << " --> ";
		// if the bucket has been printed
		if (isPrint(i))
			directory[i]->print();
		cout << endl;
	}	
}

// Use the hash function to find the directory index
int ExtensibleHashTable::hash_function(int key, int n){
	// The last n bits
	// using powers of 2 and the modulo operation.
	int hash = pow(2, n);
	return key % hash;
}

// Split the bucket
// gd for global depth, ld for local depth
void ExtensibleHashTable::split_bucket(int originalBucket, int newBucket, int local_depth){
	// Copy the original bucket
	Bucket temp_bucket = *directory[originalBucket];
	// Clear the original bucket
	directory[originalBucket]->clear();
	// For every value in the original bucket calculate a new index using hash function
	// Insert the value into the corresponding bucket
	// The number of keys should be the size of the bucket
	int numOfKeys = temp_bucket.getNumOfKeys();
	for (int i = 0; i < numOfKeys; i++){
		int key = temp_bucket[i];
		insert(key);
	}
}

// Double the size of the directory
void ExtensibleHashTable::double_directory(){
	// Increment the global depth
	global_depth++;
	// The directory size should the 2 to the power of global depth
	int directory_size = pow(2, global_depth);
	// Copy the old directory
	Bucket** old_directory = directory;
	// Double the size of the old directory
	directory = new Bucket*[directory_size];
	// The size of old directory
	int old_size = directory_size / 2;
	// For the first half of the new directory
	// set the pointers to the buckets in the old directory
	for (int i = 0; i < old_size; i++){
		directory[i] = old_directory[i];
	}
	// For the second half of the new directory
	// set the pointers to the existing directory
	for (int i = old_size; i < directory_size; i++){
		directory[i] = old_directory[i - old_size];
	}
	delete []old_directory;
}


// Update pointers in the directory
void ExtensibleHashTable::update_pointers(int index, int n){
	// Get the number of entries in the directory
	int numOfEntries = pow(2, global_depth);
	// Find the indices in the directory that have the same value with the index
	// based on the rightmost n bits
	for (int i = 0; i < numOfEntries; i++){
		if (hash_function(i, n) == index)
			directory[i] = directory[index];
	}
}

// Whether the bucket should be printed
bool ExtensibleHashTable::isPrint(int index){
	// Get the local depth of the current bucket
	int local_depth = directory[index]->getLocalDepth();
	// If the local depth does not equal the global depth
	if (local_depth < global_depth){
		int hash_value = hash_function(index, local_depth);
		if (index != hash_value)
			return false;
	}
	return true;
}

// Test the runtime error exception
// This can occur where a bucket is filled with identical search key values 
// and an attempt is made to insert another such value. 
bool ExtensibleHashTable::test_runtime_error(int index, int key){
	return directory[index]->test_runtime_error(key);
}

// Make pointers to NULL
void ExtensibleHashTable::setNull(int index){
	int local_depth = directory[index]->getLocalDepth();
	int temp = hash_function(index, local_depth);
	int numOfEntries = pow(2, global_depth);
	for (int i = 0; i < numOfEntries; i++){
		if (hash_function(i, local_depth) == temp){
			// cout << i << endl;
			directory[i] = NULL;
		}
	}
}