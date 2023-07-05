#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include <stdio.h>
#include <iterator>
#include <stack>
#include <queue>
#include <math.h>
#include <utility>

template <class T>
struct BinaryTreeNode {
    T data;
    struct BinaryTreeNode* left;
    struct BinaryTreeNode* right;
    struct BinaryTreeNode* parent;

    struct BinaryTreeNode() : left(NULL), right(NULL), parent(NULL){};
    struct BinaryTreeNode(T data) : left(NULL), right(NULL), parent(NULL), data(data) {};
    struct BinaryTreeNode(struct BinaryTreeNode* parent, T data) : left(NULL), right(NULL), parent(parent), data(data) {};
    struct BinaryTreeNode(struct BinaryTreeNode* left, struct BinaryTreeNode* right, T data) : left(left), right(right), parent(NULL), data(data) {};
    struct BinaryTreeNode(struct BinaryTreeNode* left, struct BinaryTreeNode* right, struct BinaryTreeNode* parent, T data) : left(left), right(right), parent(parent), data(data) {};
};

template <class T>
class BinaryTree {
// defined variables
protected:
//public:
    BinaryTreeNode<T>* root;
    unsigned int size;
public:
    // Iterators
    struct IteratorInorder{
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = BinaryTreeNode<T>;
        using pointer = BinaryTreeNode<T>*;
        using reference = BinaryTreeNode<T>&;

        IteratorInorder(pointer ptr){
            node_ptr = ptr;
            if (node_ptr == NULL) return; // empty tree
            visited.push(false);
            path.push(node_ptr);
            ++(*this);
        }

        reference operator*() const { return *node_ptr; }
        pointer operator->() { return node_ptr; }
        IteratorInorder& operator++() {
            if (visited.top()) {
                // go right
                if (node_ptr->right) {
                    node_ptr = node_ptr->right;
                    path.push(node_ptr);
                    visited.push(false);
                }
                // return to the parent which is un-traveled
                else {
                    while (visited.size()) {
                        if (visited.top() == false) break;
                        path.pop();
                        visited.pop();
                    }
                    if (visited.size() == 0) { // all nodes have been traveled
                        node_ptr = NULL;
                        return *this;
                    }
                    node_ptr = path.top();
                    visited.top() = true;
                    return *this;
                }
            }

            // goto leftest node
            node_ptr = node_ptr->left;
            while (node_ptr != NULL) {
                path.push(node_ptr);
                visited.push(false);
                node_ptr = node_ptr->left;
            }
            node_ptr = path.top();
            visited.top() = true;

            return *this;
        }

        IteratorInorder operator++(int) { IteratorInorder tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const IteratorInorder& a, const IteratorInorder& b) { return a.node_ptr == b.node_ptr; };
        friend bool operator!= (const IteratorInorder& a, const IteratorInorder& b) { return a.node_ptr != b.node_ptr; };

    private:
        std::stack<pointer> path;
        std::stack<bool> visited;
        pointer node_ptr;
    };
    struct IteratorPreorder {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = BinaryTreeNode<T>;
        using pointer = BinaryTreeNode<T>*;
        using reference = BinaryTreeNode<T>&;

        IteratorPreorder(pointer ptr) {
            node_ptr = ptr;
            if (node_ptr == NULL) return; // empty tree
            visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
            path.push(node_ptr);
        }

        reference operator*() const { return *node_ptr; }
        pointer operator->() { return node_ptr; }
        IteratorPreorder& operator++() {
            // go to left child first if possable
            if (node_ptr->left != NULL) {
                node_ptr = node_ptr->left;
                visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
                path.push(node_ptr);
                return *this;
            }
            // go to right child if it does not have left child
            if (node_ptr->right != NULL) {
                node_ptr = node_ptr->right;
                visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
                path.push(node_ptr);
                return *this;
            }
            // does not have children
            while (visited.size() > 0){
                if (visited.top() == false) break;
                visited.pop();
                path.pop();
            }
            if (visited.size() == 0) { // all nodes have been traveled
                node_ptr = NULL;
                return *this;
            }
            visited.top() = true;
            node_ptr = path.top();
            if (node_ptr->right) {
                node_ptr = node_ptr->right;
                visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
                path.push(node_ptr);
                return *this;
            }

            return *this;
        }

