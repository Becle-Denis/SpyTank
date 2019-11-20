#include "Target.h"

const sf::Time Target::S_BLINKING_TIME = sf::seconds(5);

Target::Target() :
	m_state(TargetState::NotInitialised),
	m_isDisplayed(false)
{
	
}

void Target::init(sf::Texture const* texture, sf::Vector2f position, sf::Time timeToStart, sf::Time timeOnScreen)
{
	m_timeToStart = timeToStart;
	m_timeOnScreen = timeOnScreen;
	m_texture = texture;
	m_sprite.setTexture(*m_texture);
	sf::IntRect baseRect(0, 0, 230, 230);
	m_sprite.setTextureRect(baseRect);
	m_sprite.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_sprite.setPosition(position);
	m_sprite.setScale(0.3, 0.3);
	m_state = TargetState::NotStarted;
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
			reveal();
		}
		break;

	case TargetState::OnScreen:
		if (m_timer.isExpired())
		{
			m_timer.restart(S_BLINKING_TIME);
			m_state = TargetState::Blinking;
			m_blinkingTimer.restart(sf::milliseconds(75));
		}
		break;

	case TargetState::Blinking:
		if (m_blinkingTimer.isExpired())
		{
			m_isDisplayed = !m_isDisplayed;
			m_blinkingTimer.restart(sf::milliseconds(75));
		}
		if (m_timer.isExpired())
		{
			m_isDisplayed = false;
			m_state = TargetState::DeadByTime;
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


sf::Time Target::hit()
{
	sf::Time remaingTime = m_timer.getRemainingTime();
	if (m_state == TargetState::OnScreen)
	{
		remaingTime += S_BLINKING_TIME;
	}
	m_isDisplayed = false;
	m_state = TargetState::DeadByHit;
	return remaingTime;
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

void Target::reveal(sf::Time bonusTime)
{
	if (m_state == TargetState::NotDisplayed)
	m_timeOnScreen += bonusTime;
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
