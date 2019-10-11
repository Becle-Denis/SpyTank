#include "Tank.h"
#include <iostream>
#include "MathUtility.h"


Tank::Tank(sf::Texture const & texture/*, sf::Vector2f const & pos*/)
: m_texture(texture)
{
	
}

void Tank::update(double dt)
{	
	//get new position
	float newXPosition = m_tankBase.getPosition().x + cos(m_rotation * MathUtility::DEG_TO_RAD) * m_speed * (dt / 1000);
	float newYPosition = m_tankBase.getPosition().y + sin(m_rotation * MathUtility::DEG_TO_RAD) * m_speed * (dt / 1000);

	//set new position 
	m_tankBase.setPosition(sf::Vector2f(newXPosition,newYPosition));
	m_turret.setPosition(sf::Vector2f(newXPosition, newYPosition));

	//set rotation 
	m_tankBase.setRotation(m_rotation);
	m_turret.setRotation(m_rotation);

}


void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
}


void Tank::initSprites(sf::Vector2f const & pos)
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_tankBase.setPosition(pos);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
	m_turret.setPosition(pos);

}

void Tank::setPosition(sf::Vector2f const& pos)
{
	initSprites(pos);
}


////////////////////////////////////////////////////////////
void Tank::increaseSpeed()
{
	if (m_speed < 100.0)
	{
		m_speed += 1;
	}
}

////////////////////////////////////////////////////////////
void Tank::decreaseSpeed()
{
	if (m_speed > -100.0)
	{
		m_speed -= 1;
	}
}

////////////////////////////////////////////////////////////
void Tank::increaseRotation()
{
	m_rotation += 1;
	if (m_rotation == 360.0)
	{
		m_rotation = 0;
	}
}

////////////////////////////////////////////////////////////
void Tank::decreaseRotation()
{
	m_rotation -= 1;
	if (m_rotation == 0.0)
	{
		m_rotation = 359.0;
	}
}