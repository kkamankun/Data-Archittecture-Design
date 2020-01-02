#include "TransferWindowManager.h"
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
using namespace std;

TransferWindowManager::SoccerTeam::SoccerTeam()
{
	//You don't need to edit this function.
}

TransferWindowManager::SoccerTeam::SoccerTeam(SoccerPlayerData fw, SoccerPlayerData mf, SoccerPlayerData df, SoccerPlayerData gk)
{
	this->fw = fw;
	this->mf = mf;
	this->df = df;
	this->gk = gk;

	this->sum_transfer_fee = fw.m_transfer_fee + mf.m_transfer_fee + df.m_transfer_fee + gk.m_transfer_fee;
	this->sum_ability = fw.m_ability + mf.m_ability + df.m_ability + gk.m_ability;
}

std::ostream& operator<<(std::ostream& os, const TransferWindowManager::SoccerTeam& team)
{
	os << team.fw ;
	os << team.mf ;
	os << team.df ;
	os << team.gk ;

	os << "sum_transfer_fee " << team.sum_transfer_fee << std::endl;
	os << "sum_ability " << team.sum_ability << std::endl;

	return os;
}


TransferWindowManager::TransferWindowManager(std::string file_dir, int budget)
{
	m_budget = budget;

	std::ifstream in(file_dir);
	std::string line;
	char* cStr = NULL;
	vector<string> v;
	

	while (std::getline(in, line))
	{
		// txt 한 줄씩 읽어와서 자르기 후 vector container에 저장하기
		cStr = new char[(line.size()*sizeof(char))+1];
		
		strcpy(cStr, line.c_str());
		char* tok = strtok(cStr, ",");
	
		while (tok != NULL)
		{
			// 선수 이름
			//cout << tok << endl;
			v.push_back(tok);
			// 포지션
			tok = strtok(NULL, ",");
			tok += 1;
			//cout << tok << endl;
			v.push_back(tok);
			// 요금
			tok = strtok(NULL, ",");
			tok += 1;
			//cout << tok << endl;
			v.push_back(tok);
			// 능력
			tok = strtok(NULL, ",");
			tok += 1;
			//cout << tok << endl;
			v.push_back(tok);

			tok = strtok(NULL, ",");
		}
		delete[] cStr;
		cStr = NULL;
	}

	// Vector container의 문자들을 SoccerPlayerData class화하여 4BST 만들기
	for (int i = 0; i < v.size(); i++)
	{
		if (i % 4 == 1)
		{
			if (v[i] == "Forward")
			{
				SoccerPlayerData data(v[i - 1], v[i], stoi(v[i + 1]), stoi(v[i + 2]));
				fwBST.insert(data);
			}
			else if (v[i] == "Midfielder")
			{
				SoccerPlayerData data(v[i - 1], v[i], stoi(v[i + 1]), stoi(v[i + 2]));
				mfBST.insert(data);
			}
			else if (v[i] == "Defender")
			{
				SoccerPlayerData data(v[i - 1], v[i], stoi(v[i + 1]), stoi(v[i + 2]));
				dfBST.insert(data);
			}
			else if (v[i] == "Goalkeeper")
			{
				SoccerPlayerData data(v[i - 1], v[i], stoi(v[i + 1]), stoi(v[i + 2]));
				gkBST.insert(data);
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const TransferWindowManager& manager)
{
	os << "********Forward List********" << std::endl;
	os << manager.fwBST;
	os << "****************************" << std::endl;

	os << "********Midflder List********" << std::endl;
	os << manager.mfBST;
	os << "*****************************" << std::endl;

	os << "********Defender List********" << std::endl;
	os << manager.dfBST;
	os << "*****************************" << std::endl;

	os << "********Goalkeeper List********" << std::endl;
	os << manager.gkBST;
	os << "*******************************" << std::endl;

	return os;
}

BinarySearchTree TransferWindowManager::getfwBST() {
	return fwBST;
}

BinarySearchTree TransferWindowManager::getmfBST()
{
	return mfBST;
}
BinarySearchTree TransferWindowManager::getdfBST()
{
	return dfBST;
}
BinarySearchTree TransferWindowManager::getgkBST()
{
	return gkBST;
}

TransferWindowManager::SoccerTeam TransferWindowManager::getBestTeam()
{
	SoccerTeam best_team;
	int sum_transfer_fee;
	int sum_ability;
	SoccerTeam temp;
	temp.fw = fwBST.v[0];
	temp.mf = mfBST.v[0];
	temp.df = dfBST.v[0];
	temp.gk = gkBST.v[0]; // Jeong Sung-Ryong
        temp.sum_transfer_fee=0;
        temp.sum_ability=0;
	for (int i = 0; i < fwBST.v.size(); i++)
	{
		for (int j = 0; j < mfBST.v.size(); j++)
		{
			for (int k = 0; k < dfBST.v.size(); k++)
			{
				sum_transfer_fee = fwBST.v[i].m_transfer_fee + mfBST.v[j].m_transfer_fee + dfBST.v[k].m_transfer_fee + gkBST.v[0].m_transfer_fee;
				sum_ability = fwBST.v[i].m_ability + mfBST.v[j].m_ability + dfBST.v[k].m_ability+gkBST.v[0].m_ability;

				if (sum_transfer_fee <= m_budget)
				{
					if (sum_ability > temp.sum_ability) 
					{
						temp.fw = fwBST.v[i];
						temp.mf = mfBST.v[j];
						temp.df = dfBST.v[k];
						temp.sum_ability = sum_ability;
						temp.sum_transfer_fee = sum_transfer_fee;
					}
					else if (sum_ability == temp.sum_ability)
					{
						if (sum_transfer_fee < temp.sum_transfer_fee)
						{
							temp.fw = fwBST.v[i];
							temp.mf = mfBST.v[j];
							temp.df = dfBST.v[k];
							temp.sum_ability = sum_ability;
							temp.sum_transfer_fee = sum_transfer_fee;
						}
						else if (sum_transfer_fee == temp.sum_transfer_fee)
						{
							continue;
						}
					}
				}
			}
		}
	}

	best_team.fw = temp.fw;
	best_team.mf = temp.mf;
	best_team.df = temp.df;
	best_team.gk = temp.gk;
	best_team.sum_ability = temp.sum_ability;
	best_team.sum_transfer_fee = temp.sum_transfer_fee;

	// Delete Players of Best team from the fwBST, mfBST, dfBST, and gkBST
	fwBST.deletion(best_team.fw.m_ability);
	mfBST.deletion(best_team.mf.m_ability);
	dfBST.deletion(best_team.df.m_ability);
	gkBST.deletion(best_team.gk.m_ability);

	return best_team;
}

