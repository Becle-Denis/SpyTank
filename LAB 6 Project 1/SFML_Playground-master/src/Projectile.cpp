#include "Projectile.h"


double Projectile::s_projectile_speed = 800;

Projectile::Projectile(sf::Texture const& texture , sf::Vector2f position, double rotation) 
	: m_texture(texture), m_rotation(rotation)
{
	m_projectileSprite.setTexture(texture);
	sf::IntRect baseRect(8,175, 8, 10);
	m_projectileSprite.setTextureRect(baseRect);
	m_projectileSprite.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_projectileSprite.setRotation(rotation - 90);
	m_projectileSprite.setPosition(position);
}

Projectile::Projectile(const Projectile& p)
	: Projectile(p.m_texture,p.m_projectileSprite.getPosition(),p.m_rotation)
{

}

Projectile& Projectile::operator=(Projectile const & rhs) 
{
	Projectile p(rhs);
	return p;
}

void Projectile::update(double dt)
{
	//get new position
	float newXPosition = m_projectileSprite.getPosition().x + cos(m_rotation * MathUtility::DEG_TO_RAD) * s_projectile_speed * (dt / 1000);
	float newYPosition = m_projectileSprite.getPosition().y + sin(m_rotation * MathUtility::DEG_TO_RAD) * s_projectile_speed * (dt / 1000);

	//set new position 
	m_projectileSprite.setPosition(sf::Vector2f(newXPosition, newYPosition));
}

bool Projectile::isAlive(std::vector<sf::Sprite>& wallSprites)
{
	sf::Vector2f position = m_projectileSprite.getPosition();
	if (position.x < 0 || position.y < 0 || position.x > 1600 || position.y > 1000 )
	{
		return false;
	}
	for (sf::Sprite& wall : wallSprites)
	{
		if (CollisionDetector::collision(m_projectileSprite,wall))
		{
			return false;
		}
	}
	return true;
}

bool Projectile::hitTarget(std::vector<Target>& targets)
{
	for (unsigned int i = 0; i < targets.size(); i++)
	{
		if (targets.at(i).isColliding(m_projectileSprite))
		{
			sf::Time bonus = targets.at(i).hit();
			if (i + 1 < targets.size())
			{
				targets.at(i + 1).reveal(bonus);
			}
			return true;
		}
	}
	return false;
}

void Projectile::render(sf::RenderWindow& window)
{
	window.draw(m_projectileSprite);
}
