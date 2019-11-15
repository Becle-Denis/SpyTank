#include "Target.h"

Target::Target(sf::Texture const& texture, sf::Vector2f position, sf::Time timeToStart, sf::Time timeOnScreen) :
	m_texture(texture),m_state(0),m_timeOnScreen(timeOnScreen),m_timeToStart(timeToStart)
{
	m_sprite.setTexture(texture);
	sf::IntRect baseRect(0, 0, 300, 300);
	m_sprite.setTextureRect(baseRect);
	m_sprite.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_sprite.setPosition(position);
	m_sprite.setScale(0.38, 0.38);
}

void Target::update()
{
	switch (m_state)
	{
	case 0:
		m_timer.restart(m_timeToStart);
		m_state++;
		break;
	case 1:
		if (m_timer.isExpired())
		{
			m_timer.restart(m_timeOnScreen);
			m_state++;
		}
		break;

	case 2:
		if (m_timer.isExpired())
		{
			m_state++;
		}
		break;
	}
}

void Target::render(sf::RenderWindow& window)
{
	if (m_state == 2)
	{
		window.draw(m_sprite);
	}
}


void Target::hit()
{
	m_state = -2;
}

bool Target::isAlive()
{
	return m_state >= 0;
}

bool Target::isColliding(sf::Sprite const& sprite)
{
	if (m_state == 2)
	{
		if (CollisionDetector::collision(m_sprite,sprite))
		{
			return true;
		}
	}
	return false;
}
