#pragma once
#include <string>
#include "TargetManager.h"
class UserPerformance
{
private: // Attribute
	int m_totalNumberOfTarget{ 0 };
	int m_targetHitted{ 0 };
	int m_sucess{ 0 };
	int m_projectileFired{ 0 };
	int m_accuracy{ 0 };
	TargetManager* m_targetManagerPtr;

public: // functions 
	UserPerformance() = default;
	UserPerformance(int totalNumberOfTarget, int t_targethitted = 0, int t_projectileFired = 0);
	UserPerformance(TargetManager* targetManagerPtr);

	void projectileFired();
	void targetHitted();
	std::string toStringColumn();
	std::string toStringColumnFull();

private: // function 
	void calculate();
};

