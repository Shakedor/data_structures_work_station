
#ifndef AVL_H_
#define AVL_H_

#include <cstdlib>
#include <cassert>
#include "dataStructures.h"
#include <iostream>	//TODO: REMOVE
#include <cmath>

template<class T>
const T& max(const T& a, const T& b){
	return (a < b) ? b : a;
}

template<class T>
const T& abs(const T& a){
	return max(a, -a);
}

// Merge A and B to C in descending order.
// Assumes A and B are in descending order
template<class T, class Compare>
void merge(Compare cmp, const T* A, const T* B, T* C, int sizeA, int sizeB){
	int ai = 0, bi = 0, ci = 0;
	while (ai < sizeA && bi < sizeB) {
		if (cmp(A[ai], B[bi]) <= 0) {
			  C[ci++] = B[bi++];
		} else {
			  C[ci++] = A[ai++];
		}
	}
	if (ai < sizeA) {
		for (int p = ai; p < sizeA; ++p) {
			  C[ci++] = A[p];
		}
	} else {
		for (int p = bi; p < sizeB; ++p) {
			  C[ci++] = B[p];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////			AvlNode			////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<class Key, class Data>
class AvlNode {
	enum Side{
		left_side,
		right_side
	};

	AvlNode* rotate_side(Side);
	void set_son(AvlNode* son, Side);

public:
	Key* keyPtr;
	Data* dataPtr;
	AvlNode* left;
	AvlNode* right;
	AvlNode* parent;
	int height;
	int bf;

	AvlNode(const Key& key, const Data& data, AvlNode* parent = NULL) : keyPtr(NULL),
			dataPtr(NULL), left(NULL), right(NULL), parent(parent), height(0), bf(0) {
		keyPtr = new Key(key);
		dataPtr = new Data(data); // TODO: Error handling
	}
	explicit AvlNode() : keyPtr(NULL), dataPtr(NULL), left(NULL), right(NULL), parent(NULL), height(0), bf(0) {}
	~AvlNode(){
		delete(keyPtr);
		delete(dataPtr);
	}

	void updateNodeInfo();
	void set_right(AvlNode*);
	void set_left(AvlNode*);

	AvlNode* rotate_left();
	AvlNode* rotate_right();
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////			AvlTree			////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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
template<class Key, class Data, class Compare>
class AvlTree {
public:
	AvlTree(const Compare& Cmp) : cmp(Cmp), treeSize(0), root(NULL), max_node(NULL){}
	AvlTree(const AvlTree& a, const AvlTree& b); // O(max(a.treeSize, b.treeSize))
	AvlTree(const AvlTree&); // O(treeSize)
	void operator= (const AvlTree&);

	~AvlTree(){
		destroyTree(root);
	}

	//May throw exceptions.
	void insert(const Key& key, const Data& data);
	void remove(const Key& key);
	template<class Condition> void removeIf(Condition); //Removes all objects that condition(key, data)==true
	Data& find(const Key& key);
	Data& get_max();
	int get_size() const;

	// Calls op(data) for every object.
	template<class Operation> void preorder(Operation& op); // op(key,data)
	template<class Operation> void inorder(Operation& op);
	template<class Operation> void postorder(Operation& op);

	void printAvlRecursive(AvlNode<Key, Data>* v, int depth);
	void printAvl();

	typedef struct element{
		Key* keyPtr;
		Data* dataPtr;
	} *Element;

private:
	Compare cmp;
	int treeSize;
	AvlNode<Key, Data>* root;
	AvlNode<Key, Data>* max_node;

	void destroyTree(AvlNode<Key, Data>*);
	void updateMaxNode();

	AvlNode<Key, Data>* doInsert(const Key& key, const Data& data);
	AvlNode<Key, Data>* doRemove(const Key& key);
	void fixPath(AvlNode<Key, Data>* v);

	AvlNode<Key, Data>* do_find(const Key& key);
	AvlNode<Key, Data>** findPtrInParent(AvlNode<Key, Data>*);

	AvlNode<Key, Data>* roll(AvlNode<Key, Data>* v);
	AvlNode<Key, Data>* roll_LL(AvlNode<Key, Data>* v);
	AvlNode<Key, Data>* roll_LR(AvlNode<Key, Data>* v);
	AvlNode<Key, Data>* roll_RR(AvlNode<Key, Data>* v);
	AvlNode<Key, Data>* roll_RL(AvlNode<Key, Data>* v);

	enum traversalOrder{
		preOrder,
		inOrder,
		postOrder
	};

	template<class Operation> void orderRecursionHelper(AvlNode<Key, Data>*, Operation&, traversalOrder);
	template<class Operation> void orderRecursion(AvlNode<Key, Data>*, Operation&, traversalOrder);
	template<class Operation> void orderRecursionHelper(AvlNode<Key, Data>*, Operation&, traversalOrder) const;
	template<class Operation> void orderRecursion(AvlNode<Key, Data>*, Operation, traversalOrder) const;

	void treeToArr(Element) const;
	void arrToTree(Element arr, int size);

	AvlNode<Key, Data>* buildEmptyFullAvl(int size);
	AvlNode<Key, Data>* buildEmptyAvl(int size);
};

////////////////////////////////////////////////////////////////////////////////
//////																	////////
//////						AvlNode implementation						////////
//////																	////////
////////////////////////////////////////////////////////////////////////////////

template<class Key, class Data>
void AvlNode<Key, Data> :: updateNodeInfo(){
	int hLeft = left ? left->height : -1;
	int hRight = right ? right->height : -1;
	height = max(hLeft, hRight) + 1;
	bf = hLeft - hRight;
}

template<class Key, class Data>
void AvlNode<Key, Data> :: set_son(AvlNode<Key, Data>* son, Side side){
	(side == left_side) ? left = son : right = son;
	if (son){
		son->parent = this;
	}
	updateNodeInfo();
}

template<class Key, class Data>
void AvlNode<Key, Data> :: set_right(AvlNode<Key, Data>* v){
	set_son(v, right_side);
}

template<class Key, class Data>
void AvlNode<Key, Data> :: set_left(AvlNode<Key, Data>* v){
	set_son(v, left_side);
}

template<class Key, class Data>
AvlNode<Key, Data>* AvlNode<Key, Data> :: rotate_side(Side side){
	AvlNode<Key, Data>* newRoot = (side == right_side) ? left : right;

	if (parent && parent->left == this){
		parent->set_left(newRoot);
	} else if (parent && parent->right == this){
		parent->set_right(newRoot);
	} else {
		assert(!parent);
		newRoot->parent = NULL;
	}

	if (side == right_side){
		set_left(newRoot->right);
		newRoot->set_right(this);
	} else{
		assert(side == left_side);
		set_right(newRoot->left);
		newRoot->set_left(this);
	}

	return newRoot;
}

template<class Key, class Data>
AvlNode<Key, Data>* AvlNode<Key, Data> :: rotate_right(){
	return rotate_side(right_side);
}

template<class Key, class Data>
AvlNode<Key, Data>* AvlNode<Key, Data> :: rotate_left(){
	return rotate_side(left_side);
}


////////////////////////////////////////////////////////////////////////////////
//////																	////////
//////						AvlTree implementation						////////
//////																	////////
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////
///// AvlTree copy constructor	/////
/////////////////////////////////////

template<class Key, class Data, class Compare>
AvlTree<Key, Data, Compare> ::AvlTree(const AvlTree& source) : 	cmp(source.cmp),
	treeSize(source.treeSize), root(NULL), max_node(NULL){
	this->operator=(source);
}

/////////////////////////////////////////
/////	AvlTree assignment operator	/////
/////////////////////////////////////////

template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: operator= (const AvlTree& other){
	destroyTree(root);
	treeSize = other.treeSize;
	root = buildEmptyAvl(treeSize);

	Element tmp = new struct element[treeSize];
	other.treeToArr(tmp);
	arrToTree(tmp, treeSize);
	delete[](tmp);

	updateMaxNode();
}


/////////////////////
///// treeToArr	/////
/////////////////////

template<class Key, class Data, class Compare>
class treeToArrOperation{
	typedef typename AvlTree<Key, Data, Compare>::Element Element;

	Element arr;
	int current;
	const int size;
public:
	treeToArrOperation(Element arr, int size) : arr(arr), current(0), size(size){}
	void operator()(AvlNode<Key, Data>* v){
		assert(current <= size);
		assert(v && arr);
		arr[current].keyPtr = v->keyPtr;
		arr[current].dataPtr = v->dataPtr;
		++current;
	}

};

template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: treeToArr (Element arr) const{
	orderRecursion(root, treeToArrOperation<Key, Data, Compare>(arr, treeSize),
			inOrder);
}

/////////////////////
///// arrToTree	/////
/////////////////////

template<class Key, class Data, class Compare>
class arrToTreeOperation{
	typedef typename AvlTree<Key, Data, Compare>::Element Element;

	Element arr;
	int current;
	const int size;
public:
	arrToTreeOperation(Element arr, int size) : arr(arr), current(0), size(size){}
	void operator()(AvlNode<Key, Data>* v){
		if (current < size){
			v->keyPtr = new Key(*(arr[current].keyPtr));
			v->dataPtr = new Data(*(arr[current].dataPtr));
			++current;
		}
	}
};

template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: arrToTree(Element arr, int size){
	orderRecursion(root, arrToTreeOperation<Key, Data, Compare>(arr, size),
			inOrder);
}

/////////////////////////////
/////	buildEmptyAvl	/////
/////////////////////////////

template<class Key, class Data, class Compare>
class addNodesToFullAvl{
	int toAdd;
public:
	addNodesToFullAvl(int toAdd) : toAdd(toAdd){}
	void operator()(AvlNode<Key, Data>* v){
		if (!v->left && !v->right){
			if (toAdd > 1){
				v->set_left(new AvlNode<Key, Data>());
				v->set_right(new AvlNode<Key, Data>());
				toAdd -= 2;
			} else if (toAdd == 1){
				v->set_left(new AvlNode<Key, Data>());
				--toAdd;
			}
		}
	}
};

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: buildEmptyFullAvl(int height){
	if (height == -1){
		return NULL;
	}
	AvlNode<Key, Data>* node = new AvlNode<Key, Data>();
	node->set_right(buildEmptyFullAvl(height - 1));
	node->set_left(buildEmptyFullAvl(height - 1));
	return node;
}

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: buildEmptyAvl(int size){
	int internalNodes = size/2; // The number of internal nodes in a complete binary tree of n nodes is floor(n/2)
	int fullAvlHeight = (internalNodes == 0) ? 0 : int(log2(internalNodes));

	AvlNode<Key, Data>* avl = buildEmptyFullAvl(fullAvlHeight);

	int fullAvlSize = int(pow(2, fullAvlHeight+1)) -1;
	int leafNodesToAdd = size - fullAvlSize;
	orderRecursion(avl, addNodesToFullAvl<Key, Data, Compare>(leafNodesToAdd),
			postOrder);
	return avl;
}

//////////////////////////////////////////////////////////
//////	AvlTree constructor from two trees method	//////
//////////////////////////////////////////////////////////

template<class Key, class Data, class Compare>
class elementCompare{
	Compare cmp;
	typedef typename AvlTree<Key, Data, Compare>::element element;

public:
	elementCompare(Compare cmp) : cmp(cmp){}
	int operator()(const element& a, const element& b) const{
			return cmp(*(a.keyPtr),*(b.keyPtr));
	}
};

template<class Key, class Data, class Compare>
AvlTree<Key, Data, Compare> :: AvlTree(const AvlTree& a, const AvlTree& b) :
	cmp(a.cmp), treeSize(a.get_size()), root(buildEmptyAvl(treeSize)), max_node(NULL){
	int sizeA = a.get_size();
	int sizeB = b.get_size();

	Element A = new struct element[sizeA];
	Element B = new struct element[sizeB];
	Element C = new struct element[sizeA + sizeB]; //TODO: error management

	a.treeToArr(A);
	b.treeToArr(B);

	merge<struct element, elementCompare<Key, Data, Compare> >
		(elementCompare<Key, Data, Compare>(cmp), A, B, C, sizeA, sizeB);

	arrToTree(C, treeSize);
	updateMaxNode();

	delete[](C);
	delete[](B);
	delete[](A);
}

//////////////////////////////////////
//////	AvlTree destroy method	//////
//////////////////////////////////////

template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: destroyTree(AvlNode<Key, Data>* v){
	if (!v){
		return;
	}
	destroyTree(v->left);
	destroyTree(v->right);
	delete(v);
}


//////////////////////////////////////
//////	AvlTree find methods	//////
//////////////////////////////////////

//If data is found - returns a pointer to its node.
//If data isn't found - returns a pointer to its 'wanna-be' parent
template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: do_find(const Key& key){
	if (!root){
		throw dataStructures::sturctIsEmpty();
	}
	AvlNode<Key, Data>* current = root;
	AvlNode<Key, Data>* next = NULL; //Pointer to left\right field (which is also a pointer) of current
	bool found = false;

	while (!found) {
		switch (cmp(key, *current->keyPtr)){
		case 0:
			found = true;
			break;
		case 1:
			next = current->left;
			break;
		case -1:
			next = current->right;
			break;
		default:
			assert(0);
		}

		if (!next){
			break; // data is not found. It should be the son of "current".
		}
		current = next;
	}

	return current;	//Address of the new leaf
}

//Finds the pointer to v in the parent of v.
template<class Key, class Data, class Compare>
AvlNode<Key, Data>** AvlTree<Key, Data, Compare> :: findPtrInParent(AvlNode<Key, Data>* v){
	assert(v);
	if (!v->parent){
		return &root;
	}
	AvlNode<Key, Data>* p = v->parent;
	return (p->left == v) ? &(p->left) : &(p->right);
}

template<class Key, class Data, class Compare>
Data& AvlTree<Key, Data, Compare> :: find(const Key& key){
	AvlNode<Key, Data>* v = do_find(key);
	if (cmp(key, *v->keyPtr) == 0){
		return *v->dataPtr;
	}
	throw dataStructures::dataDoesNotExist();
}

template<class Key, class Data, class Compare>
Data& AvlTree<Key, Data, Compare> :: get_max(){
	if (max_node){
		return *max_node->dataPtr;
	}
	assert(treeSize == 0 && !root);
	throw dataStructures::sturctIsEmpty();
}

template<class Key, class Data, class Compare>
int AvlTree<Key, Data, Compare> :: get_size() const{
	assert(treeSize >= 0);
	return treeSize;
}

//////////////////////////////////////////////
//////	AvlTree updateMaxNode method	//////
//////////////////////////////////////////////

// Updates the max node. Operates at O(log(treeSize)).
template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: updateMaxNode(){
	AvlNode<Key, Data>* v = root;
	AvlNode<Key, Data>* max = NULL;

	while(v){
		max = v;
		v = v->left;
	}
	max_node = max;
}


//////////////////////////////////////////////
//////	AvlTree insert/remove methods	//////
//////////////////////////////////////////////

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: doInsert(const Key& key, const Data& data){
	AvlNode<Key, Data>* v = do_find(key);

	switch (cmp(key, *v->keyPtr)){
	case 0:
		throw dataStructures::dataAlreadyExists();
		break;
	case 1:
		v->left = new AvlNode<Key, Data>(key, data, v);
		break;
	case -1:
		v->right = new AvlNode<Key, Data>(key, data, v);
		break;
	default:
		assert(0); //Shouldn't get here
	}

	return v; //Address of parent
}

template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: insert(const Key& key, const Data& data){
	if (!root){
		root = new AvlNode<Key, Data>(key, data);
	} else {
		fixPath (doInsert(key, data));
	}
	++treeSize;
	updateMaxNode();
}

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: doRemove(const Key& key){
	AvlNode<Key, Data>* v = do_find(key);
	if (cmp(*v->keyPtr, key) != 0){
		throw dataStructures::dataDoesNotExist();
	}

	AvlNode<Key, Data>** vPtr = findPtrInParent(v); 	//pointer to v in the parent of v.
	AvlNode<Key, Data>* returnVal = NULL; //This will be updated to deepest changed node

	if (v->left){
		//Finds biggest in the left sub tree.
		AvlNode<Key, Data>* vNew = v->left;
		while(vNew->right) {
			vNew = vNew->right;
		}
		assert(!vNew->left && !vNew->right); // vNew is a leaf
		returnVal = (vNew->parent == v) ? vNew : vNew->parent;

		// Disconnects biggest leaf from its parent, and places instead of v.
		*(findPtrInParent(vNew)) = NULL;
		vNew->set_right(v->right);
		vNew->set_left(v->left);
		vNew->parent = v->parent;
		*vPtr = vNew; //New parent points to vNew
	} else {
		*vPtr = v->right;
		if (v->right){
			v->right->parent = v->parent;
		}
		returnVal = v->parent;
	}
	delete(v);
	return returnVal;
}

template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: remove(const Key& key){
	if (!root){
		throw dataStructures::sturctIsEmpty();
	} else {
		fixPath (doRemove(key));
	}
	--treeSize;
	updateMaxNode();
}

// insert/remove actions may have unwanted side effects on the insert/remove path:
//		- errors (incorrect bf and height)
//		- |bf| = 2. This means the tree is no longer avl tree.
// This method passes on the insert/remove path (upwards) and:
//		- Updates height and bf of every node.
//		- For any node that has |BF| = 2, it fixes it by rolling the node. (See "roll")
template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: fixPath(AvlNode<Key, Data>* v){
	//This algorithm is based on lecture 4, page 25
	while(v){
		v->updateNodeInfo();

		if (abs(v->bf) == 2){
			AvlNode<Key, Data>** vPtr = findPtrInParent(v);
			*vPtr = roll(v);
		}
		v = v->parent; // |bf| <= 1. Continue upwards.
	}
}

/////////////////////////
/////	removeIf	/////
/////////////////////////

template<class Key, class Data, class Compare, class Condition>
class removeIfOperaion{
	typedef typename AvlTree<Key, Data, Compare>::Element Element;

	Element arr;
	int current;
	const int maxSize;
	int* finalSize;
	const Condition& condition;
public:
	removeIfOperaion(Element arr, int maxSize, int* finalSize, const Condition& condition) :
		arr(arr), current(0), maxSize(maxSize), finalSize(finalSize), condition(condition){}
	~removeIfOperaion(){
		*finalSize = current;
	}
	void operator()(AvlNode<Key, Data>* v){
		assert(v && arr);
		assert(current < maxSize);
		if ( ! condition(*(v->keyPtr), *(v->dataPtr)) ){
			arr[current].keyPtr = v->keyPtr;
			arr[current].dataPtr = v->dataPtr;
			++current;
		}
	}

};

template<class Key, class Data, class Compare>
template<class Condition>
void AvlTree<Key, Data, Compare> :: removeIf(Condition condition){ //Removes all elements that condition(key, data)==true
	Element newArr = new struct element[treeSize];
	int newSize;

	//Updates old and newSize
	orderRecursion(root, removeIfOperaion<Key, Data, Compare, Condition>
		(newArr, treeSize, &newSize, condition), inOrder);

	AvlNode<Key, Data>* newRoot = buildEmptyAvl(newSize);
	AvlNode<Key, Data>* oldRoot = root;
	root = newRoot;

	arrToTree(newArr, newSize); // Must come in this order

	destroyTree(oldRoot);
	delete[](newArr);
	updateMaxNode();
}

//////////////////////////////////////
//////	AvlTree roll methods	//////
//////////////////////////////////////

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: roll(AvlNode<Key, Data>* v){
	int bf = v->bf;
	int leftBF = v->left ? v->left->bf : 0;
	int rightBF = v->right ? v->right->bf : 0;

	//This algorithm is based on technion's data structure lectures, lecture 4, page 16.
	if (bf == 2 && leftBF >= 0){
		return roll_LL(v);
	} else if (bf ==2 && leftBF == -1){
		return roll_LR(v);
	} else if (bf == -2 && rightBF == 1){
		return roll_RL(v);
	} else {
		assert(bf == -2 && rightBF <= 0);
		return roll_RR(v);
	}
}

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: roll_LL(AvlNode<Key, Data>* v){
	return v->rotate_right(); //New root of sub tree
}

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: roll_LR(AvlNode<Key, Data>* v){
	v->set_left(v->left->rotate_left());
	return v->rotate_right(); //New root of sub tree
}

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: roll_RR(AvlNode<Key, Data>* v){
	return v->rotate_left(); //New root of sub tree
}

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: roll_RL(AvlNode<Key, Data>* v){
	v->set_right(v->right->rotate_right());
	return v->rotate_left(); //New root of sub tree
}

//////////////////////////////////////////
//////	AvlTree iterating methods	//////
//////////////////////////////////////////

template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: orderRecursion(AvlNode<Key, Data>* v, Operation& op,
		traversalOrder order){
	if (!v){
		return;
	}
	if (order == preOrder) {op(v);}
	orderRecursionHelper(v->left, op, order);
	if (order == inOrder) {op(v);}
	orderRecursionHelper(v->right, op, order);
	if (order == postOrder) {op(v);}
}

template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: orderRecursionHelper(AvlNode<Key, Data>* v, Operation& op,
		traversalOrder order) const{
	if (!v){
		return;
	}
	if (order == preOrder) {op(v);}
	orderRecursionHelper(v->left, op, order);
	if (order == inOrder) {op(v);}
	orderRecursionHelper(v->right, op, order);
	if (order == postOrder) {op(v);} //TODO: eliminate code duplication
}


template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: orderRecursion(AvlNode<Key, Data>* v, Operation op,
		traversalOrder order) const{
	orderRecursionHelper(v, op, order);
}

template<class Key, class Data, class Operation>
class NodeOperation{
	Operation& op;
public:
	NodeOperation(Operation& op) : op(op){}
	void operator()(AvlNode<Key, Data>* v){
		op(*(v->keyPtr), *(v->dataPtr));
	}
};

template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: preorder(Operation& op){
	orderRecursion(root, NodeOperation<Key, Data, Operation>(op), preOrder);
}

template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: inorder(Operation& op){
	orderRecursion(root, NodeOperation<Key, Data, Operation>(op), inOrder);
}

template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: postorder(Operation& op){
	orderRecursion(root, NodeOperation<Key, Data, Operation>(op), postOrder);
}

template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: printAvlRecursive(AvlNode<Key, Data>* v, int depth){
	if (!v){
		return;
	}
	printAvlRecursive(v->right, depth+1);
	for (int i=0 ; i < depth ; ++i){
		std::cout << "    ";
	}
	std::cout << *(v->keyPtr) << "," << *(v->dataPtr) << std::endl;
	printAvlRecursive(v->left,depth+1);
}

template<class Key, class Data, class Compare>
void AvlTree<Key, Data, Compare> :: printAvl(){
	printAvlRecursive(root, 0);
}

#endif /* AVL_H_ */
