#include "TargetManager.h"

TargetManager::TargetManager() :
	m_number_of_targets(0),
	m_targets(nullptr)
{

}

TargetManager::~TargetManager()
{
	if (m_targets != nullptr)
	{
		delete[] m_targets;
	}
}

void TargetManager::construct(std::vector<TargetData>& targetData, SoundManager* soundManager)
{
	//deletion if reconstruct 
	if (m_targets != nullptr)
	{
		delete[] m_targets; 
	}
	m_number_of_targets = targetData.size();
	m_targets = new Target[m_number_of_targets];
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->init(targetData.at(i).m_position, sf::seconds(targetData.at(i).secondsToStart), sf::seconds(targetData.at(i).secondsOnScreen), soundManager);
	}
}

void TargetManager::start(bool timed)
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->start(timed);
	}
	if (!timed)
	{
		int nextIndex = getNextIndex();
		(m_targets + nextIndex)->reveal();
	}
}

void TargetManager::update(double dt)
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->update();
	}
}

int TargetManager::checkForCollision(sf::Sprite const& sprite) const
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		if ((m_targets + i)->isColliding(sprite))
		{
			return i;
		}
	}
	return -1;
}

void TargetManager::hit(int index)
{
	if (index < m_number_of_targets)
	{
		sf::Time bonusTime = (m_targets + index)->hit(); // hit the actual target 
		//limiting the bonus time 
		if (bonusTime > sf::seconds(4.f))
		{
			bonusTime = sf::seconds(4.f);
		}
		if (!isOneOnScreen())
		{
			int nextIndex = getNextIndex();
			if (nextIndex != -1)
			{
				(m_targets + nextIndex)->reveal(bonusTime); // reveal the next one 
			}
		}
	}
}

void TargetManager::render(sf::RenderWindow& window) const
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->render(window);
	}
}

void TargetManager::revealResult()
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->setResult();
	}
}

int TargetManager::getNumberOfDisplayedTarget() const
{
	return m_number_of_targets;
}

bool TargetManager::isOneOnScreen() const
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		if ((m_targets + i)->isOnScreen())
		{
			return true;
		}
	}
	return false;
}

int TargetManager::getNextIndex() const
{
	int minIndex = -1;
	sf::Time minTime = sf::seconds(100); // not really good but it works
	sf::Time targetReaminingTime;
	for (int i = 0; i < m_number_of_targets; i++)
	{
		if ((m_targets + i)->waintingToBeDisplayed(targetReaminingTime))
		{
			if (targetReaminingTime < minTime)
			{
				minTime = targetReaminingTime;
				minIndex = i;
			}
		}
	}
	return minIndex;
}