        IteratorPreorder operator++(int) { IteratorPreorder tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const IteratorPreorder& a, const IteratorPreorder& b) { return a.node_ptr == b.node_ptr; };
        friend bool operator!= (const IteratorPreorder& a, const IteratorPreorder& b) { return a.node_ptr != b.node_ptr; };

    private:
        std::stack<pointer> path;
        std::stack<bool> visited;
        pointer node_ptr;
    };
    struct IteratorPostorder {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = BinaryTreeNode<T>;
        using pointer = BinaryTreeNode<T>*;
        using reference = BinaryTreeNode<T>&;

        IteratorPostorder(pointer ptr) {
            node_ptr = ptr;
            if (node_ptr == NULL) return; // empty tree
            visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
            path.push(node_ptr);
            
            while ((node_ptr->left != NULL) || (node_ptr->right != NULL)) {
                // go to left child first if possable
                if (node_ptr->left) {
                    node_ptr = node_ptr->left;
                    visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
                    path.push(node_ptr);
                    continue;
                }
                // go to right child if it does not have left child
                if (node_ptr->right) {
                    node_ptr = node_ptr->right;
                    visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
                    path.push(node_ptr);
                    continue;
                }
            }
        }

        reference operator*() const { return *node_ptr; }
        pointer operator->() { return node_ptr; }
        IteratorPostorder& operator++() {
            if (path.size() <= 1) {
                node_ptr = NULL;
                return *this;
            }
            path.pop(); visited.pop();
            node_ptr = path.top();
            if (visited.top()) {
                return *this;
            }
            else {
                visited.top() = true;
                node_ptr = node_ptr->right;
                visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
                path.push(node_ptr);

                while ((node_ptr->left != NULL) || (node_ptr->right != NULL)) {
                    // go to left child first if possable
                    if (node_ptr->left) {
                        node_ptr = node_ptr->left;
                        visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
                        path.push(node_ptr);
                        continue;
                    }
                    // go to right child if it does not have left child
                    if (node_ptr->right) {
                        node_ptr = node_ptr->right;
                        visited.push((node_ptr->left == NULL) || (node_ptr->right == NULL));
                        path.push(node_ptr);
                        continue;
                    }

                    // have traveled to leaf
                    return *this;
                }
            }

            return *this;
        }

        IteratorPostorder operator++(int) { IteratorPostorder tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const IteratorPostorder& a, const IteratorPostorder& b) { return a.node_ptr == b.node_ptr; };
        friend bool operator!= (const IteratorPostorder& a, const IteratorPostorder& b) { return a.node_ptr != b.node_ptr; };

    private:
        std::stack<pointer> path;
        std::stack<bool> visited;
        pointer node_ptr;
    };
    struct IteratorLevelorder {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = BinaryTreeNode<T>;
        using pointer = BinaryTreeNode<T>*;
        using reference = BinaryTreeNode<T>&;

        IteratorLevelorder(pointer ptr) {
            node_ptr = ptr;
            if (node_ptr == NULL) return; // empty tree
            if (node_ptr->left) nextNodes.push(node_ptr->left);
            if (node_ptr->right) nextNodes.push(node_ptr->right);
        }

        reference operator*() const { return *node_ptr; }
        pointer operator->() { return node_ptr; }
        IteratorLevelorder& operator++() {
            if (nextNodes.size() <= 0) {
                node_ptr = NULL;
                return *this;
            }

            node_ptr = nextNodes.front();
            nextNodes.pop();
            if (node_ptr->left) nextNodes.push(node_ptr->left);
            if (node_ptr->right) nextNodes.push(node_ptr->right);

            return *this;
        }

        IteratorLevelorder operator++(int) { IteratorLevelorder tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const IteratorLevelorder& a, const IteratorLevelorder& b) { return a.node_ptr == b.node_ptr; };
        friend bool operator!= (const IteratorLevelorder& a, const IteratorLevelorder& b) { return a.node_ptr != b.node_ptr; };

