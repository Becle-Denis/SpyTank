#include "Projectile.h"

double Projectile::s_projectile_speed = 1000;

Projectile::Projectile(sf::Texture& texture , sf::Vector2f position, double rotation) 
	: m_texture(texture), m_rotation(rotation)
{
	m_projectileSprite.setTexture(texture);
	sf::IntRect baseRect(8,175, 8, 10);
	m_projectileSprite.setTextureRect(baseRect);
	m_projectileSprite.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_projectileSprite.setRotation(-90);
	m_projectileSprite.setPosition(position);
}

void Projectile::update(double dt)
{
	//get new position
	float newXPosition = m_projectileSprite.getPosition().x + cos(m_rotation * MathUtility::DEG_TO_RAD) * s_projectile_speed * (dt / 1000);
	float newYPosition = m_projectileSprite.getPosition().y + sin(m_rotation * MathUtility::DEG_TO_RAD) * s_projectile_speed * (dt / 1000);

	//set new position 
	m_projectileSprite.setPosition(sf::Vector2f(newXPosition, newYPosition));
}

void Projectile::render(sf::RenderWindow& window)
{
	window.draw(m_projectileSprite);
}
