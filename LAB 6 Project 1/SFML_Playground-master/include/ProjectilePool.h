#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"

class ProjectilePool
{
	sf::Texture const& m_texture;
	Projectile* m_projectiles;
	unsigned int const m_numberOfProjectile;
	unsigned int m_projectileIndex;

public:
	ProjectilePool(sf::Texture const& texture,unsigned int poolSize = 15);
	~ProjectilePool();
	Projectile* getProjectile();
};