    private:
        std::queue<pointer> nextNodes;
        pointer node_ptr;
    };
    
    typedef IteratorLevelorder Iterator;
// defined functions
private:
public:
    // default constructor
    BinaryTree() {
        root = NULL;
        size = 0;
    }

    // copy constructor
    BinaryTree(BinaryTree& src) {
        CopyTree(src.root, root);
    }
    BinaryTreeNode<T>* CopyTree(BinaryTreeNode<T>* srcNode, BinaryTreeNode<T>* dstNode) {
        if (srcNode == NULL) return NULL;
        dstNode = new BinaryTreeNode<T>;
        dstNode->left = CopyTree(srcNode->left);
        dstNode->left->parent = dstNode;
        dstNode->right = CopyTree(srcNode->right);
        dstNode->right->parent = dstNode;
        return dstNode;
    }

    // destructor
    ~BinaryTree() {
        DeleteTree(root);
    }
    void DeleteTree(BinaryTreeNode<T>* node = root) {
        if (node == NULL) return;
        DeleteTree(node->left);
        DeleteTree(node->right);
        if (node->parent) {
            if (node->parent->left == node) node->parent->left = NULL;
            if (node->parent->right == node) node->parent->right = NULL;
        }
        if (node == root) root = NULL;
        delete node;
    }

    unsigned int Size(void) { return BinaryTree::size; }

    Iterator begin() { return Iterator(root); }
    Iterator end() { return Iterator(NULL); }

    IteratorInorder beginInorder() { return IteratorInorder(root); }
    IteratorInorder endInorder() { return IteratorInorder(NULL); }
    IteratorPreorder beginPreorder() { return IteratorPreorder(root); }
    IteratorPreorder endPreorder() { return IteratorPreorder(NULL); }
    IteratorPostorder beginPostorder() { return IteratorPostorder(root); }
    IteratorPostorder endPostorder() { return IteratorPostorder(NULL); }
    IteratorLevelorder beginLevelorder() { return IteratorLevelorder(root); }
    IteratorLevelorder endLevelorder() { return IteratorLevelorder(NULL); }
};



template <class T>
class CompleteBinaryTree :public BinaryTree<T> {
public:
    BinaryTreeNode<T>* Insert(T data) {
        BinaryTree<T>::size = BinaryTree<T>::size + 1;
        if (BinaryTree<T>::size == 1) { // insert as a root
            BinaryTree<T>::root = new BinaryTreeNode<T>(data);
            return BinaryTree<T>::root;
        }
        // already has a root
        BinaryTreeNode<T>* ptr = BinaryTree<T>::root;
        unsigned int level = log2f(BinaryTree<T>::size) + 1.0f;
        for (unsigned int mask = 1 << (level - 2); mask > 0; mask = mask >> 1) {
            if (mask & BinaryTree<T>::size) {
                // go right
                if (ptr->right == NULL) {
                    ptr->right = new BinaryTreeNode<T>(data);
                    ptr->right->parent = ptr;
                    return ptr->right;
                }
                else { ptr = ptr->right; }
            }
            else {
                // go left
                if (ptr->left == NULL) {
                    ptr->left = new BinaryTreeNode<T>(data);
                    ptr->left->parent = ptr;
                    return ptr->left;
                }
                else { ptr = ptr->left; }
            }
        }
        return NULL;
    }
    BinaryTreeNode<T>* Back(void) {
        // already has a root
        BinaryTreeNode<T>* ptr = BinaryTree<T>::root;
        unsigned int level = log2f(BinaryTree<T>::size) + 1.0f;
        for (unsigned int mask = 1 << (level - 2); mask > 0; mask = mask >> 1) {
            if (mask & BinaryTree<T>::size) {
                // go right
                if (ptr->right != NULL)
                    ptr = ptr->right;
            }
            else {
                // go left
                if (ptr->left != NULL)
                    ptr = ptr->left;
            }
        }
        return ptr;
    }
protected:
};


