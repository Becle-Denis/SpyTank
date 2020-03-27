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

bool UserPerformance::saveOnFile(std::string fileName)
{
	calculate();
	YAML::Node bestNode;
	bestNode["best"]["totalTarget"] = m_totalNumberOfTarget;
	bestNode["best"]["targetHitted"] = m_targetHitted;
	bestNode["best"]["projectileFired"] = m_projectileFired;
	std::ofstream fout(fileName);
	fout << bestNode;
	return true;
}

UserPerformance UserPerformance::loadFromFile(std::string filePath)
{
	UserPerformance perf;
	try
	{
		YAML::Node baseNode = YAML::LoadFile(filePath);
		if (baseNode.IsNull())
		{
			std::string message("Loading Scores : File: " + filePath + " not found");
			throw std::exception(message.c_str());
		}
		// do magic here
		int totalTargets = baseNode["best"]["totalTarget"].as<int>();
		int hitted = baseNode["best"]["targetHitted"].as<int>();
		int fired = baseNode["best"]["projectileFired"].as<int>();
		perf = UserPerformance(totalTargets,hitted,fired);
	}
	catch (YAML::ParserException& e)
	{
		std::string message(e.what());
		message = "Loading Scores :  YAML Parser Error: " + message;
		std::cout << message << std::endl;
	}
	catch (std::exception& e)
	{
		std::string message(e.what());
		message = "Loading Scores : Unexpected Error: " + message;
		std::cout << message << std::endl;
	}
	return perf;
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

bool operator<(UserPerformance a, UserPerformance b)
{
	a.calculate();
	b.calculate();
	if (a.m_sucess < b.m_sucess)
	{
		return true;
	}
	if (a.m_sucess == b.m_sucess)
	{
		if (a.m_accuracy < b.m_accuracy)
		{
			return true;
		}
	}
	return false;
}
