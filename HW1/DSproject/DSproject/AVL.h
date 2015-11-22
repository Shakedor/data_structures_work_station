
#ifndef AVL_HPP_
#define AVL_HPP_

#include <cstdlib>
#include <cassert>

template<class T>
const T& max(const T& a, const T& b){
	return (a < b) ? b : a;
}

template<class T>
const T& abs(const T& a){
	return max(a, -a);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////			AvlNode			////////////////////////////

template<class Data>
class AvlNode {
public:
	Data data;
	AvlNode<Data>* left;
	AvlNode<Data>* right;
	AvlNode<Data>* parent;
	int height;
	int bf;

	explicit AvlNode(const Data& data, AvlNode<Data>* parent = NULL) : data(data),
			left(NULL), right(NULL), parent(parent), height(-1), bf(0) {}
	~AvlNode() = default;

	void updateNodeHeight();

};

template<class Data>
void AvlNode<Data>::updateNodeHeight(){
	int hLeft = left ? left->height : -1;
	int hRight = right ? right->height : -1;
	height = max(hLeft, hRight);
	bf = hLeft - hRight;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////			AvlTree			////////////////////////////

/*
 * This is a template implementation of AVL binary search tree.
 * 	<Data> is the type of objects stored in the AVL.
 * 	<Compare> is a function object that compares two <Data> objects. Compare(a, b) returns:
 * 		0	-	If a == b
 * 		1	-	If a > b
 * 		-1	-	If a < b
 *
 * 	<Data> and <Compare> must have copy constructors.
 */
template<class Data,class Compare>
class AvlTree {

	AvlNode<Data>* root;
	Compare cmp;
	int treeSize;

	void destroyTree();
	void fixBF(AvlNode<Data>* v);
	AvlNode<Data>* doInsert(const Data& data);

public:

	AvlTree(const Compare& Cmp) : root(NULL), cmp(Cmp), treeSize(0){}
	~AvlTree(){
		//destroyTree();
	}

	void insert(const Data& data);
	void remove(const Data& data);

	class avlException{};
	class dataAlreadyExists : public avlException {};
	class dataDoesNotExist : public avlException {};
};

/////////////////////////////////////////////////////////////////////////////////////
///////////////////		AvlTree member function implementation	/////////////////////

template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: doInsert(const Data& data){
	assert(root);
	AvlNode<Data>* current = root;
	AvlNode<Data>** next = NULL; //Pointer to left\right field (which is also a pointer) of current
	bool searching = true;

	while (searching) {
		switch (cmp(data, current->data)){
			case -1:
				next = &(current->right);
				break;
			case 1:
				next = &(current->left);
				break;
			default:
				throw dataAlreadyExists();
		}

		if (!(*next)){
			*next = new AvlNode<Data>(data, current);
			searching = false;
		} else {
			current = *next;
		}
	}

	return *next;	//Address of the new leaf
}



template<class Data,class Compare>
void AvlTree<Data, Compare> :: insert(const Data& data){
	if (!root){
		root = new AvlNode<Data>(data);
	} else {
		fixBF (doInsert(data));
	}
	++treeSize;
}

template<class Data,class Compare>
void AvlTree<Data, Compare> :: fixBF(AvlNode<Data>* v){
	while(v != root){
		//int oldHeight = v->height;
		v->updateNodeHeight();

		if (abs(v->bf) == 2){

		}
	}
}


#endif /* AVL_HPP_ */
