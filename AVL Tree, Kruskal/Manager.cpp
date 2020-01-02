#include "Manager.h"
#include <stdlib.h>
#include <fstream>
#include <string>
using namespace std;

Manager::Manager()
{
	flog.open("log.txt", ios::app);  // Option app: write new content into an existing file.
	//flog.open("log.txt", ios::trunc);
	flog.setf(ios::fixed);

	avl = new AVLTree(&flog);
	gp = new Graph(&flog);
	iter = 0;
	cmd = NULL;
}

Manager::~Manager()
{
	flog.close();
}

void Manager::run(const char * command)  // "command.txt"
{
	fin.open(command);

	cmd = new char[40];

	while (!fin.eof())
	{
		iter = iter + 1;
		fin.getline(cmd, 40);
		char * one = strtok(cmd, " ");
		char * two = 0;

		// LOAD
		if (!strcmp(one, "LOAD")) {
			one = strtok(NULL, " ");
			if (one == NULL) {
				if (LOAD()) printSuccessCode("LOAD");
				else printErrorCode(100, "LOAD");	// Print error code, if txt file opening fails or tree already exists
			}
			else
				printErrorCode(100, "LOAD");	// Print error code, if nonnecessary parameters exist
		}

		// INSERT
		else if (!strcmp(one, "INSERT")) {
			char *arr[3] = { NULL, };	// Save parameters(LocationID, name, country)
			one = strtok(NULL, " "); arr[0] = one;
			one = strtok(NULL, " "); arr[1] = one;
			one = strtok(NULL, " "); arr[2] = one;

			if (INSERT(atoi(arr[0]), arr[1], arr[2])) printInfo("INSERT", atoi(arr[0]), arr[1], arr[2]);
			else printErrorCode(200, "INSERT");

		}

		// PRINT_AVL
		else if (!strcmp(one, "PRINT_AVL")) {
			one = strtok(NULL, "\t"); 
			if (!one) { 
				if (PRINT_AVL()) {
					printCommand("PRINT_AVL");
					avl->InOrder(avl->Getroot());
				}
				else printErrorCode(300, "PRINT_AVL");
			}
			else
				printErrorCode(300, "PRINT_AVL"); // Print error code, if nonnecessary parameters exist
		}

		// SEARCH_AVL
		else if (!strcmp(one, "SEARCH_AVL")) {
			one = strtok(NULL, " "); 
			two = strtok(NULL, " ");
			if (!two) {
				if (!one) printErrorCode(500, "SEARCH_AVL"); // Print error code, if input data does not exist
				else {
					if (SEARCH_AVL(atoi(one))) { ; }
					else printErrorCode(500, "SEARCH_AVL");
				}
			}
			else printErrorCode(500, "SEARCH_AVL"); // Print error code, if nonnecessary parameters exist
			
		}

		// DELETE_AVL
		else if (!strcmp(one, "DELETE_AVL")) {
			one = strtok(NULL, " "); 
			two = strtok(NULL, " ");
			if (!two) {
				if (!one) printErrorCode(400, "SEARCH_AVL"); // Print error code, if input data does not exist
				else if (DELETE_AVL(atoi(one))) { ; }
				else printErrorCode(400, "DELETE_AVL");
			}
			else printErrorCode(400, "DELETE_AVL"); // Print error code, if nonnecessary parameters exist
		}

		// BUILD_GP
		else if (!strcmp(one, "BUILD_GP")) {
			one = strtok(NULL, " ");
			if (!one) {
				if (BUILD_GP()) printSuccessCode("BUILD_GP");
				else printErrorCode(600, "BUILD_GP");
			}
			else
				printErrorCode(600, "BUILD_GP"); // Print error code, if nonnecessary parameters exist
		}

		// PRINT_GP
		else if (!strcmp(one, "PRINT_GP")) {
			one = strtok(NULL, " ");
			if (!one) {
				printCommand("PRINT_GP");
				if (PRINT_GP()) { ; }
				else flog << "Error code: " << 700 << endl;
			}
			else
				printErrorCode(700, "PRINT_GP"); // Print error code, if nonnecessary parameters exist
		}

		// BUILD_MST
		else if (!strcmp(one, "BUILD_MST")) {
			one = strtok(NULL, " ");
			if (!one) {
				if (BUILD_MST()) printSuccessCode("BUILD_MST");
				else printErrorCode(800, "BUILD_MST");
			}
			else 
				printErrorCode(800, "BUILD_MST"); // Print error code, if nonnecessary parameters exist
		}

		// PRINT_MST
		else if (!strcmp(one, "PRINT_MST")) {
			one = strtok(NULL, " ");
			if (!one) {
				printCommand("PRINT_MST");
				if (PRINT_MST()) { ; }
				else flog << "Error code: " << 900 << endl;
			}
			else
				printErrorCode(900, "PRINT_MST"); // Print error code, if nonnecessary parameters exist
		}

		// EXIT
		else if (!strcmp(one, "EXIT")) {
			printSuccessCode("EXIT");
			EXIT();	
		}
		else {
			printErrorCode(0,"Unknown");
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD()
{
	ifstream fin_0("city_list.txt");	
	string line;

	while (getline(fin_0, line)) {
		CityData * data = new CityData;	
		char cStr[100];
		strcpy(cStr, line.c_str());	

		// LocationId
		char * tok = strtok(cStr, "\t");	
		data->SetLocationId(atoi(tok));

		// city name
		tok = strtok(NULL, "\t");	
		data->Setname(tok);	

		// country name
		tok = strtok(NULL, "\n");	
		data->Setcountry(tok);	

		if (avl->flag_Tree != 0) return false; // Print error code, if tree already exists
		else avl->Insert(data);	
	}
	avl->flag_Tree = 1; // Tree configuration complete
	
	return true;
}

bool Manager::INSERT(int LocationID, char* name, char* country)
{
	if (!avl->Getroot()) // print error code, if tree does not exist
		return false;
	else
	{
		CityData* data = new CityData;
		data->SetLocationId(LocationID);
		data->Setname(name);
		data->Setcountry(country);

		avl->Insert(data);

		return true;
	}
}

bool Manager::PRINT_AVL()
{
	return avl->Print();
}

bool Manager::SEARCH_AVL(int LocationID)
{
	CityData* data = new CityData;
	data = avl->Search(LocationID);

	if (data != NULL) {
		printCommand("SEARCH_AVL");
		flog << "( " << (data->GetLocationId()) << ", " << data->Getname() << ", " << data->Getcountry() << " )" << endl;
		return true;
	}
	else
		return false;
}

bool Manager::DELETE_AVL(int LocationID)
{
	if (avl->Delete(LocationID)) {
		printSuccessCode("DELETE_AVL");
		return true;
	}
	else
		return false;
}

bool Manager::BUILD_GP()
{
	avl->SetSize(0);
	avl->PreOrder(avl->Getroot()); // Update AVLTree size
	if(gp->Build(avl, avl->GetSize()))	return true;
	else return false;
	
}

bool Manager::PRINT_GP()
{
	if (gp->Print_GP(avl->GetSize())) {
		return true;
	}
	else return false;
}

bool Manager::BUILD_MST()
{
	if(gp->Kruskal(avl->GetSize()))	return true;
	else return false;
}

bool Manager::PRINT_MST()
{	
	if(gp->Print_MST(avl->GetSize()))	return true;
	else return false;
}

void Manager::EXIT()
{
	exit(1);
}

void Manager::printErrorCode(int n, const char * cmdname) {// ERROR CODE PRINT FUNCTION
	flog << "==> command " << iter << ") " << cmdname << endl;
	flog << "Error code: " << n << endl;
}

void Manager::printSuccessCode(const char * cmdname) {// SUCCESS CODE PRINT FUNCTION
	flog << "==> command " << iter << ") " << cmdname << endl;
	flog << "Success" << endl;
}

void Manager::printInfo(const char * cmdname, int locationID, char *name, char *country) { // INFORMATION OF CITIES PRINT FUNCTION
	flog << "==> command " << iter << ") " << cmdname << endl;
	flog << "( " << (locationID) << ", " << (name) << ", " << (country) << " )" << endl;
}

void Manager::printCommand(const char * cmdname) { // COMMAND PRINT FUNCTION
	flog << "==> command " << iter << ") " << cmdname << endl;
}


