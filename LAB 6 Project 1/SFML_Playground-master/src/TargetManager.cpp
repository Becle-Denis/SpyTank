#include "TargetManager.h"

TargetManager::TargetManager(sf::Texture const& targetTexture) :
	m_number_of_targets(0),
	m_texture(targetTexture),
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

void TargetManager::construct(std::vector<TargetData>& targetData)
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
		(m_targets + i)->init(&m_texture, targetData.at(i).m_position, sf::seconds(targetData.at(i).secondsToStart), sf::seconds(targetData.at(i).secondsOnScreen));
	}
}

void TargetManager::update(double dt)
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->update();
	}
}

int TargetManager::checkForCollision(sf::Sprite const& sprite)
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

void TargetManager::render(sf::RenderWindow& window)
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

bool TargetManager::isOneOnScreen()
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

int TargetManager::getNextIndex()
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
