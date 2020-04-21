#include "TargetManager.h"

TargetManager::TargetManager() :
	m_number_of_targets(0),
	m_targets(nullptr),
	m_numberMinOfTargets(0),
	m_numberofActiveTargets(0)
{

}

TargetManager::~TargetManager()
{
	if (m_targets != nullptr)
	{
		delete[] m_targets;
	}
}

void TargetManager::construct(std::vector<TargetData>& targetData, int minNumberOfTarget, SoundManager* soundManager)
{
	//deletion if reconstruct 
	if (m_targets != nullptr)
	{
		delete[] m_targets; 
	}

	//settings valuess
	m_number_of_targets = targetData.size();
	m_numberMinOfTargets = m_number_of_targets;
	m_numberofActiveTargets = m_number_of_targets;
	if (minNumberOfTarget < m_number_of_targets)
	{
		m_numberMinOfTargets = minNumberOfTarget;
	}

	//constructing targets 
	m_targets = new Target[m_number_of_targets];
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->init(targetData.at(i).m_position, sf::seconds(targetData.at(i).secondsToStart), sf::seconds(targetData.at(i).secondsOnScreen), soundManager);
	}
}

void TargetManager::start(bool timed, sf::Texture const& targetTexture, bool easyMode)
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->start(targetTexture, timed);
	}
	if (!timed)
	{
		int nextIndex = getNextIndex();
		(m_targets + nextIndex)->reveal();
	}

	if (easyMode)
	{
		m_numberofActiveTargets = m_numberMinOfTargets;
	}
	else
	{
		m_numberofActiveTargets = m_number_of_targets;
	}
}

void TargetManager::update(double dt)
{
	for (int i = 0; i < m_numberofActiveTargets; i++)
	{
		(m_targets + i)->update();
	}
}

int TargetManager::checkForCollision(sf::Sprite const& sprite) const
{
	for (int i = 0; i < m_numberofActiveTargets; i++)
	{
		if ((m_targets + i)->isColliding(sprite))
		{
			return i;
		}
	}
	return -1;
}

void TargetManager::capture(int index, bool hitted)
{
	if (index < m_numberofActiveTargets)
	{
		sf::Time bonusTime = (m_targets + index)->capture(hitted); // hit the actual target 
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
	for (int i = 0; i < m_numberofActiveTargets; i++)
	{
		(m_targets + i)->render(window);
	}
}

void TargetManager::revealResult()
{
	for (int i = 0; i < m_numberofActiveTargets; i++)
	{
		(m_targets + i)->setResult();
	}
}

int TargetManager::getNumberOfActiveTarget() const
{
	return m_numberofActiveTargets;
}

bool TargetManager::isOneOnScreen() const
{
	for (int i = 0; i < m_numberofActiveTargets; i++)
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
	for (int i = 0; i < m_numberofActiveTargets; i++)
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
