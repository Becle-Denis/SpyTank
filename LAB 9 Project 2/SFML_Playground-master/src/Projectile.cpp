#include "Projectile.h"


double Projectile::s_projectile_default_speed = 800;

Projectile::Projectile(SoundManager& soundManager) : 
	m_soundManagerPtr(soundManager)
{
	m_projectileSprite.setTexture(ResourcesManager::getTexture(TexturesName::SPRITE_SHEET));
	sf::IntRect baseRect(8, 175, 8, 10);
	m_projectileSprite.setTextureRect(baseRect);
	m_projectileSprite.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	setInactive();
}

void Projectile::launch(sf::Vector2f position, double rotation, double speed)
{
	m_rotation = rotation;
	m_projectileSprite.setRotation(rotation - 90);
	m_projectileSprite.setPosition(position);

	m_projectile_speed = speed;

	m_soundPtr = m_soundManagerPtr.startProjectileSound(position);

}

sf::Sprite const& Projectile::getSprite() const
{
	return m_projectileSprite;
}

void Projectile::setInactive()
{
	m_projectileSprite.setPosition(sf::Vector2f(-100, -100));
	//sound desactivation 
	if (m_soundPtr != nullptr)
	{
		m_soundPtr->setEnd();
	}
	m_soundPtr = nullptr;
}

bool Projectile::isInactive() const
{
	return m_projectileSprite.getPosition() == sf::Vector2f(-100,-100);
}

void Projectile::update(double dt)
{
	//get new position
	float newXPosition = m_projectileSprite.getPosition().x + cos(m_rotation * MathUtility::DEG_TO_RAD) * m_projectile_speed * (dt / 1000);
	float newYPosition = m_projectileSprite.getPosition().y + sin(m_rotation * MathUtility::DEG_TO_RAD) * m_projectile_speed * (dt / 1000);
	sf::Vector2f newPosition(newXPosition, newYPosition);

	//set new position 
	m_projectileSprite.setPosition(newPosition);

	//update the sound poisition
	if (m_soundPtr != nullptr)
	{
		m_soundPtr->updatePosition(newPosition);
	}
}

int Projectile::lifeState(std::map<int, std::list< sf::Sprite> >& wallMap) const
{
	
	sf::Vector2f position = m_projectileSprite.getPosition();
	//check for the limit of the screen 
	if (position.x < -1000 || position.y < -1000 || position.x > 2440 || position.y > 1900 )
	{
		return 2; //outisde the map 
	}
	//check for collision with wall
	//find the cell number
	int cellN = (floor(position.y / (ScreenSize::s_width / 10)) * 10) + (floor(position.x / (ScreenSize::s_height / 10)));
	
	if (wallMap.count(cellN) > 0)
	{
		//check for collision
		for (sf::Sprite& wall : wallMap[cellN])
		{
			if (CollisionDetector::collision(m_projectileSprite, wall))
			{
				return 1; //Hit a target 
			}
		}
	}
	return -1; // Alive

	/*
	//Check for collision with the AITankb DISABLED
	if ((CollisionDetector::collision(m_projectileSprite,aiTankSprites.first)) || (CollisionDetector::collision(m_projectileSprite,aiTankSprites.second)))
	{
		return 3; //Hit the AITank
	}
	*/
	
}

bool Projectile::collideWithSprites(std::pair<sf::Sprite, sf::Sprite> sprites)
{
	return ((CollisionDetector::collision(m_projectileSprite, sprites.first)) || (CollisionDetector::collision(m_projectileSprite, sprites.second)));
}


void Projectile::render(sf::RenderWindow& window) const
{
	window.draw(m_projectileSprite);
}

void Projectile::setLightMode(LightMode mode)
{
	if (mode == LightMode::DAY)
	{
		m_projectileSprite.setTexture(ResourcesManager::getTexture(TexturesName::SPRITE_SHEET));
	}
	else
	{
		m_projectileSprite.setTexture(ResourcesManager::getTexture(TexturesName::SPRITE_SHEET_NIGHT));
	}
}
