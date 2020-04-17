#include "ProjectilePool.h"

ProjectilePool::ProjectilePool(unsigned int poolSize, SoundManager* soundManager) :
	m_numberOfProjectile(poolSize), m_projectileIndex(0)
{
	m_projectiles = new Projectile[m_numberOfProjectile];
	for (int i = 0; i < m_numberOfProjectile; i++)
	{
		(m_projectiles + i)->setProjectile(soundManager);
	}
}

ProjectilePool::~ProjectilePool()
{
	delete[] m_projectiles;
}

Projectile* ProjectilePool::getProjectile()
{
	//first loop index to the end
	for (int i = m_projectileIndex; i < m_numberOfProjectile; i++)
	{
		if ((m_projectiles + i)->isInactive())
		{
			if (i == m_numberOfProjectile - 1)
			{
				m_projectileIndex = 0;
			}
			else
			{
				m_projectileIndex = i + 1;
			}
			return m_projectiles + i;
		}
	}

	//second loop, 0 to index 
	for (int i = 0; i < m_projectileIndex; i++)
	{
		if ((m_projectiles + i)->isInactive())
		{
			if (i == m_numberOfProjectile - 1)
			{
				m_projectileIndex = 0;
			}
			else
			{
				m_projectileIndex = i + 1;
			}
			return m_projectiles + i;
		}
	}
	// if all projectiles are active, return nullptr;
	return nullptr;
}

void ProjectilePool::setLightMode(LightMode mode)
{
	for (int i = 0; i < m_numberOfProjectile; i++)
	{
		(m_projectiles + i)->setLightMode(mode);
	}
}
