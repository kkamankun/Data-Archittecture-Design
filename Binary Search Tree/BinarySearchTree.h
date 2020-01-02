#pragma once

#include "TreeNode.h"
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

class BinarySearchTree
{
	friend class TransferWindowManager;
private:
	TreeNode * m_root;
	vector<SoccerPlayerData> v;
public:
	BinarySearchTree() { m_root = NULL; }
	
	~BinarySearchTree() { deleteTree(m_root); }

	void insert(SoccerPlayerData& data);
	void deletion(int ability); // ability = key
	void deleteTree(TreeNode* node)
	{
		if (node)
		{
			deleteTree(node->m_left);
			deleteTree(node->m_right);
			delete node;
		}
	}
	static void inorder(TreeNode* node)
	{
		if (node)
		{
			inorder(node->m_left);
			cout << node;
			inorder(node->m_right);
		}
	}
	
	friend std::ostream& operator<<(std::ostream& os, const BinarySearchTree& tree) 
	{
		TreeNode* root = tree.m_root;
		inorder(root);

		return os;
		
		//stack<TreeNode*> s;

		//TreeNode* curr = tree.m_root;

		//while (true)
		//{
		//	// Search leftmost node and push node
		//	// L
		//	while (curr != NULL)
		//	{
		//		s.push(curr);
		//		curr = curr->getLeftNode();
		//	}

		//	// Execute, if element exists in stack
		//	if (!s.empty())
		//	{
		//		// curr = leftmost node
		//		curr = s.top();
		//		s.pop();
		//		// V
		//		os << curr;
		//		// R
		//		curr = curr->getRightNode();
		//	}
		//	else
		//		break;
		//}
	}

	void print_v();
};
