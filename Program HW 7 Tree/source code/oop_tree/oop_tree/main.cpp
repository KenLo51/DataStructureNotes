#include "BinaryTree.h"

#include "stdio.h"
int main() {
	// Complete Binary Tree //////////////////////////////////////////
	printf("Complete Binary Tree:\n");

	// init tree
	CompleteBinaryTree<int> cbt;
	for (int i = 0; i < 8; i++) cbt.Insert(i);

	// print tree
	printf("  Inorder Traversal   : ");
	for (BinaryTree<int>::IteratorInorder it = cbt.beginInorder(); it != cbt.endInorder(); it++) {
		printf("%2d ", it->data); }
	printf("\n");

	printf("  Preorder Traversal  : ");
	for (BinaryTree<int>::IteratorPreorder it = cbt.beginPreorder(); it != cbt.endPreorder(); it++) {
		printf("%2d ", it->data); }
	printf("\n");

	// Max-Heap //////////////////////////////////////////
	printf("MaxHeap:\n");

	// init tree
	MaxHeap<int, int> mh;
	for (int i = 0; i < 8; i++) 
		mh.Insert(i, i);

	// print tree
	printf("  Inorder Traversal   : ");
	for (BinaryTree<std::pair<int, int>>::IteratorInorder it = mh.beginInorder(); it != mh.endInorder(); it++) {
		printf("%2d ", it->data.first); }
	printf("\n");

	printf("  Extract Max         : ");
	while (mh.Size()>0) {
		int maxval = mh.ExtractMax();
		printf("%2d ", maxval);
	}
	printf("\n");

	// Binary Search Tree //////////////////////////////////////////
	printf("Binary Search Tree:\n");
	// init tree
	BinarySearchTree<int, int> bst;
	int d[8] = { 3, 7, 11, 6, 37, 28, 1, 4 };
	for (int i = 0; i < 8; i++)
		bst.Insert(d[i], d[i]);

	// print tree
	printf("  Inorder Traversal   : ");
	for (BinaryTree<std::pair<int, int>>::IteratorInorder it = bst.beginInorder(); it != bst.endInorder(); it++) {
		printf("%2d ", it->data.first);
	}
	printf("\n");

	printf("  Preorder Traversal  : ");
	for (BinaryTree<std::pair<int, int>>::IteratorPreorder it = bst.beginPreorder(); it != bst.endPreorder(); it++) {
		printf("%2d ", it->data.first);
	}
	printf("\n");

	printf("  Postorder Traversal : ");
	for (BinaryTree<std::pair<int, int>>::IteratorPostorder it = bst.beginPostorder(); it != bst.endPostorder(); it++) {
		printf("%2d ", it->data.first);
	}
	printf("\n");

	printf("  Levelorder Traversal: ");
	for (BinaryTree<std::pair<int, int>>::IteratorLevelorder it = bst.beginLevelorder(); it != bst.endLevelorder(); it++) {
		printf("%2d ", it->data.first);
	}
	printf("\n");

	getchar();

	return 0;
}