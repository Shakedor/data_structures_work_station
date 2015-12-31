
#include "hashTable.h"
#include "testsMacros.h"
#include <iostream>




////////////////////////////////////////////////////////////////////////////////
///////		Assertions related to assumptions of AVL correctness	////////////
////////////////////////////////////////////////////////////////////////////////

#define ASSERT_DATA_ALREADY_EXISTS(Hash_NAME, key_value, data_value) do {	\
		try {	\
			Hash_NAME.insert(key_value, data_value);	\
		} catch (dataStructures::dataAlreadyExists&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

#define ASSERT_DATA_DOESNT_EXIST(Hash_NAME, key_value) do {	\
		try {	\
			Hash_NAME.remove(key_value);	\
		} catch (dataStructures::dataDoesNotExist&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

#define ASSERT_HASH_IS_EMPTY(Hash_NAME, key_value) do {	\
		try {	\
			Hash_NAME.remove(key_value);	\
		} catch (dataStructures::sturctIsEmpty&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

////////////////////////////////////////////////////////////////////////////////
///////					Insert/Remove Assertions					////////////
////////////////////////////////////////////////////////////////////////////////

#define INSERT(Hash_NAME, key_value, data_value) do {	\
		try {	\
			Hash_NAME.insert(key_value, data_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "+++++++ key,data = (" << key_value << "," << data_value << ") was inserted: +++++++" << std::endl; \
		ASSERT_DATA_ALREADY_EXISTS(Hash_NAME, key_value, data_value); \
} while (0)

#define REMOVE(Hash_NAME, key_value) do {	\
		try {	\
			Hash_NAME.remove(key_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "------- key = " << key_value << " was removed: -------" << std::endl;	\
		ASSERT_DATA_DOESNT_EXIST(Hash_NAME, key_value); \
} while (0)

#define REMOVE_LAST(Hash_NAME, key_value) do {	\
		try {	\
			Hash_NAME.remove(key_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "------- key = " << key_value << " was removed: -------" << std::endl;	\
		ASSERT_HASH_IS_EMPTY(Hash_NAME, key_value); \
} while (0)

////////////////////////////////////////////////////////////////////////////////
///////			function objects needed for hashTable				////////////
////////////////////////////////////////////////////////////////////////////////

template<class T>
class Compare {
public:
	int operator() (const T& a, const T& b) const{
		if (a < b){
			return -1;
		} else if ( b < a){
			return 1;
		} else {
			return 0;
		}
	}
};

class intHasher {
	int modulu;
public:
	intHasher() : modulu(10){}
	intHasher(const intHasher& hasher) : modulu(hasher.modulu){}

	int operator() (int x){
		return x%modulu;
	}
	void setRange(int newRange){
		modulu = newRange;
	}
};

////////////////////////////////////////////////////////////////////////////////
///////				Essential variables for every test				////////////
////////////////////////////////////////////////////////////////////////////////

#define ESSENTIAL_VARIABLES()	\
		Compare<Key> cmp; \
		intHasher tmpHasher; \
		hashTable<Key, Data, Compare<Key>, intHasher> hash(tmpHasher, cmp, 10); \
		std::cout << std::endl; \

////////////////////////////////////////////////////////////////////////////////
///////							Unit tests							////////////
////////////////////////////////////////////////////////////////////////////////

typedef int Key;
typedef int Data;

static bool testHashInsert(){
	bool result = true;

	ESSENTIAL_VARIABLES();

	for (int i=0 ; i<50 ; ++i){
		INSERT(hash, i, 2*i);
	}

	for (int i=100 ; i<1000 ; i+=2){
		INSERT(hash, i, 2*i);
	}

	return result;
}

static bool testHashRemove(){
	bool result = true;

	ESSENTIAL_VARIABLES();

	// Insert and remove all 0-99
	for (int i=0 ; i<100 ; ++i){
		INSERT(hash, i, 2*i);
	}
	for (int i=0 ; i<99 ; ++i){
		REMOVE(hash, i);
	}
	REMOVE_LAST(hash, 99);

	// Insert and remove all odd keys 1-999
	for (int i=1 ; i<1000 ; i+=2){
		INSERT(hash, i, 2*i);
	}
	for (int i=1 ; i<999 ; i+=2){
		REMOVE(hash, i);
	}
	REMOVE_LAST(hash, 999);

	// Insert all 0-999
	for (int i=0 ; i<1000 ; ++i){
		INSERT(hash, i, 2*i);
	}

	// Remove all even
	for (int i=998 ; i>=0 ; i-=2){
		REMOVE(hash, i);
	}

	// Remove all odd
	for (int i=1 ; i<999 ; i+=2){
		REMOVE(hash, i);
	}
	REMOVE_LAST(hash, 999);


	return result;
}

static bool testHashSearch(){
	bool result = true;

	ESSENTIAL_VARIABLES();

	for (int i=0 ; i<1000 ; ++i){
		INSERT(hash, i, 2*i);
	}

	for (int key=0 ; key<1000 ; ++key){
		Data data = hash.search(key);
		ASSERT_TEST(data == 2*key);
	}

	return result;
}

int main() {
	RUN_TEST(testHashInsert);
	RUN_TEST(testHashRemove);
	RUN_TEST(testHashSearch);

	printf("Testing completed successfully.\n");
	return 0;

}
