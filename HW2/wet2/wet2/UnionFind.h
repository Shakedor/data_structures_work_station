#ifndef UNION_FIND_
#define UNION_FIND_

#include <cstdlib>
#include <cassert>
#include "dataStructures.h"
#include <cmath>

using namespace dataStructures;


/*
a template implementation of a union find disjoint set.

member represents data of members of sets 
set represents data of a sinle set

classes area ssumed to have null argument constructors.

*/


template<class set, class member>
class UnionFind{
	int n;
	
	set* setArr;
	member* memberArr;
	int* sizeArr;
	int* parentArr;

public:
	explicit UnionFind(int size = 0); // TODO : check protected for size == 0
	~UnionFind();
	// get the ith member in the member array
	member& getMember(int i);
	// find the num of the root node of the tree that memNum belongs to
	int findNum(int memNum);
	// get the ith set in the set array.
	set& getSet(int i);
	//get the set that member memNum belongs to
	set& find(int memNum);
	//union the set of mem1 and mem2
	set& unionize(int mem1, int mem2);
	


};

template<class set, class member>
UnionFind<set, member>::UnionFind(int size):n(size),setArr(NULL),memberArr(NULL),sizeArr(NULL),parentArr(NULL){
	if (size <= 0){
		throw sizeOverFlow();
	}


	setArr = new set[n];
	memberArr = new member[n];
	sizeArr = new int[n];
	parentArr = new int[n];

	for (int i = 0; i < n; i++){
		sizeArr[i] = 1;
		parentArr[i] = -1;
		//memberArr[i] = member();
		//setArr[i] = set();
	}

}

template<class set, class member>
UnionFind<set, member>::~UnionFind(){

	for (int i = 0; i < n; i++){
		memberArr[i].~member();
		setArr[i].~set();
	}

	delete[] setArr;
	delete[] memberArr;
	delete[] sizeArr;
	delete[] parentArr;
	
}

template<class set, class member>
member& UnionFind<set, member>::getMember(int i){
	if (i < 0 || i >= n){
		throw sizeOverFlow();
	}
	
	return memberArr[i];
}

template<class set, class member>
set& UnionFind<set, member>::getSet(int i){
	if (i < 0 || i >= n){
		throw sizeOverFlow();
	}

	return setArr[i];
}

template<class set, class member>
int UnionFind<set, member>::findNum(int memNum){
	if (memNum < 0 || memNum >= n){
		throw sizeOverFlow();
	}
	
	int parentNum = parentArr[memNum];
	
	//if root node, return itself
	if (parentNum == -1){
		return memNum;
	}
	
	// if has parent, call itself recursivly, 
	//and assign root to parent slot
	else{
		int updatedParent=findNum(parentNum);
		parentArr[memNum] = updatedParent;

		return updatedParent;
	}


}

template<class set, class member>
set& UnionFind<set, member>::find(int memNum){
	int parent = findNum(memNum);

	return setArr[parent];

}

template<class set, class member>
set& UnionFind<set, member>::unionize(int mem1, int mem2){
	if (mem1 < 0 || mem1 >= n || mem2 < 0 || mem2 >= n){
		throw sizeOverFlow();
	}

	int root1 = findNum(mem1),root2 = findNum(mem2);

	if (root1 == root2){
		return find(root1);
	}

	int size1 = sizeArr[root1], size2 = sizeArr[root2];
	
	if (size1 == size2 || size1>size2){
		sizeArr[root1] = size1 + size2;
		sizeArr[root2] = 0;
		parentArr[root2] = root1;

	}
	else{
		sizeArr[root2] = size1 + size2;
		sizeArr[root1] = 0;
		parentArr[root1] = root2;

	}

	return find(root1);

}


#endif //UNION_FIND_