
#ifndef AVL_H_
#define AVL_H_

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
	enum Side {
		left_side,
		right_side
	};

	AvlNode<Data>* rotate_side(Side);
	void set_son(AvlNode<Data>* son, Side);

public:
	Data data;
	AvlNode<Data>* left;
	AvlNode<Data>* right;
	AvlNode<Data>* parent;
	int height;
	int bf;

	explicit AvlNode(const Data& data, AvlNode<Data>* parent = NULL) : data(data),
			left(NULL), right(NULL), parent(parent), height(0), bf(0) {}
	~AvlNode() = default;

	void updateNodeInfo();
	void set_right(AvlNode<Data>*);
	void set_left(AvlNode<Data>*);

	AvlNode<Data>* rotate_left();
	AvlNode<Data>* rotate_right();
};

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

	void destroyTree(AvlNode<Data>*);

	AvlNode<Data>* doInsert(const Data& data);
	AvlNode<Data>* doRemove(const Data& data);
	void fixPath(AvlNode<Data>* v);

	AvlNode<Data>* find(const Data& data);
	AvlNode<Data>** findPtrInParent(AvlNode<Data>*);

	AvlNode<Data>* roll(AvlNode<Data>* v);
	AvlNode<Data>* roll_LL(AvlNode<Data>* v);
	AvlNode<Data>* roll_LR(AvlNode<Data>* v);
	AvlNode<Data>* roll_RR(AvlNode<Data>* v);
	AvlNode<Data>* roll_RL(AvlNode<Data>* v);

	enum Order{
		preOrder,
		inOrder,
		postOrder
	};
	template<class Operation> void orderRecursion(AvlNode<Data>*, Operation, Order);

public:

	AvlTree(const Compare& Cmp) : root(NULL), cmp(Cmp), treeSize(0){}
	~AvlTree(){
		destroyTree(root);
	}

	void insert(const Data& data);
	void remove(const Data& data);

	template<class Operation> void preorder(Operation);
	template<class Operation> void inorder(Operation);
	template<class Operation> void postorder(Operation);

	class avlException{};
	class dataAlreadyExists : public avlException {};
	class dataDoesNotExist : public avlException {};
	class avlIsEmpty : public avlException {};
};

////////////////////////////////////////////////////////////////////////////////
//////																	////////
//////						AvlNode implementation						////////
//////																	////////
////////////////////////////////////////////////////////////////////////////////

template<class Data>
void AvlNode<Data> :: updateNodeInfo(){
	int hLeft = left ? left->height : -1;
	int hRight = right ? right->height : -1;
	height = max(hLeft, hRight) + 1;
	bf = hLeft - hRight;
}

template<class Data>
void AvlNode<Data> :: set_son(AvlNode<Data>* son, Side side){
	(side == Side::left_side) ? left = son : right = son;
	if (son){
		son->parent = this;
	}
	updateNodeInfo();
}

template<class Data>
void AvlNode<Data> :: set_right(AvlNode<Data>* v){
	set_son(v, Side::right_side);
}

template<class Data>
void AvlNode<Data> :: set_left(AvlNode<Data>* v){
	set_son(v, Side::left_side);
}

template<class Data>
AvlNode<Data>* AvlNode<Data> :: rotate_side(Side side){
	AvlNode<Data>* newRoot = (side == Side::right_side) ? left : right;

	if (parent && parent->left == this){
		parent->set_left(newRoot);
	} else if (parent && parent->right == this){
		parent->set_right(newRoot);
	} else {
		assert(!parent);
		newRoot->parent = NULL;
	}

	if (side == Side::right_side){
		set_left(newRoot->right);
		newRoot->set_right(this);
	} else{
		assert(side == Side::left_side);
		set_right(newRoot->left);
		newRoot->set_left(this);
	}

	return newRoot;
}

template<class Data>
AvlNode<Data>* AvlNode<Data> :: rotate_right(){
	return rotate_side(Side::right_side);
}

template<class Data>
AvlNode<Data>* AvlNode<Data> :: rotate_left(){
	return rotate_side(Side::left_side);
}


////////////////////////////////////////////////////////////////////////////////
//////																	////////
//////						AvlTree implementation						////////
//////																	////////
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////
//////	AvlTree destroy method	//////
//////////////////////////////////////

