#include "Graph.h"
#include<utility>
#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;

class Manager
{
public:
	AVLTree * avl;
	Graph * gp;

	char * cmd;
	int iter;

public:
	Manager();
	~Manager();

	ofstream flog;
	ifstream fin; // command.txt

	void run(const char * command);
	bool LOAD();
	bool INSERT(int, char*, char*);
	bool PRINT_AVL();
	bool SEARCH_AVL(int);
	bool DELETE_AVL(int);
	bool BUILD_GP();
	bool PRINT_GP();
	bool BUILD_MST();
	bool PRINT_MST();
	void EXIT();

	void printErrorCode(int n, const char * cmdname);
	void printSuccessCode(const char * cmdname);
	void printInfo(const char * cmdname, int locationID, char *name, char *country);
	void printCommand(const char * cmdname);

};
