#ifndef HASH_TABLE_
#define HASH_TABLE_

#include <cstdlib>
#include <cassert>
#include "dataStructures.h"
#include <cmath>
#include "AVL.h"

using namespace dataStructures;

/*
hasher is a function objecct with a randomize method and a () operator where randomize chooses a random 
hashing function out of a uniform set  that is then operable using the () operation
*/

template<class Key,class data,class hasher,class comparer>
class hashTable{
	
	dynamicArray<AvlTree<Key, data, comparer>> hashArray;
public:
	hashTable(int size);
	hashTable();
	~hashTable();
	void insert(Key key, data element);
	void remove(Key key);
	data& search(Key key);


};


#endif // !HASH_TABLE_
