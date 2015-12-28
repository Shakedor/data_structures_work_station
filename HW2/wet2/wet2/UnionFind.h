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
	
	dynamicArray<set> setArr;
	dynamicArray<member> memberArr;
	dynamicArray<int> sizeArr;
	dynamicArray<int> parentArr;

public:
	UnionFind() :n(NULL){};
	UnionFind(int size);
	~UnionFind();
	member& getMember(int i);
	int findNum(int memNum);
	set& getSet(int i);
	set& find(int memNum);
	set& unionize(int mem1, int mem2);
	


};

template<class set, class member>
UnionFind<set, member>::UnionFind(int size){
	if (size <= 0){
		throw sizeOverFlow();
	}
	n = size;
	sizeArr = dynamicArray<int>(n);
	parentArr = dynamicArray<int>(n);
	setArr = dynamicArray<set>(n);
	memberArr = dynamicArray<member>(n);
	for (int i = 0; i < n; i++){
		sizeArr[i] = 1;
		parentArr[i] = 0;
		memberArr[i] = member();
		setArr[i] = set();
	}

}

template<class set, class member>
UnionFind<set, member>::~UnionFind(){

	for (int i = 0; i < n; i++){
		memberArr[i].~member();
		setArr[i].~set();
	}
	
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
	if (parentNum == 0){
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

	int size1 = sizeArr[mem1], size2 = sizeArr[mem2];
	
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