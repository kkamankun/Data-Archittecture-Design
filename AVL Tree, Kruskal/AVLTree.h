#include "AVLNode.h"
#include <iostream>
#include <fstream>// header file declare
using namespace std;
class AVLTree
{
public:
	AVLNode * root;	// AVLTree Root
	ofstream * flog;
	int size; // AVLTree size

public:
	AVLTree(ofstream * flog); 
	~AVLTree();

	int			flag_Tree = 0; // Variable value is 1, if tree configuration complete
	int			flag_Delete = 0; // Variable value is 1, if node deletion complete
	AVLNode  *  SearchNode = NULL; // Nodes whose city location information matches the key value
	AVLNode  *  Getroot();
	void	 	Setroot(AVLNode * node);
	bool		Insert(CityData* node);
	bool		Delete(int num);
	CityData *	Search(int num);
	bool		Print();
	void		InOrder(AVLNode *node);				// Inorder traversal(Recursion)
	void		printData(AVLNode *node);			// Print informations of cities
	void		PostOrder(AVLNode *node, int key); // Postorder traversal(Recursion)
	void		PreOrder(AVLNode *node); // Preorder traversal(Recursion)
	void		Update_BF();	// Update balance factor of whole nodes
	int			GetHeight(AVLNode *node); // Return Height(Recursion)
	void		SetSize(int);	// Set size of AVLTree
	int			GetSize();	// return AVLTree size
};

