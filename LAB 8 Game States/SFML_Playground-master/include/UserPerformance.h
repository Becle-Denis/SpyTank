#pragma once
#include <iostream>
#include <string>
#include "TargetManager.h"
#include "LevelLoader.h"

class UserPerformance
{
private: // Attribute
	int m_totalNumberOfTarget{ 0 }; 
	int m_targetHitted{ 0 };
	int m_sucess{ 0 };
	int m_projectileFired{ 0 };
	int m_accuracy{ 0 };
	TargetManager* m_targetManagerPtr; //for accessing real time statistique 

public: // functions 
	UserPerformance() = default;
	UserPerformance(int totalNumberOfTarget, int t_targethitted = 0, int t_projectileFired = 0); 
	UserPerformance(TargetManager* targetManagerPtr);

	void projectileFired();
	void targetHitted();
	std::string toStringColumn();
	std::string toStringColumnFull();

	bool saveOnFile(std::string fileName);

	static UserPerformance loadFromFile(std::string fileName);

	friend bool operator<(UserPerformance a, UserPerformance b);

private: // function 
	void calculate();


};