template<class Data,class Compare>
void AvlTree<Data, Compare> :: destroyTree(AvlNode<Data>* v){
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
template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: find(const Data& data){
	assert(root);
	AvlNode<Data>* current = root;
	AvlNode<Data>* next = NULL; //Pointer to left\right field (which is also a pointer) of current
	bool found = false;

	while (!found) {
		switch (cmp(data, current->data)){
		case 0:
			found = true;
			break;
		case -1:
			next = current->left;
			break;
		case 1:
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
template<class Data, class Compare>
AvlNode<Data>** AvlTree<Data, Compare> :: findPtrInParent(AvlNode<Data>* v){
	assert(v);
	if (!v->parent){
		return &root;
	}
	AvlNode<Data>* p = v->parent;
	return (p->left == v) ? &(p->left) : &(p->right);
}

//////////////////////////////////////////////
//////	AvlTree insert/remove methods	//////
//////////////////////////////////////////////

template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: doInsert(const Data& data){
	AvlNode<Data>* v = find(data);

	switch (cmp(v->data, data)){
	case 0:
		throw dataAlreadyExists();
		break;
	case 1:
		v->left = new AvlNode<Data>(data, v);
		break;
	case -1:
		v->right = new AvlNode<Data>(data, v);
		break;
	default:
		assert(0); //Shouldn't get here
	}

	return v; //Address of parent
}

template<class Data,class Compare>
void AvlTree<Data, Compare> :: insert(const Data& data){
	if (!root){
		root = new AvlNode<Data>(data);
	} else {
		fixPath (doInsert(data));
	}
	++treeSize;
}

template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: doRemove(const Data& data){
	AvlNode<Data>* v = find(data);
	if (cmp(v->data, data) != 0){
		throw dataDoesNotExist();
	}

	AvlNode<Data>** vPtr = findPtrInParent(v); 	//pointer to v in the parent of v.
	AvlNode<Data>* returnVal = NULL; //This will be updated to deepest changed node

	if (v->left){
		//Finds biggest in the left sub tree.
		AvlNode<Data>* vNew = v->left;
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

template<class Data,class Compare>
void AvlTree<Data, Compare> :: remove(const Data& data){
	if (!root){
		throw avlIsEmpty();
	} else {
		fixPath (doRemove(data));
	}
	--treeSize;
}

// insert/remove actions may have unwanted side effects on the insert/remove path:
//		- errors (incorrect bf and height)
//		- |bf| = 2. This means the tree is no longer avl tree.
// This method passes on the insert/remove path (upwards) and:
//		- Updates height and bf of every node.
//		- For any node that has |BF| = 2, it fixes it by rolling the node. (See "roll")
template<class Data,class Compare>
void AvlTree<Data, Compare> :: fixPath(AvlNode<Data>* v){
	//This algorithm is based on lecture 4, page 25
	while(v){
		v->updateNodeInfo();

		if (abs(v->bf) == 2){
			AvlNode<Data>** vPtr = findPtrInParent(v);
			*vPtr = roll(v);
		}
		v = v->parent; // |bf| <= 1. Continue upwards.
	}
}

//////////////////////////////////////
//////	AvlTree roll methods	//////
//////////////////////////////////////

template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: roll(AvlNode<Data>* v){
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

template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: roll_LL(AvlNode<Data>* v){
	return v->rotate_right(); //New root of sub tree
}

template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: roll_LR(AvlNode<Data>* v){
	v->set_left(v->left->rotate_left());
	return v->rotate_right(); //New root of sub tree
}

template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: roll_RR(AvlNode<Data>* v){
	return v->rotate_left(); //New root of sub tree
}

template<class Data,class Compare>
AvlNode<Data>* AvlTree<Data, Compare> :: roll_RL(AvlNode<Data>* v){
	v->set_right(v->right->rotate_right());
	return v->rotate_left(); //New root of sub tree
}

//////////////////////////////////////////
//////	AvlTree iterating methods	//////
//////////////////////////////////////////

template<class Data,class Compare>
template<class Operation>
void AvlTree<Data, Compare> :: orderRecursion(AvlNode<Data>* v, Operation op, Order order){
	if (!v){
		return;
	}
	if (order == Order::preOrder) {op(v->data);}
	inorderRecursion(v->right);
	if (order == Order::inOrder) {op(v->data);}
	inorderRecursion(v->left);
	if (order == Order::postOrder) {op(v->data);}
}

template<class Data,class Compare>
template<class Operation>
void AvlTree<Data, Compare> :: preorder(Operation op){
	orderRecursion(root, op, Order::preOrder);
}

template<class Data,class Compare>
template<class Operation>
void AvlTree<Data, Compare> :: inorder(Operation op){
	orderRecursion(root, op, Order::inOrder);
}

template<class Data,class Compare>
template<class Operation>
void AvlTree<Data, Compare> :: postorder(Operation op){
	orderRecursion(root, op, Order::postOrder);
}

#endif /* AVL_H_ */