template <class T_key, class T_data>
class MaxHeap :public CompleteBinaryTree<std::pair<T_key, T_data>> {
public:
    void Insert(T_key key, T_data data) {
        std::pair<T_key, T_data> hNode(key, data);
        Insert(hNode);
    }
    void Insert(std::pair<T_key, T_data> hNode) {
        BinaryTreeNode<std::pair<T_key, T_data>>* btNode = CompleteBinaryTree<std::pair<T_key, T_data>>::Insert(hNode);
        if (btNode == NULL) return;
        Heapify_buttomUp(btNode);
    }

    T_data* Max(void) {
        if (CompleteBinaryTree<std::pair<T_key, T_data>>::root == NULL) return NULL;
        return &(CompleteBinaryTree<std::pair<T_key, T_data>>::root->data.second);
    }
    T_data ExtractMax(void) {
        T_data returnData = CompleteBinaryTree<std::pair<T_key, T_data>>::root->data.second;

        // replace root with last node
        CompleteBinaryTree<std::pair<T_key, T_data>>::root->data = CompleteBinaryTree<std::pair<T_key, T_data>>::Back()->data;
        CompleteBinaryTree<std::pair<T_key, T_data>>::DeleteTree(CompleteBinaryTree<std::pair<T_key, T_data>>::Back());

        //Heapify
        CompleteBinaryTree<std::pair<T_key, T_data>>::size--;
        Heapify_topDown(CompleteBinaryTree<std::pair<T_key, T_data>>::root);
        return returnData;
    }
private:
    void Heapify_buttomUp(BinaryTreeNode<std::pair<T_key, T_data>>* node) {
        if (CompleteBinaryTree<std::pair<T_key, T_data>>::size == 0) return;
        while (node != CompleteBinaryTree<std::pair<T_key, T_data>>::root) {
            node = node->parent;
            if (node == NULL) return; // root node
            // find the node with maximum key in parent and sibling
            BinaryTreeNode<std::pair<T_key, T_data>>* maxNode = node;
            if (node->left != NULL)
                if (node->left->data.first > maxNode->data.first) maxNode = node->left;
            if (node->right != NULL)
                if (node->right->data.first > maxNode->data.first) maxNode = node->right;

            // swap 2 node
            if (maxNode == node) return; // Heapify completed
            std::swap(maxNode->data, node->data);
        }
        return;
    }
    void Heapify_topDown(BinaryTreeNode<std::pair<T_key, T_data>>* node) {
        if (CompleteBinaryTree<std::pair<T_key, T_data>>::size == 0) return;
        while (1) {
            // find the node with maximum key in parent and sibling
            BinaryTreeNode<std::pair<T_key, T_data>>* maxNode = node;
            if (node == NULL) return; // root node
            if (node->left != NULL)
                if (node->left->data.first > maxNode->data.first) maxNode = node->left;
            if (node->right != NULL)
                if (node->right->data.first > maxNode->data.first) maxNode = node->right;

            // swap 2 node
            if (maxNode == node) return; // Heapify completed
            std::swap(maxNode->data, node->data);
            node = maxNode;
        }
        return;
    }
};


