#include "BinarySearchTree.h"
using namespace std;

void BinarySearchTree::insert(SoccerPlayerData& data)
{
	TreeNode *pp = NULL;
	TreeNode *p = m_root;

	// root가 없다면 root 생성
	if (m_root == NULL)
	{
		m_root = new TreeNode(data, NULL, NULL);
		v.push_back(data);
	}
	
	// root가 있다면
	else
	{
		p = m_root;

		//탐색할 위치 탐색
		while (p != NULL)
		{
			pp = p;

			if (p->m_data.m_ability < data.m_ability)
				p = p->m_right;
			else if (p->m_data.m_ability > data.m_ability)
				p = p->m_left;
		}

		// insert
		if (pp->m_data.m_ability > data.m_ability)
		{
			pp->m_left = new TreeNode(data, NULL, NULL);
			v.push_back(data);
		}
		else
		{
			pp->m_right = new TreeNode(data, NULL, NULL);
			v.push_back(data);
		}
	}
}

void BinarySearchTree::deletion(int ability) // ability = key
{
	TreeNode *pp = NULL;
	TreeNode *p = m_root;
	TreeNode *c = NULL;
	TreeNode *pPredecessor = NULL;
	TreeNode *pSuccessor = NULL;

	// key를 갖는 노드 p를 탐색한다.
	while (p != NULL && p->m_data.m_ability != ability)
	{
		pp = p;
		p = (ability < pp->m_data.m_ability) ? pp->m_left : pp->m_right;
	}

	// 탐색이 끝났는데 p가 NULL이라면 트리에 없다.
	if (!p)
	{
		return;
	}
	
	// p는 삭제할 노드
	// case 1 : p의 자식이 없을 때
	if (p->m_left == NULL && p->m_right == NULL)
	{
		if (pp)
		{
			if (pp->m_left == p)
				pp->m_left = NULL;
			else
				pp->m_right = NULL;
		}
		else // 부모노드가 NULL이라면 삭제하려는 노드가 루트 노드
			m_root = NULL;
	}

	// case 3 : p의 자식이 2개 있을 때
	else if(p->m_left != NULL && p->m_right!=NULL)
	{
		pPredecessor = p;
		pSuccessor = p->m_left;
		while (pSuccessor->m_right != NULL)
		{
			pPredecessor = pSuccessor;
			pSuccessor = pSuccessor->m_right;
		}

		pPredecessor->m_right = pSuccessor->m_left;
		pSuccessor->m_left = p->m_left;
		pSuccessor->m_right = p->m_right;

		if (pp != NULL) {
			if (pp->m_left == p) {
				pp->m_left = pSuccessor;
			}
			else {
				pp->m_right = pSuccessor;
			}
		}
		else {
			m_root = pSuccessor;
		}
	}

	// case 2 : p의 자식이 1개 있을 때
	else if ((p->m_left == NULL) || (p->m_right == NULL))
	{
		c = (p->m_left != NULL) ? p->m_left : p->m_right;
		if (pp)
		{
			if (pp->m_left == p)
				pp->m_left = c;
			else
				pp->m_right = c;
		}
		else
			m_root = c;
	}
	delete p;
}

void BinarySearchTree::print_v()
{
	cout << "print_v size : " << v.size() << endl;
	for (int i = 0; i < v.size(); i++)
	{
		// cout << v.size();
		cout << v[i].m_name << ' ';
	}
}


