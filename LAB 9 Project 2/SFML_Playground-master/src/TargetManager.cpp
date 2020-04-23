#include "TargetManager.h"

TargetManager::TargetManager() :
	m_numberMinOfTargets(0),
	m_numberofActiveTargets(0)
{

}

TargetManager::~TargetManager()
{

}

void TargetManager::construct(std::vector<TargetData>& targetData, int minNumberOfTarget, SoundManager* soundManager)
{
	//deletion if reconstruct 
	m_targets.clear();

	
	//constructing tagets 
	//settings values
	m_targets.resize(targetData.size());
	m_numberMinOfTargets = m_targets.size();
	m_numberofActiveTargets = m_targets.size();
	if (minNumberOfTarget < m_targets.size())
	{
		m_numberMinOfTargets = minNumberOfTarget;
	}

	//targets initialisation  
	for (int i = 0; i < m_targets.size(); i++)
	{
		m_targets.at(i).init(targetData.at(i).m_position, sf::seconds(targetData.at(i).secondsToStart), sf::seconds(targetData.at(i).secondsOnScreen), soundManager);
	}
}

void TargetManager::start(bool timed, sf::Texture const& targetTexture, bool easyMode)
{
	for (int i = 0; i < m_targets.size(); i++)
	{
		m_targets.at(i).start(targetTexture, timed);
	}
	if (!timed)
	{
		int nextIndex = getNextIndex();
		m_targets.at(nextIndex).reveal();
	}

	if (easyMode)
	{
		m_numberofActiveTargets = m_numberMinOfTargets;
	}
	else
	{
		m_numberofActiveTargets = m_targets.size();
	}
}

void TargetManager::update(double dt)
{
	for (int i = 0; i < m_numberofActiveTargets; i++)
	{
		m_targets.at(i).update();
	}
}

int TargetManager::checkForCollision(sf::Sprite const& sprite) const
{
	for (int i = 0; i < m_numberofActiveTargets; i++)
	{
		if (m_targets.at(i).isColliding(sprite))
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
		sf::Time bonusTime = m_targets.at(index).capture(hitted); // hit the actual target 
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
				m_targets.at(nextIndex).reveal(bonusTime); // reveal the next one 
			}
		}
	}
}

void TargetManager::render(sf::RenderWindow& window) const
{
	for (int i = 0; i < m_numberofActiveTargets; i++)
	{
		m_targets.at(i).render(window);
	}
}

void TargetManager::revealResult()
{
	for (int i = 0; i < m_numberofActiveTargets; i++)
	{
		m_targets.at(i).setResult();
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
		if (m_targets.at(i).isOnScreen())
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
		if (m_targets.at(i).waintingToBeDisplayed(targetReaminingTime))
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
