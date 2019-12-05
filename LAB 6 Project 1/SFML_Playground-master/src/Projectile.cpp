#include "Projectile.h"


double Projectile::s_projectile_speed = 800;

void Projectile::launch(sf::Vector2f position, double rotation)
{
	m_rotation = rotation;
	m_projectileSprite.setRotation(rotation - 90);
	m_projectileSprite.setPosition(position);
}

void Projectile::setSprite(sf::Texture const* texturePtr)
{
	m_texturePtr = texturePtr;
	m_projectileSprite.setTexture(*texturePtr);
	sf::IntRect baseRect(8, 175, 8, 10);
	m_projectileSprite.setTextureRect(baseRect);
	m_projectileSprite.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	setInactive();
}

sf::Sprite const& Projectile::getSprite() const
{
	return m_projectileSprite;
}

void Projectile::setInactive()
{
	m_projectileSprite.setPosition(sf::Vector2f(-100, -100));
}

bool Projectile::isInactive() const
{
	return m_projectileSprite.getPosition() == sf::Vector2f(-100,-100);
}

void Projectile::update(double dt)
{
	//get new position
	float newXPosition = m_projectileSprite.getPosition().x + cos(m_rotation * MathUtility::DEG_TO_RAD) * s_projectile_speed * (dt / 1000);
	float newYPosition = m_projectileSprite.getPosition().y + sin(m_rotation * MathUtility::DEG_TO_RAD) * s_projectile_speed * (dt / 1000);

	//set new position 
	m_projectileSprite.setPosition(sf::Vector2f(newXPosition, newYPosition));
}

int Projectile::lifeState(std::vector<sf::Sprite>& wallSprites) const
{
	sf::Vector2f position = m_projectileSprite.getPosition();
	//check for the limit of the screen 
	if (position.x < 0 || position.y < 0 || position.x > 1600 || position.y > 1000 )
	{
		return 2;
	}
	//check for collision with wall
	for (sf::Sprite& wall : wallSprites)
	{
		if (CollisionDetector::collision(m_projectileSprite,wall))
		{
			return 1;
		}
	}
	return -1;
}


void Projectile::render(sf::RenderWindow& window) const
{
	window.draw(m_projectileSprite);
}
