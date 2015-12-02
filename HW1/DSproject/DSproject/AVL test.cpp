
#include "AVL.h"

#include "testsMacros.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
///////		Assertions related to assumptions of AVL correctness	////////////
////////////////////////////////////////////////////////////////////////////////

#define ASSERT_DATA_ALREADY_EXISTS(Avl_NAME, key_value, data_value) do {	\
		try {	\
			Avl_NAME.insert(key_value, data_value);	\
		} catch (dataStructures::dataAlreadyExists&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

#define ASSERT_DATA_DOESNT_EXIST(Avl_NAME, key_value) do {	\
		try {	\
			Avl_NAME.remove(key_value);	\
		} catch (dataStructures::dataDoesNotExist&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

#define ASSERT_AVL_IS_EMPTY(Avl_NAME, key_value) do {	\
		try {	\
			Avl_NAME.remove(key_value);	\
		} catch (dataStructures::sturctIsEmpty&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

////////////////////////////////////////////////////////////////////////////////
///////					Insert/Remove Assertions					////////////
////////////////////////////////////////////////////////////////////////////////

#define INSERT(Avl_NAME, key_value, data_value) do {	\
		try {	\
			Avl_NAME.insert(key_value, data_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "+++++++ key,data = (" << key_value << "," << data_value << ")was inserted: +++++++" << std::endl; \
		ASSERT_DATA_ALREADY_EXISTS(Avl_NAME, key_value, data_value); \
} while (0)

#define REMOVE(Avl_NAME, key_value) do {	\
		try {	\
			Avl_NAME.remove(key_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "------- key = " << key_value << "was removed: -------" << std::endl;	\
		ASSERT_DATA_DOESNT_EXIST(Avl_NAME, key_value); \
} while (0)

#define REMOVE_LAST(Avl_NAME, key_value) do {	\
		try {	\
			Avl_NAME.remove(key_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "------- key = " << key_value << "was removed: -------" << std::endl;	\
		ASSERT_AVL_IS_EMPTY(Avl_NAME, key_value); \
} while (0)

/////////////////////////
/// Perform and print ///
/////////////////////////

#define INSERT_AND_PRINT(Avl_NAME, key_value, data_value) do {	\
		INSERT(Avl_NAME, key_value, data_value); \
		Avl_NAME.printAvl();	\
} while (0)

#define REMOVE_AND_PRINT(Avl_NAME, key_value) do {	\
		REMOVE(Avl_NAME, key_value); \
		Avl_NAME.printAvl();	\
} while (0)

#define REMOVE_LAST_AND_PRINT(Avl_NAME, key_value) do {	\
		REMOVE_LAST(Avl_NAME, key_value); \
		Avl_NAME.printAvl();	\
} while (0)

////////////////////////////////////////////////////////////////////////////////
///////				Compare between keys function object			////////////
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

template<class T>
class Even {
public:
	bool operator() (const T& t_key, const T& t_data) const{
		if (t_key %2 == 0){
			return true;
		} else {
			return false;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
///////							Unit tests							////////////
////////////////////////////////////////////////////////////////////////////////

typedef int keyTest;
typedef int DataTest;

static bool testAvlInsert(){
	bool result = true;
	Compare<keyTest> cmp;
	AvlTree<keyTest, DataTest,Compare<keyTest> > avl(cmp);
	std::cout << std::endl;

	for (int i = 10 ; i>=0 ; --i){
		INSERT_AND_PRINT(avl, i, 2*i);
	}

	std::cout << std::endl << std::endl << "	Removing..." << std::endl << std::endl;

	for (int i = 0 ; i<10 ; ++i){
		REMOVE_AND_PRINT(avl, i);
	}
	REMOVE_LAST_AND_PRINT(avl, 10);


	INSERT_AND_PRINT(avl, 5, -5);
	INSERT_AND_PRINT(avl, 6, -6);
	INSERT_AND_PRINT(avl, 10, -10);
	INSERT_AND_PRINT(avl, 8, -8);
	INSERT_AND_PRINT(avl, 7, -7);
	INSERT_AND_PRINT(avl, 1, -1);
	INSERT_AND_PRINT(avl, 2, -2);
	REMOVE_AND_PRINT(avl,7);
	INSERT_AND_PRINT(avl, 9, -9);
	INSERT_AND_PRINT(avl, 3, -3);
	INSERT_AND_PRINT(avl, 4, -4);
	INSERT_AND_PRINT(avl, 7, -7);
	REMOVE_AND_PRINT(avl,10);
	REMOVE_AND_PRINT(avl,9);
	REMOVE_AND_PRINT(avl,8);


	return result;
}


static bool testAvlJoinTrees(){
	bool result = true;
	Compare<keyTest> cmp;
	std::cout << std::endl;

	AvlTree<keyTest, DataTest,Compare<keyTest> > avl_1(cmp);
	for (int i=0 ; i<20 ; i+=2){
		INSERT(avl_1, i, -i);
	}

	AvlTree<keyTest, DataTest,Compare<keyTest> > avl_2(cmp);
	for (int i=1; i<20 ; i+=2){
		INSERT(avl_2, i, -i);
	}

	AvlTree<keyTest, DataTest,Compare<keyTest> > avl_3(avl_1, avl_2);
	avl_3.printAvl();

	AvlTree<keyTest, DataTest,Compare<keyTest> > avl_4(cmp);
	for (int i=20; i<1000 ; i+=3){
		INSERT(avl_4, i, -i);
	}

	AvlTree<keyTest, DataTest,Compare<keyTest> > avl_5(avl_4, avl_3);
	avl_5.printAvl();

	AvlTree<keyTest, DataTest,Compare<keyTest> > avl_6(avl_5);
	avl_6.printAvl();

	avl_6.removeIf(Even<int>());
	avl_6.printAvl();

	return result;
}


int main() {
	RUN_TEST(testAvlInsert);
	RUN_TEST(testAvlJoinTrees);

	printf("Testing completed successfully.");
	return 0;
}
