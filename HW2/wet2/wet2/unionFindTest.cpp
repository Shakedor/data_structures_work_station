
#include "UnionFind.h"
#include "testsMacros.h"
#include "dataStructures.h"

class intCont{
public:
	int val;
	intCont() :val(0){}
	intCont(int x) :val(x){}
};

bool testInitAndGetMember(){
	bool res=true;



	UnionFind<int, int> Un(10);
	for (int i = 0; i < 10; i++){
		Un.getMember(i) = i;
	}
	for (int i = 0; i < 10; i++){
		TEST_EQUALS(res, Un.getMember(i), i);
	}




	UnionFind<int, intCont> Uni(10);
	for (int i = 0; i < 10; i++){
		Uni.getMember(i) = intCont(10 * i);
	}
	for (int i = 0; i < 10; i++){
		TEST_EQUALS(res, (Uni.getMember(i)).val, 10*i);
	}


	return res;

}

bool testfindNum(){
	bool res=true;
	UnionFind<int, int> Un(10);
	for (int i = 0; i < 10; i++){
		Un.getMember(i) = i;
	}
	for (int i = 0; i < 10; i++){
		TEST_EQUALS(res, Un.findNum(i), i);
	}



	return res;

}



bool testGetSet(){
	bool res = true;
	UnionFind<intCont, intCont> Un(10);
	for (int i = 0; i < 10; i++){
		Un.getMember(i) = intCont(10 * i);
		Un.getSet(i) = intCont(100 * i);
	}

	for (int i = 0; i < 10; i++){
		TEST_EQUALS(res, (Un.getSet(i)).val, 100 * i);
	}
	return res;
}

bool testfind(){
	bool res = true;
	UnionFind<intCont, intCont> Un(10);
	for (int i = 0; i < 10; i++){
		Un.getMember(i) = intCont(10 * i);
		Un.getSet(i) = intCont(100 * i);
	}
	for (int i = 0; i < 10; i++){

		TEST_EQUALS(res, Un.find(i).val, 100 * i);

	}
	return res;

}

bool testUnion(){
	bool res = true;
	UnionFind<intCont, intCont> Un(6);
	for (int i = 0; i < 6; i++){
		Un.getMember(i) = intCont(10 * i);
		Un.getSet(i) = intCont(100 * i);
	}

	try{
		Un.unionize(-1, 1);
	}
	catch (dataStructures::sizeOverFlow&){
		;
	}
	catch (...){
		TEST_EQUALS(res,0,1)
	}

	try{
		Un.unionize(6, 1);
	}
	catch (dataStructures::sizeOverFlow&){
		;
	}
	catch (...){
		TEST_EQUALS(res, 0, 1)
	}
	
	Un.unionize(1, 2);
	TEST_EQUALS(res, Un.find(1).val, 100);
	TEST_EQUALS(res, Un.find(2).val, 100);


	Un.unionize(4, 3);
	TEST_EQUALS(res, Un.find(3).val, 400);
	TEST_EQUALS(res, Un.find(4).val, 400);

	Un.unionize(5, 3);
	TEST_EQUALS(res, Un.find(3).val, 400);
	TEST_EQUALS(res, Un.find(4).val, 400);
	TEST_EQUALS(res, Un.find(5).val, 400);
	
	return res;
}

int main2(){
	
	RUN_TEST(testInitAndGetMember);
	RUN_TEST(testfindNum);
	RUN_TEST(testGetSet);
	RUN_TEST(testfind);
	RUN_TEST(testUnion);

	
	return 0;
}