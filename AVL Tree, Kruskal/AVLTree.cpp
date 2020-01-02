#include "AVLTree.h"
#include <queue>
#include <utility>

AVLTree::AVLTree(ofstream * flog)
{
	Setroot(NULL);
	this->flog = flog;
	size = 0;
}

AVLTree::~AVLTree()
{
}

AVLNode * AVLTree::Getroot()
{
	return this->root;
}

void AVLTree::Setroot(AVLNode * node)
{
		this->root = node;
		return;
}

bool AVLTree::Insert(CityData* node)
{
	// Special case : emptry tree
	if (root == NULL)
	{
		this->root = new AVLNode;
		this->root->SetCityData(node);
		this->root->SetLeft(NULL);
		this->root->SetRight(NULL);
		this->root->SetmBF(0);
		return true;
	}

	// Phase 1: Locate insertion point  
	AVLNode *a = Getroot(),	// most recent node with bf = +-1
		*pa = NULL,	// parent of a
		*p = Getroot(), // p moves through the tree
		*pp = NULL, // parent of p
		*rootSub = NULL;

	while (p != NULL) {
		if (p->GetmBF() != 0) { a = p; pa = pp; }
		if (strcmp(node->Getname(), p->GetCityData()->Getname()) < 0) { pp = p; p = p->GetLeft(); } // take left branch
		else if (strcmp(node->Getname(), p->GetCityData()->Getname()) > 0) { pp = p; p = p->GetRight(); } // take right branch
		else return true;
	}

	// Phase 2: Insert and rebalance. 
	AVLNode *y = new AVLNode;
	y->SetCityData(node);
	y->SetLeft(NULL);
	y->SetRight(NULL);
	y->SetmBF(0);
	if (strcmp(node->Getname(), pp->GetCityData()->Getname()) < 0) { pp->SetLeft(y); } // Insert as left child
	else if (strcmp(node->Getname(), pp->GetCityData()->Getname()) > 0) { pp->SetRight(y); } // Insert as right child

	// Adjust balance factors of nodes on path from a to pp.
	int d;
	AVLNode *b, // child of a
		*c; // child of b

	if (strcmp(node->Getname(), a->GetCityData()->Getname()) > 0) { b = p = a->GetRight(); d = -1; }
	else { b = p = a->GetLeft(); d = 1; }

	while (p != y) {
		if (strcmp(node->Getname(), p->GetCityData()->Getname()) > 0) { // height of right increases by 1
			p->SetmBF(-1); p = p->GetRight();
		}
		else { // height of left increases by 1
			p->SetmBF(1); p = p->GetLeft();
		}
	}

	// Is tree unbalanced?
	if (a->GetmBF() == 0 || a->GetmBF() + d == 0) { // tree still balanced
		a->SetmBF(a->GetmBF() + d); return true;
	}

	// tree unbalanced, determine rotation type
	if (d == 1) { // left imbalance
		if (b->GetmBF() == 1) { // rotation type LL
			a->SetLeft(b->GetRight());
			b->SetRight(a); a->SetmBF(0); b->SetmBF(0);
			rootSub = b; // b is the new root of the subtree
		}
		else { // rotation type LR
			c = b->GetRight();
			b->SetRight(c->GetLeft());
			a->SetLeft(c->GetRight());
			c->SetLeft(b);
			c->SetRight(a);
			switch (c->GetmBF()) {
			case 0:
				b->SetmBF(0); a->SetmBF(0);
				break;
			case 1:
				a->SetmBF(-1); b->SetmBF(0);
				break;
			case -1:
				b->SetmBF(1); a->SetmBF(0);
				break;
			}
			c->SetmBF(0); rootSub = c; // c is the new root of the subtree
		} // end of LR
	} // end of left imbalance

	else { // right imbalance
		if (b->GetmBF() == -1) { // rotation type RR
			a->SetRight(b->GetLeft());
			b->SetLeft(a); a->SetmBF(0); b->SetmBF(0);
			rootSub = b; // b is the new root of the subtree
		}
		else { // rotation type RL
			c = b->GetLeft();
			b->SetLeft(c->GetRight());
			a->SetRight(c->GetLeft());
			c->SetRight(b);
			c->SetLeft(a);
			switch (c->GetmBF()) {
			case 0:
				b->SetmBF(0); a->SetmBF(0);
				break;
			case 1:
				b->SetmBF(-1); a->SetmBF(0);
				break;
			case -1:
				a->SetmBF(1); b->SetmBF(0);
				break;
			}
			c->SetmBF(0); rootSub = c; // c is the new root of the subtree
		}// end of RL
	} // end of right imbalance

	// Subtree with root b has been rebalanced.
	if (pa == NULL) Setroot(rootSub);
	else if (a == pa->GetLeft()) pa->SetLeft(rootSub);
	else pa->SetRight(rootSub);
	return true;
} // end of AVLTree::Insert

