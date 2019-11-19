#include "ProjectilePool.h"

ProjectilePool::ProjectilePool(sf::Texture const& texture, unsigned int poolSize) :
	m_texture(texture), m_numberOfProjectile(poolSize), m_projectileIndex(0)
{
	m_projectiles = new Projectile[m_numberOfProjectile];
	for (int i = 0; i < m_numberOfProjectile; i++)
	{
		(m_projectiles + i)->setSprite(&m_texture);
	}
}

ProjectilePool::~ProjectilePool()
{
	delete[] m_projectiles;
}

Projectile* ProjectilePool::getProjectile()
{
	for (unsigned int i = 0; i < m_numberOfProjectile; i++)
	{
		/*
		if (i == m_projectileIndex)
		{
			i = 0;
		}
		*/
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
	return nullptr;
}
