#include "Graph.h"
#include <map>
#include <iomanip>
#include <vector>
#include <algorithm>

Graph::Graph(ofstream * flog)
{
	this->mList = NULL;				// mList[from vetex] = map<to vertex, weigth>
	this->vertex = NULL;			// vetex[index] = CityData * 
	this->mstMatrix = NULL;			// MST

	this->flog = flog;
}

Graph::~Graph()
{
}

bool Graph::Build(AVLTree * tree, int size)
{
	if (this->mList != NULL) {
		delete[] this->mList;
		delete[] this->vertex;
		delete[] this->set;
		delete[] this->mstMatrix;

		this->mList = NULL;				// mList[from vetex] = map<to vertex, weigth>
		this->vertex = NULL;			// vetex[index] = CityData * 
		this->mstMatrix = NULL;
		this->set = NULL;
		this->idx = 0;
	}
	AVLNode* root = tree->Getroot();

	if (size == 0)	// Return false, if tree is empty
		return false;

	this->mList = new map<int, CityData *>[size];
	this->vertex = new CityData*[size];

	InOrderInit(root);

	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++) {
			mList[row].insert(make_pair(col, vertex[col]));
		}

	return true;
}

bool Graph::Print_GP(int size)
{
	(*flog).setf(ios::left);

	if (vertex == NULL)
		return false;

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++)
			*flog << setw(3) << GetDistance(vertex[row], vertex[col]);
		*flog << endl;
	}

	return true;
}

bool Graph::Print_MST(int size)
{
	if (size == 0)
		return false;

	int total = 0;	// sum of edges

	int* cnt = new int[size];
	int* check = new int[size];

	for (int i = 0; i < size; i++) {	// Init the array
		cnt[i] = 0; check[i] = 0;
	}

	for (int i = 0; i < size-1; i++) {	// Search start point and end point
		cnt[mstMatrix[i].second.first] += 1;
		cnt[mstMatrix[i].second.second] += 1;
	}

	int start = 0;	// Set start point
	for (int i = 0; i < size; i++) {
		if (cnt[i] == 1) {
			start = i;
			break;
		}
	}

	int end = 0;	// Set end point
	for (int i = 0; i < size; i++) {
		if (cnt[i] == 1)
			if (start < i)
				end = i;
			else if (start > i) {
				end = start;
				start = i;
			}
	}
	
	int key = start;
	int next_key = 0;
	for (int j = 0; j < size - 1; j++) {
		for (int i = 0; i < size - 1; i++) {									//n(mstMatrix) = 9
				if (key == this->mstMatrix[i].second.first && check[key] < 2 && key!=end) {
					*flog << "( " << this->vertex[this->mstMatrix[i].second.first]->Getname()
						<< ", " << this->vertex[this->mstMatrix[i].second.second]->Getname()
						<< " ), " << this->mstMatrix[i].first << endl;
					total += this->mstMatrix[i].first;
					check[this->mstMatrix[i].second.first]++; check[this->mstMatrix[i].second.second]++;
					key = this->mstMatrix[i].second.second;
				}
				else if (key == this->mstMatrix[i].second.second  && check[key] < 2 && key!=end) {
					*flog << "( " << this->vertex[this->mstMatrix[i].second.second]->Getname()
						<< ", " << this->vertex[this->mstMatrix[i].second.first]->Getname()
						<< " ), " << this->mstMatrix[i].first << endl;
					total += this->mstMatrix[i].first;
					check[this->mstMatrix[i].second.first]++; check[this->mstMatrix[i].second.second]++;
					key = this->mstMatrix[i].second.first;
				}
						
		}
	}
	*flog << "Total: " << (total) << endl;
	return true;
}

bool Graph::Kruskal(int size)
{
	if (size == 0 || size == 1) return false;

	int i = 0, j = 0, weight = 0;
	vector<pair<int, pair<int, int>>> v;
	make_set(size); // make cycle table

	// Phase 1: Sort graph edges in ascending order of weight
	for (int i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++) {
			weight = GetDistance(vertex[i], vertex[j]);
			v.push_back(make_pair(weight, make_pair(i, j)));
		}
	sort(v.begin(), v.end());

	// Phase 2: Select edges that don't form a cycle from the list in order
	// Phase 2-1: Choose a lower weight
	mstMatrix = new pair<int, pair<int, int>>[size - 1];

	int cnt = 0;
	// Phase 2-2: Add, If cycle does not occur
	for (int i = 0; i < v.size(); i++) {
		if (!check(v[i].second.first, v[i].second.second)) {
			union_set(v[i].second.first, v[i].second.second);
			mstMatrix[cnt] = v[i];
			cnt++;
		}
	}

	/*for (int i = 0; i < v.size(); i++)
		cout << v[i].first << " " << v[i].second.first << " " << v[i].second.second << endl;*/
	return true;
}

void Graph::make_set(int size)
{
	this ->set = new int[size];
	for (int i = 0; i < size; i++)
		set[i] = i;
}

void Graph::union_set(int x, int y)
{
	x = find(x);
	y = find(y);

	if(x<y) set[y] = x;
	else set[x] = y;
}

int Graph::find(int x)	// Get 
{
	if (set[x] == x) return x;
	return find(set[x]);
}

void Graph::InOrderInit(AVLNode* node)
{
	if (node != NULL)
	{
		InOrderInit(node->GetLeft());
		vertex[idx++] = node->GetCityData();
		InOrderInit(node->GetRight());
	}
}

int Graph::GetDistance(CityData * from, CityData * to)
{
	int distance = 0;
	if (from->GetLocationId() > to->GetLocationId()) distance = from->GetLocationId() - to->GetLocationId();
	else distance = to->GetLocationId() - from->GetLocationId();

	return distance;
}

int Graph::check(int a, int b) {
	a = find(a);
	b = find(b);
	if (a == b) return 1;
	else return 0;
}