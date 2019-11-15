#include "Target.h"

const sf::Time Target::S_BLINKING_TIME = sf::seconds(5);

Target::Target(sf::Texture const& texture, sf::Vector2f position, sf::Time timeToStart, sf::Time timeOnScreen) :
	m_texture(texture), m_state(TargetState::NotStarted), m_timeOnScreen(timeOnScreen), m_timeToStart(timeToStart),
	m_isDisplayed(false)
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
	case TargetState::NotStarted:
		m_timer.restart(m_timeToStart);
		m_state = TargetState::NotDisplayed;
		break;

	case TargetState::NotDisplayed:
		if (m_timer.isExpired())
		{
			m_isDisplayed = true;
			if (m_timeOnScreen > S_BLINKING_TIME)
			{
				m_timer.restart(m_timeOnScreen - S_BLINKING_TIME);
				m_state = TargetState::OnScreen;
			}
			else
			{
				m_timer.restart(m_timeOnScreen);
				m_state = TargetState::Blinking;
			}

		}
		break;

	case TargetState::OnScreen:
		if (m_timer.isExpired())
		{
			m_timer.restart(S_BLINKING_TIME);
			m_state = TargetState::Blinking;
			m_blinkingTimer.restart(sf::milliseconds(250));
		}
		break;

	case TargetState::Blinking:
		if (m_blinkingTimer.isExpired())
		{
			m_isDisplayed = !m_isDisplayed;
			m_blinkingTimer.restart(sf::milliseconds(250));
		}
		if (m_timer.isExpired())
		{
			m_isDisplayed = false;
			m_state = TargetState::DeadByHit;
		}
		break;
	}
}

void Target::render(sf::RenderWindow& window)
{
	if (m_isDisplayed)
	{
		window.draw(m_sprite);
	}
}


void Target::hit()
{
	m_state = TargetState::DeadByHit;
}

bool Target::isAlive()
{
	return m_state >= NotStarted;
}

bool Target::isColliding(sf::Sprite const& sprite)
{
	if (m_state >= TargetState::OnScreen)
	{
		if (CollisionDetector::collision(m_sprite,sprite))
		{
			return true;
		}
	}
	return false;
}