bool AVLTree::Delete(int k) // key = LocationId
{
	flag_Delete = 0;
	PostOrder(this->root, k);
	
	if (flag_Delete == 0)	// Not found
		return false;

	AVLNode* node = SearchNode; // delete node
	
	// Phase 1: Search delete node
	AVLNode *p = Getroot(), *pp = 0;
	AVLNode *a =  0, *pa = 0, *rootSub = NULL;
	
	while (p && node->GetCityData()->Getname() != p->GetCityData()->Getname()) {
		//if (p->GetmBF() != 0) { a = p; pa = pp; }
		if (strcmp(node->GetCityData()->Getname(), p->GetCityData()->Getname()) < 0 ) { pp = p; p = p->GetLeft(); }
		else { pp = p; p = p->GetRight(); }
	}

	// Phase 2: Delete node & Update balance factor
	if (p->GetLeft() == 0 && p->GetRight() == 0) // p is leaf
	{
		if (pp == 0) { Setroot(0);	return true; }
		else if (pp->GetLeft() == p) pp->SetLeft(0);
		else pp->SetRight(0);
		delete p;
		node = pp;
	}
	else if (p->GetLeft() == 0) // p only has right child
	{
		if (pp == 0) Setroot(p->GetRight());
		else if (pp->GetLeft() == p) pp->SetLeft(p->GetRight());
		else pp->SetRight(p->GetRight());
		delete p;
		node = pp;
	}
	else if (p->GetRight() == 0) // p only has left child
	{
		if (pp == 0) Setroot(p->GetLeft());
		else if (pp->GetLeft() == p) pp->SetLeft(p->GetLeft());
		else pp->SetRight(p->GetLeft());
		delete p;
		node = pp;
	}
	else // p has left and right child
	{
		AVLNode *prevprev = p, *prev = p->GetRight(), *curr = p->GetRight()->GetLeft();
		while (curr) {
			prevprev = prev;
			prev = curr;
			curr = curr->GetLeft();
		}
		p->SetCityData(prev->GetCityData());
		if (prevprev == p) prevprev->SetRight(prev->GetRight());
		else prevprev->SetLeft(prev->GetRight());
		delete prev;
		//node = prevprev->GetLeft();
		node = prevprev;
	}
	Update_BF();

	// Set A node
	p = Getroot(); pp = 0;
	if(node)
	while (p) {
		if (p->GetmBF() == 2 || p->GetmBF() == -2) { a = p; pa = pp; }
		if (node->GetCityData()->Getname() != p->GetCityData()->Getname()) 	break;
		if (strcmp(node->GetCityData()->Getname(), p->GetCityData()->Getname()) < 0) { pp = p; p = p->GetLeft(); }
		else { pp = p; p = p->GetRight(); }
	}

	// Return true, if tree balanced
	if (a == 0)  return true;
	
	// Phase 3: Rotate, if tree unbalanced
	// determine rotation type
	AVLNode *b = NULL, *c = NULL;
	if (a->GetmBF() == 2) { // left imbalance
		b = a->GetLeft();
		if(b)
		if (b->GetmBF() >= 0) { // rotation type LL
			a->SetLeft(b->GetRight());
			b->SetRight(a);
			rootSub = b; // b is the new root of the subtree
		}
		else { // rotation type LR
			c = b->GetRight();
			b->SetRight(c->GetLeft());
			a->SetLeft(c->GetRight());
			c->SetLeft(b);
			c->SetRight(a);
			rootSub = c; // c is the new root of the subtree
		} // end of LR
	} // end of left imbalance

	else { // right imbalance
		b = a->GetRight();
		if(b)
		if (b->GetmBF() <= 0) { // rotation type RR
			a->SetRight(b->GetLeft());
			b->SetLeft(a);
			rootSub = b; // b is the new root of the subtree
		}
		else { // rotation type RL
			c = b->GetLeft();
			b->SetLeft(c->GetRight());
			a->SetRight(c->GetLeft());
			c->SetRight(b);
			c->SetLeft(a);
			rootSub = c; // c is the new root of the subtree
		}// end of RL
	} // end of right imbalance
	
	// Subtree with root b has been rebalanced.
	if (pa == NULL) Setroot(rootSub);
	else if (a == pa->GetLeft()) pa->SetLeft(rootSub);
	else pa->SetRight(rootSub);

	// Phase 4: Update balance factor
	Update_BF();

	return true;

}
	

CityData * AVLTree::Search(int num)
{
	PostOrder(this->root, num);
	AVLNode * node = SearchNode;
	if (node == NULL)
		return NULL;
	else
		return node->GetCityData();
}

bool AVLTree::Print()
{
	if (!this->root) // Print error code, if tree does not exist
		return false;
	else
		return true;
}

void AVLTree::InOrder(AVLNode *node)
{
	if (node != NULL)
	{
		InOrder(node->GetLeft());
		printData(node);
		InOrder(node->GetRight());
	}
}

void AVLTree::printData(AVLNode *node) // Add print node->GetmBF()
{
	*flog << "( " << (node->GetCityData()->GetLocationId()) << ", "
		<< (node->GetCityData()->Getname()) << ", " << (node->GetCityData()->Getcountry()) << " )" //<< node->GetmBF()
		 << endl;
}

void AVLTree::PostOrder(AVLNode *node, int key)
{
	if (node != NULL)
	{
		PostOrder(node->GetLeft(), key);
		PostOrder(node->GetRight(), key);
		if (node->GetCityData()->GetLocationId() == key) {
			SearchNode = node;
			flag_Delete = 1;
		}
	}		
}

void AVLTree::Update_BF()
{
	queue <AVLNode *> q;
	AVLNode *p = root;

	while (p) {
		int leftheight = GetHeight(p->GetLeft());
		int rightheight = GetHeight(p->GetRight());

		p->SetmBF(leftheight - rightheight);

		if (p->GetLeft())
			q.push(p->GetLeft());
		if (p->GetRight())
			q.push(p->GetRight());
		if (q.empty())
			break;

		p = q.front();
		q.pop();
	}
}

int AVLTree::GetHeight(AVLNode* node)
{
	if (node == NULL) 	return 0;
	
	return max(GetHeight(node->GetLeft()), GetHeight(node->GetRight())) + 1;
}

void AVLTree::PreOrder(AVLNode *node)
{
	if (node != NULL)
	{
		size += 1;
		PreOrder(node->GetLeft());
		PreOrder(node->GetRight());
	}
}

void AVLTree::SetSize(int size)
{
	this->size = size;
}

int	AVLTree::GetSize()
{
	return this->size;
}
