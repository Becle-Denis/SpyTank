#include "UserPerformance.h"

UserPerformance::UserPerformance(int totalNumberOfTarget, int t_targethitted, int t_projectileFired) :
	m_totalNumberOfTarget(totalNumberOfTarget),m_targetHitted(t_targethitted),m_projectileFired(t_projectileFired),
	m_targetManagerPtr(nullptr)
{
}

UserPerformance::UserPerformance(TargetManager* targetManagerPtr)
	: m_targetManagerPtr(targetManagerPtr)
{
}

void UserPerformance::projectileFired()
{
	m_projectileFired++;
}

void UserPerformance::targetHitted()
{
	m_targetHitted++;
}

std::string UserPerformance::toStringColumn()
{
	calculate();
	return std::to_string(m_targetHitted)
		+ "\n" + std::to_string(m_sucess) + " %" + "\n"
		+ std::to_string(m_accuracy) + " %";
}

std::string UserPerformance::toStringColumnFull()
{
	calculate();
	return std::to_string(m_targetHitted) + "\n"
		+ std::to_string(m_totalNumberOfTarget) + "\n"
		+ std::to_string(m_sucess) + " %" + "\n"
		+ std::to_string(m_projectileFired) + "\n"
		+ std::to_string(m_accuracy) + " %";
}

void UserPerformance::calculate()
{
	if (m_targetManagerPtr != nullptr)
	{
		m_totalNumberOfTarget = m_targetManagerPtr->getNumberOfDisplayedTarget();
	}
	if (m_totalNumberOfTarget != 0)
	{
		m_sucess = static_cast<int>((static_cast<double>(m_targetHitted) / static_cast<double>(m_totalNumberOfTarget)) * 100);
	}
	if (m_projectileFired != 0)
	{
		m_accuracy = static_cast<int>((static_cast<double>(m_targetHitted) / static_cast<double>(m_projectileFired)) * 100);
	}
}