template <class T_key, class T_data>
class BinarySearchTree :public BinaryTree<std::pair<T_key, T_data>> {
protected:
    BinaryTreeNode<std::pair<T_key, T_data>>* Rotation_R(BinaryTreeNode<std::pair<T_key, T_data>>* node) {
        BinaryTreeNode<std::pair<T_key, T_data>>* nodeA = node;
        if (nodeA == NULL) return;
        BinaryTreeNode<std::pair<T_key, T_data>>* nodeB = node->left;
        if (nodeB == NULL) return;

        // get sub-trees
        BinaryTreeNode<std::pair<T_key, T_data>>* subTree1 = nodeB->left;
        BinaryTreeNode<std::pair<T_key, T_data>>* subTree2 = nodeB->right;
        BinaryTreeNode<std::pair<T_key, T_data>>* subTree3 = nodeA->right;

        // adject parent node and the link to the parent
        if (nodeA == BinaryTree<std::pair<T_key, T_data>>::root) {
            BinaryTree<std::pair<T_key, T_data>>::root = nodeB;
            nodeB->parent = NULL;
        }
        else {
            if (nodeA->parent->left == nodeA) nodeA->parent->left = nodeB;
            else nodeA->parent->right = nodeB;
            nodeB->parent = nodeA->parent;
        }
        // rotate A and B
        nodeA->parent = nodeB;
        nodeB->right = nodeA;

        // link sub-trees
        nodeB->left = subTree1;
        nodeA->left = subTree2;
        nodeA->right = subTree3;
        
        // return the top node
        return nodeB;
    }
    BinaryTreeNode<std::pair<T_key, T_data>>* Rotation_L(BinaryTreeNode<std::pair<T_key, T_data>>* node) {
        BinaryTreeNode<std::pair<T_key, T_data>>* nodeA = node;
        if (nodeA == NULL) return;
        BinaryTreeNode<std::pair<T_key, T_data>>* nodeB = node->right;
        if (nodeB == NULL) return;

        // get sub-trees
        BinaryTreeNode<std::pair<T_key, T_data>>* subTree1 = nodeA->left;
        BinaryTreeNode<std::pair<T_key, T_data>>* subTree2 = nodeB->left;
        BinaryTreeNode<std::pair<T_key, T_data>>* subTree3 = nodeB->right;

        // adject parent node and the link to the parent
        if (nodeA == BinaryTree<std::pair<T_key, T_data>>::root) {
            BinaryTree<std::pair<T_key, T_data>>::root = nodeB;
            nodeB->parent = NULL;
        }
        else {
            if (nodeA->parent->right == nodeA) nodeA->parent->right = nodeB;
            else nodeA->parent->left = nodeB;
            nodeB->parent = nodeA->parent;
        }
        // rotate A and B
        nodeA->parent = nodeB;
        nodeB->left = nodeA;

        // link sub-trees
        nodeA->left = subTree1;
        nodeA->right = subTree2;
        nodeB->right = subTree3;

        // return the top node
        return nodeB;
    }

public:
    BinaryTreeNode<std::pair<T_key, T_data>>* Insert(T_key key, T_data data) {
        BinaryTreeNode<std::pair<T_key, T_data>>* node = BinaryTree<std::pair<T_key, T_data>>::root;
        while (node != NULL) {
            // return data if the key is match, replace data
            if (key == node->data.first) {
                node->data.second = data;
            }
            // if key is smaller then current node, goto left child
            if (key < node->data.first) {
                if (node->left) {
                    node = node->left;
                    continue;
                }
                else {
                    BinaryTreeNode<std::pair<T_key, T_data>>* newNode =
                        new BinaryTreeNode<std::pair<T_key, T_data>>(node, std::pair<T_key, T_data>(key, data));
                    node->left = newNode;
                    return newNode;
                }
            }

            // if key is greater then current node, goto right child
            if (key > node->data.first) {
                if (node->right) {
                    node = node->right;
                    continue;
                }
                else {
                    BinaryTreeNode<std::pair<T_key, T_data>>* newNode =
                        new BinaryTreeNode<std::pair<T_key, T_data>>(node, std::pair<T_key, T_data>(key, data));
                    node->right = newNode;
                    return newNode;
                }
            }
        }
        // root is null pointer
        BinaryTree<std::pair<T_key, T_data>>::root = new BinaryTreeNode<std::pair<T_key, T_data>>(std::pair<T_key, T_data>(key, data));
        return BinaryTree<std::pair<T_key, T_data>>::root;
    }
    T_data Search(T_key key, T_data defVal) {
        BinaryTreeNode<std::pair<T_key, T_data>>* node = BinaryTree<std::pair<T_key, T_data>>::root;
        while (node != NULL) {
            // return data if the key is match
            if (key == node->data.first) {
                return node->data.second;
            }
            // if key is smaller then current node, goto left child
            if (key < node->data.first) {
                node = node->left;
                continue;
            }

            // if key is greater then current node, goto right child
            if (key > node->data.first) {
                node = node->right;
                continue;
            }
        }
        // can not find key
        return defVal;
    }
};

#endif