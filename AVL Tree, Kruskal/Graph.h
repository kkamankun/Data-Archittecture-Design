#include "AVLTree.h"
#include <iostream>
#include <fstream>// header file declare
#include <map>
using namespace std;
class Graph
{
public:
	map<int, CityData *> * mList;	
	CityData * * vertex;			// vetex[index] = CityData * 
	pair<int, pair<int, int>> * mstMatrix;				// MST

	ofstream * flog;

public:
	Graph(ofstream * flog);
	~Graph();

	bool	Build(AVLTree * tree, int size);
	bool 	Print_GP(int);
	bool	Print_MST(int);
	void	InOrderInit(AVLNode *);			// Initialize element of verticies
	int		GetDistance(CityData*, CityData*);  // Calculate distance between two cities
	int		idx = 0; // index of vertex

	/*for Kruskal union_set*/
	bool	Kruskal(int);
	void 	make_set(int);
	void 	union_set(int x, int y);
	int		find(int x);
	int *	set;
	int		check(int, int);
};


