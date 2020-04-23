#include "ProjectilePool.h"

ProjectilePool::ProjectilePool(unsigned int poolSize, SoundManager& soundManager) :
	m_projectileIndex(0)
{
	m_projectiles.reserve(poolSize);
	for (int i = 0; i < poolSize; i++)
	{
		m_projectiles.push_back(std::make_shared<Projectile>(soundManager));
	}
}

ProjectilePool::~ProjectilePool()
{

}

std::shared_ptr<Projectile> ProjectilePool::getProjectile()
{
	//first loop index to the end
	for (int i = m_projectileIndex; i < m_projectiles.size(); i++)
	{
		if (m_projectiles.at(i)->isInactive())
		{
			if (i == m_projectiles.size() - 1)
			{
				m_projectileIndex = 0;
			}
			else
			{
				m_projectileIndex = i + 1;
			}
			return m_projectiles.at(i);
		}
	}

	//second loop, 0 to index 
	for (int i = 0; i < m_projectileIndex; i++)
	{
		if (m_projectiles.at(i)->isInactive())
		{
			if (i == m_projectiles.size() - 1)
			{
				m_projectileIndex = 0;
			}
			else
			{
				m_projectileIndex = i + 1;
			}
			return m_projectiles.at(i);
		}
	}
	// if all projectiles are active, return nullptr;
	return nullptr;
}

void ProjectilePool::setLightMode(LightMode mode)
{
	for (std::shared_ptr<Projectile> projectile : m_projectiles)
	{
		projectile->setLightMode(mode);
	}
}
