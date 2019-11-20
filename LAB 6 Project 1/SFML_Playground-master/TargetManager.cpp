#include "TargetManager.h"

TargetManager::TargetManager(sf::Texture const& targetTexture) :
	m_number_of_targets(0),
	m_texture(targetTexture),
	m_targets(nullptr)
{

}

TargetManager::~TargetManager()
{
	delete[] m_targets;
}

void TargetManager::construct(std::vector<TargetData>& targetData)
{
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

int TargetManager::checkForCollision(sf::Sprite& const sprite)
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
		(m_targets + index)->hit();
	}
}

void TargetManager::render(sf::RenderWindow& window)
{
	for (int i = 0; i < m_number_of_targets; i++)
	{
		(m_targets + i)->render(window);
	}
}

bool TargetManager::isOneOnScreen()
{
	return false;
}

int TargetManager::getNextIndex()
{
	return 0;
}
