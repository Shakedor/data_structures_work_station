
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

template<class Key, class Data>
class AvlNode {
	enum Side {
		left_side,
		right_side
	};

	AvlNode* rotate_side(Side);
	void set_son(AvlNode* son, Side);

public:
	Key key;
	Data data;
	AvlNode* left;
	AvlNode* right;
	AvlNode* parent;
	int height;
	int bf;

	AvlNode(const Key& key, const Data& data, AvlNode* parent = NULL) : key(key),
			data(data), left(NULL), right(NULL), parent(parent), height(0), bf(0) {}
	~AvlNode() = default;

	void updateNodeInfo();
	void set_right(AvlNode*);
	void set_left(AvlNode*);

	AvlNode* rotate_left();
	AvlNode* rotate_right();
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
template<class Key, class Data, class Compare>
class AvlTree {

	AvlNode<Key, Data>* root;
	AvlNode<Key, Data>* max_node;
	Compare cmp;
	int treeSize;

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

	enum Order{
		preOrder,
		inOrder,
		postOrder
	};
	template<class Operation> void orderRecursion(AvlNode<Key, Data>*, Operation, Order);

public:

	AvlTree(const Compare& Cmp) : root(NULL), max_node(NULL), cmp(Cmp), treeSize(0){}
	~AvlTree(){
		destroyTree(root);
	}

	void insert(const Key& key, const Data& data);
	void remove(const Key& key);
	Data& find(const Key& key);

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

template<class Key, class Data>
void AvlNode<Key, Data> :: updateNodeInfo(){
	int hLeft = left ? left->height : -1;
	int hRight = right ? right->height : -1;
	height = max(hLeft, hRight) + 1;
	bf = hLeft - hRight;
}

template<class Key, class Data>
void AvlNode<Key, Data> :: set_son(AvlNode<Key, Data>* son, Side side){
	(side == Side::left_side) ? left = son : right = son;
	if (son){
		son->parent = this;
	}
	updateNodeInfo();
}

template<class Key, class Data>
void AvlNode<Key, Data> :: set_right(AvlNode<Key, Data>* v){
	set_son(v, Side::right_side);
}

template<class Key, class Data>
void AvlNode<Key, Data> :: set_left(AvlNode<Key, Data>* v){
	set_son(v, Side::left_side);
}

template<class Key, class Data>
AvlNode<Key, Data>* AvlNode<Key, Data> :: rotate_side(Side side){
	AvlNode<Key, Data>* newRoot = (side == Side::right_side) ? left : right;

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

template<class Key, class Data>
AvlNode<Key, Data>* AvlNode<Key, Data> :: rotate_right(){
	return rotate_side(Side::right_side);
}

template<class Key, class Data>
AvlNode<Key, Data>* AvlNode<Key, Data> :: rotate_left(){
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
	assert(root);
	AvlNode<Key, Data>* current = root;
	AvlNode<Key, Data>* next = NULL; //Pointer to left\right field (which is also a pointer) of current
	bool found = false;

	while (!found) {
		switch (cmp(key, current->key)){
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
	if (cmp(key, v->key) == 0){
		return v->data;
	}
	throw dataDoesNotExist();
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
	return max;
}


//////////////////////////////////////////////
//////	AvlTree insert/remove methods	//////
//////////////////////////////////////////////

template<class Key, class Data, class Compare>
AvlNode<Key, Data>* AvlTree<Key, Data, Compare> :: doInsert(const Key& key, const Data& data){
	AvlNode<Key, Data>* v = do_find(data);

	switch (cmp(v->key, key)){
	case 0:
		throw dataAlreadyExists();
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
	if (cmp(v->key, key) != 0){
		throw dataDoesNotExist();
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
		throw avlIsEmpty();
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
void AvlTree<Key, Data, Compare> :: orderRecursion(AvlNode<Key, Data>* v, Operation op, Order order){
	if (!v){
		return;
	}
	if (order == Order::preOrder) {op(v->data);}
	inorderRecursion(v->right);
	if (order == Order::inOrder) {op(v->data);}
	inorderRecursion(v->left);
	if (order == Order::postOrder) {op(v->data);}
}

template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: preorder(Operation op){
	orderRecursion(root, op, Order::preOrder);
}

template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: inorder(Operation op){
	orderRecursion(root, op, Order::inOrder);
}

template<class Key, class Data, class Compare>
template<class Operation>
void AvlTree<Key, Data, Compare> :: postorder(Operation op){
	orderRecursion(root, op, Order::postOrder);
}

#endif /* AVL_H_ */
