#include <iostream>
#include <stdlib.h>
#include "TransferWindowManager.h"
using namespace std;

int main(int argc, char** argv)
{
	// Stage 1 : Setup
	

	TransferWindowManager transfer_window_manager(argv[1], atoi(argv[2]));
	
	//TransferWindowManager transfer_window_manager("ShootForLog.txt", 2000);

	//TransferWindowManager transfer_window_manager(argv[1], atoi(argv[2]));
	
	//TransferWindowManager transfer_window_manager("ShootForLog.txt", 1000);


	// print for test vector
	//BinarySearchTree fwbst = transfer_window_manager.getfwBST();
	//fwbst.print_v();
	//BinarySearchTree mfbst = transfer_window_manager.getmfBST();
	//mfbst.print_v();
	//BinarySearchTree dfbst = transfer_window_manager.getdfBST();
	//dfbst.print_v();
	//BinarySearchTree gkbst = transfer_window_manager.getgkBST();
	//gkbst.print_v();


	// Stage 2 : Print Players
	//cout << transfer_window_manager << endl;

	
	// Stage 3 : Search the Best Team & Delete Players of Best team from the 4BST
	TransferWindowManager::SoccerTeam team = transfer_window_manager.getBestTeam();
	cout << "Best Players" << endl;
	cout << team << endl;
	cout << "-----" << endl;
	cout << "The Transfer window close" << endl;
	cout << transfer_window_manager << endl;
	
    return 0;
}

