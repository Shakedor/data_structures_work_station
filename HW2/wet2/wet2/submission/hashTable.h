#ifndef HASH_TABLE_
#define HASH_TABLE_

#include <cstdlib>
#include <cassert>
#include "dataStructures.h"
#include <cmath>
#include "rankAvl.h"

using namespace dataStructures;

/*
hasher is a function objecct with a () operator, that returns an int value between
0 and m. (While n = O(m))
It has a method called "setRange(newM)", that allows to change the range of
hashing to 0-newM.
*/

template<class Key, class Data, class KeyCompare, class hasherFunc>
class hashTable{
	hasherFunc hasher;
	KeyCompare cmp;
	AvlTree<Key, Data, KeyCompare>** arr;
	int arrSize;

	void resizeIfNeeded();
	void resize(int newArrSize);
	void doInsert(Key key, Data element);

	// Function object that inserts all elements of a single tree in "arr" to "newArr".
	// (They will probably be inserted to different trees)
	class InsertToHash{
		hashTable<Key, Data, KeyCompare, hasherFunc>& newHash;
	public:
		InsertToHash(hashTable<Key, Data, KeyCompare, hasherFunc>& newHash):
			newHash(newHash){}
		void operator()(Key key, Data data){
			newHash.doInsert(key, data);
		}
	};

public:
	int numberOfElements;

	hashTable(const hasherFunc& hasher, const KeyCompare& cmp, int size); //O(size)
	~hashTable(); //O(size)
	
	void insert(Key key, Data element);	// worst: O(size)		average: O(1)
	void remove(Key key);				// worst: O(size)		average: O(1)
	Data& search(const Key& key);		// worst: O(size)		average: O(1)
};

////////////////////////////////////////////////////////////////////////////////
///////					Methods implementation							////////
////////////////////////////////////////////////////////////////////////////////

template<class Key, class Data, class KeyCompare, class hasherFunc>
hashTable<Key, Data, KeyCompare, hasherFunc> :: hashTable(const hasherFunc& hasher,
		const KeyCompare& cmp, int size) :	hasher(hasher), cmp(cmp), arr(NULL),
		arrSize(size), numberOfElements(0){
	arr = new AvlTree<Key, Data, KeyCompare>*[arrSize];
	for (int i=0 ; i<arrSize ; ++i){
		arr[i] = NULL;
	}
}

template<class Key, class Data, class KeyCompare, class hasherFunc>
hashTable<Key, Data, KeyCompare, hasherFunc> :: ~hashTable(){
	for (int i=0 ; i<arrSize ; ++i){
		delete(arr[i]);
	}
	delete[] arr;
}

template<class Key,class Data, class KeyCompare, class hasherFunc>
void hashTable<Key, Data, KeyCompare, hasherFunc> :: doInsert(Key key, Data data){
	int index = hasher(key);
	assert(index >= 0 && index < arrSize);

	if (!arr[index]){
		arr[index] = new AvlTree<Key, Data, KeyCompare>(cmp);
	}
	arr[index]->insert(key, data); // May throw dataStructures::dataAlreadyExists()
}

template<class Key,class Data, class KeyCompare, class hasherFunc>
void hashTable<Key, Data, KeyCompare, hasherFunc> :: insert(Key key, Data data){
	doInsert(key, data);
	++numberOfElements; //Only done if insert succeeds
	resizeIfNeeded();
}

template<class Key,class Data, class KeyCompare, class hasherFunc>
void hashTable<Key, Data, KeyCompare, hasherFunc> :: remove(Key key){
	int index = hasher(key);
	assert(index >= 0 && index < arrSize);

	if (!arr[index]){
		if (numberOfElements == 0){
			throw dataStructures::sturctIsEmpty();
		}
		throw dataStructures::dataDoesNotExist();
	}

	arr[index]->remove(key); // May throw dataDoesNotExist
	--numberOfElements; // Done only if remove succeeded
	if (arr[index]->get_size() == 0){
		delete(arr[index]);
		arr[index] = NULL;
	}
	resizeIfNeeded();
}

template<class Key,class Data, class KeyCompare, class hasherFunc>
Data& hashTable<Key, Data, KeyCompare, hasherFunc> :: search(const Key& key){
	int index = hasher(key);
	assert(index >= 0 && index < arrSize);

	if (!arr[index]){
		if (numberOfElements == 0){
			throw dataStructures::sturctIsEmpty();
		}
		throw dataStructures::dataDoesNotExist();
	}

	return arr[index]->find(key);
}

template<class Key,class Data, class KeyCompare, class hasherFunc>
void hashTable<Key, Data, KeyCompare, hasherFunc> :: resize(int newArrSize){

	// Save old class members
	AvlTree<Key, Data, KeyCompare>** oldArr = arr;
	int oldArrSize = arrSize;

	// Update new class members
	hasher.setRange(newArrSize);
	arr = new AvlTree<Key, Data, KeyCompare>*[newArrSize];
	for (int i=0 ; i<newArrSize ; ++i){
		arr[i] = NULL;
	}
	arrSize = newArrSize;
	// numberOfElements is not changed

	// Create the insertion function object
	InsertToHash insertToHash(*this);

	// Insert all (key,data) of "oldArr" to "newArr"
	for (int i=0 ; i<oldArrSize ; ++i){
		if (oldArr[i]){
			oldArr[i]->inorder(insertToHash);
		}
	}

	// Delete oldArr
	for (int i=0 ; i<oldArrSize ; ++i){
		delete(oldArr[i]);
	}
	delete[] oldArr;
}

template<class Key,class Data, class KeyCompare, class hasherFunc>
void hashTable<Key, Data, KeyCompare, hasherFunc> :: resizeIfNeeded(){
	if (numberOfElements == arrSize){ // arr is full
		resize(2*arrSize);
	} else {
		if (numberOfElements <=  arrSize/4){ // only 1/4 (or less) is taken
			resize( (0.5*arrSize < 1) ? 1 : 0.5*arrSize);
		}
	}
}

#endif // !HASH_TABLE_
