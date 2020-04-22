#include "Tank.h"
#include <iostream>


Tank::Tank(std::vector<sf::Sprite>& wallSprites, std::map<int, std::list< sf::Sprite> >& wallMap, TargetManager& targets, ProjectilePool& projectilePool, SoundManager& soundManager, HUD& hud) :
	m_wallSprites(wallSprites),
	m_wallSpatialMap(wallMap),
	m_targets(targets),
	m_projectilesPool(projectilePool),
	m_maximumSpeed(100.0),
	m_soundManager(soundManager),
	m_hud(hud),
	m_performances(&m_targets)
{
	initSprites();
}

////////////////////////////////////////////////////////////
void Tank::initialise(GameState gameRunningState)
{
	m_motorSound = m_soundManager.tankMotorEffect();
	//reseting values 
	m_fireTimer.restart(sf::seconds(1.f));
	m_performances = UserPerformance(&m_targets);
	m_enableRotation = true;
	m_rotation = 0.0;
	m_turretRotation = 0.0;
	m_speed = 0;
	m_lifePoint = LIFE_POINTS;
	m_maximumSpeed = 100.0;
	m_rotationStep = 1.0f;
	m_decreasingFactor = 0.99f;
	m_rotationBugUpdateCount = 0;
	m_rotationBug = -1;
	m_capturedTarget = 0;

	//resetting sprite 
	m_tankBase.setPosition(m_startingPosition);
	m_turret.setPosition(m_startingPosition);
	m_tankBase.setRotation(m_rotation);
	m_turret.setRotation(m_turretRotation);
	m_tankBase.setColor(sf::Color::White);
	m_turret.setColor(sf::Color::White);
	m_tankBase.setScale(1, 1);
	m_turret.setScale(1, 1);

	m_gameplay = gameRunningState;
}

////////////////////////////////////////////////////////////
void Tank::update(double dt)
{
	//Specific Gameplay 
	switch (m_gameplay)
	{
	case GameState::RUNNING_HIT_GAME:
		//updating the projectiles
		for (int i = m_projectilesPtr.size() - 1; i >= 0; i--)
		{
			//update 
			m_projectilesPtr.at(i)->update(dt);

			//Collision 
			int collisionResult = m_targets.checkForCollision(m_projectilesPtr.at(i)->getSprite());
			if (collisionResult != -1) // Colision with target
			{
				m_targets.capture(collisionResult, true);
				m_projectilesPtr.at(i)->setInactive();
				m_projectilesPtr.erase(m_projectilesPtr.begin() + i);
				m_performances.targetHitted();
			}
			else  //colision with wall or outside the map
			{
				int lifeState = m_projectilesPtr.at(i)->lifeState(m_wallSpatialMap);
				if (lifeState != -1)
				{
					if (lifeState == 1) //Colision With wall
					{
						//playing the impact sound 
						m_soundManager.playWallImpactSound(m_projectilesPtr.at(i)->getSprite().getPosition());
					}
					//removing the projectile 
					m_projectilesPtr.at(i)->setInactive();
					m_projectilesPtr.erase(m_projectilesPtr.begin() + i);
				}
			}
		}

		//updating the timer
		if (m_fireTimer.isExpired())
		{
			canFire = true;
		}

		break;
	case GameState::RUNNING_CATCH_GAME:
		// check for target Collection 
		int collisionResult = m_targets.checkForCollision(m_tankBase);
		if (collisionResult != -1)
		{
			m_targets.capture(collisionResult, false);
			m_capturedTarget++;
		}


		//Rotation Bug (damaged Gameplay)

		if (m_rotationBugUpdateCount == m_rotationBug)
		{
			m_rotationBugUpdateCount = 0;

			if (abs(m_speed) > 1)
			{
				increaseRotation();
			}
		}
		else
		{
			m_rotationBugUpdateCount++;
		}
	}

	



	//TANK MOVEMENT, GENERAL GAMEPLAY 
	// Handle user input 
	handleKeyInput();

	//save the actual position
	m_previousPosition = m_tankBase.getPosition();
	
	//get new position
	float newXPosition = m_tankBase.getPosition().x + cos(m_rotation * MathUtility::DEG_TO_RAD) * m_speed * (dt / 1000);
	float newYPosition = m_tankBase.getPosition().y + sin(m_rotation * MathUtility::DEG_TO_RAD) * m_speed * (dt / 1000);

	//set new position 
	m_tankBase.setPosition(sf::Vector2f(newXPosition,newYPosition));
	m_turret.setPosition(sf::Vector2f(newXPosition, newYPosition));

	//replacing the tank inside the map 
	limitInsideMap();

	//set rotation 
	m_tankBase.setRotation(m_rotation);
	m_turret.setRotation(m_turretRotation);

	//decrease speed 
	m_speed = m_decreasingFactor * m_speed;

	//limiting speed 
	double high = m_maximumSpeed;
	double low = m_maximumSpeed * -1;
	
	m_speed = std::clamp(m_speed,low,high);
	

	//checking wall collisions
	if (checkWallCollision())
	{
		deflect();
	}
	else
	{
		//reanable rotation, when the collision is gone
		m_enableRotation = true;
	}

	
	//soundStuff 
	m_soundManager.updateListenerPostion(m_tankBase.getPosition(), m_rotation);
	if (m_motorSound != nullptr)
	{
		m_motorSound->updateState(m_tankBase.getPosition(), abs(m_speed / m_maximumSpeed));
	}
}

////////////////////////////////////////////////////////////
void Tank::render(sf::RenderWindow & window)  const
{
	for (Projectile* projectile : m_projectilesPtr)
	{
		projectile->render(window);
	}
	window.draw(m_tankBase);
	window.draw(m_turret);
}

////////////////////////////////////////////////////////////
void Tank::initSprites()
{
	sf::Texture const& texture = ResourcesManager::getTexture(TexturesName::SPRITE_SHEET);
	// Initialise the tank base
	m_tankBase.setTexture(texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);


	// Initialise the turret
	m_turret.setTexture(texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);


}

////////////////////////////////////////////////////////////
void Tank::setStartingPosition(sf::Vector2f const& pos)
{
	m_tankBase.setPosition(pos);
	m_turret.setPosition(pos);
	m_startingPosition = pos;
}

////////////////////////////////////////////////////////////
UserPerformance& Tank::getPerformance()
{
	return m_performances;
}

sf::Vector2f Tank::getPosition() const
{
	return m_tankBase.getPosition();
}

sf::Sprite Tank::getBaseTank() const
{
	return m_tankBase;
}

sf::Sprite Tank::getTurret() const
{
	return m_turret;
}

////////////////////////////////////////////////////////////
void Tank::clearDependantObjects()
{
	//deleting the projectiles 
	for (Projectile* projPtr : m_projectilesPtr)
	{
		projPtr->setInactive();
		projPtr = nullptr;
	}
	m_projectilesPtr.clear();

	//stopping the sound motor effect
	if (m_motorSound != nullptr)
	{
		m_motorSound->setEnd();
		m_motorSound = nullptr;
	}
	
}


////////////////////////////////////////////////////////////
void Tank::increaseSpeed()
{

	if (m_speed < 100.0)
	{
		//saving the actual speed
		m_previousSpeed = m_speed;

		m_speed += 1;
	}
}

////////////////////////////////////////////////////////////
void Tank::decreaseSpeed()
{
	if (m_speed > -100.0)
	{
		//saving the actual speed
		m_previousSpeed = m_speed;

		m_speed -= 1;
	}
}

////////////////////////////////////////////////////////////
void Tank::increaseRotation()
{
	//if (m_enableRotation)
	//{
		//saving the actual rotation
		m_previousRotation = m_rotation;

		m_rotation += m_rotationStep;
		if (m_rotation == 360.0)
		{
			m_rotation = 0;
		}
		increaseTurretRotation();
	//}

}

////////////////////////////////////////////////////////////
void Tank::decreaseRotation()
{
	//if (m_enableRotation)
	//{
		//saving the actual rotation
		m_previousRotation = m_rotation;

		m_rotation -= m_rotationStep;
		if (m_rotation == 0.0)
		{
			m_rotation = 359.0;
		}
		decreaseTurretRotation();
	//}

}

////////////////////////////////////////////////////////////
void Tank::increaseTurretRotation()
{
	if (m_enableRotation)
	{
		//saving the actual turret rotation
		m_previousTurretRotation = m_turretRotation;

		m_turretRotation += m_rotationStep;
		if (m_turretRotation == 360.0)
		{
			m_turretRotation = 0;
		}
	}
}

////////////////////////////////////////////////////////////
void Tank::decreaseTurretRotation()
{
	if (m_enableRotation)
	{
		//saving the actual turret rotation
		m_previousTurretRotation = m_turretRotation;

		m_turretRotation -= m_rotationStep;
		if (m_turretRotation == 0.0)
		{
			m_turretRotation = 359.0;
		}
	}
}

//////////////////////////////////////////////////////////////
void Tank::centerTurret()
{
	m_turretRotation = m_rotation;
}

////////////////////////////////////////////////////////////
bool Tank::checkWallCollision()
{
	for (sf::Sprite const& sprite : m_wallSprites)
	{
		// Checks if either the tank base or turret has collided with the current wall sprite.
		if (CollisionDetector::collision(m_turret, sprite) ||
			CollisionDetector::collision(m_tankBase, sprite))
		{
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////
void Tank::limitInsideMap()
{
	sf::Vector2f tankPosition = m_tankBase.getPosition();
	bool changedPosition = false;
	//limiting the position;
	if (tankPosition.x < 0.0f)
	{
		changedPosition = true;
		tankPosition.x = 0.0f;
	}
	if (tankPosition.x > ScreenSize::s_height)
	{
		changedPosition = true;
		tankPosition.x = ScreenSize::s_height;
	}
	if (tankPosition.y < 0)
	{
		changedPosition = true;
		tankPosition.y = 0.0f;
	}
	if (tankPosition.y > ScreenSize::s_width)
	{
		changedPosition = true;
		tankPosition.y = ScreenSize::s_width;
	}

	//appliying changes
	if (changedPosition)
	{
		m_tankBase.setPosition(tankPosition);
		m_turret.setPosition(tankPosition);
	}
}

////////////////////////////////////////////////////////////
void Tank::deflect()
{
	
	// In case tank was rotating.
	adjustRotation();

	// If tank was moving.
	if (m_speed != 0)
	{
		// Temporarily disable turret rotations on collision.
		m_enableRotation = false;
		// Back up to position in previous frame.
		m_tankBase.setPosition(m_previousPosition);
		// Apply small force in opposite direction of travel.
		if (m_previousSpeed < 0)
		{
			m_speed = 40;
		}
		else
		{
			m_speed = -40;
		}
	}
	
}

////////////////////////////////////////////////////////////
void Tank::adjustRotation()
{
	// If tank was rotating...
	if (m_rotation != m_previousRotation)
	{
		// Work out which direction to rotate the tank base post-collision.
		if (m_rotation > m_previousRotation)
		{
			m_rotation = m_previousRotation - 1;
		}
		else
		{
			m_rotation = m_previousRotation + 1;
		}
	}
	// If turret was rotating while tank was moving
	if (m_turretRotation != m_previousTurretRotation)
	{
		// Set the turret rotation back to it's pre-collision value.
		m_turretRotation = m_previousTurretRotation;
	}
}

////////////////////////////////////////////////////////////////
void Tank::fire()
{
	Projectile* newProjectilePtr = m_projectilesPool.getProjectile();
	if (newProjectilePtr != nullptr)
	{
		newProjectilePtr->launch(m_tankBase.getPosition(),m_turretRotation);
		m_projectilesPtr.push_back(newProjectilePtr);
		m_performances.projectileFired();
		canFire = false;
		m_fireTimer.restart(sf::seconds(1.f));

		//sound stuff

		//get a vector position at the end of the turret
		sf::Vector2f initialPosition = m_turret.getPosition();
		initialPosition.x += 30;
		initialPosition -= m_turret.getPosition();
		thor::rotate<float>(initialPosition, m_turretRotation);
		initialPosition += m_turret.getPosition();

		m_soundManager.playFireSound(initialPosition);
	}
	else
	{
		std::cout << "Error no Projectile available" << std::endl;
	}
}

int Tank::getNumberOfCapturedTarget() const
{
	return m_capturedTarget;
}

void Tank::setLightMode(LightMode mode)
{
	if (mode == LightMode::DAY)
	{
		m_tankBase.setTexture(ResourcesManager::getTexture(TexturesName::SPRITE_SHEET));
		m_turret.setTexture(ResourcesManager::getTexture(TexturesName::SPRITE_SHEET));
	}
	else
	{
		m_tankBase.setTexture(ResourcesManager::getTexture(TexturesName::SPRITE_SHEET_NIGHT));
		m_turret.setTexture(ResourcesManager::getTexture(TexturesName::SPRITE_SHEET_NIGHT));
	}
}

////////////////////////////////////////////////////////////
void Tank::takeImpact()
{
	//affecting life point 
	m_lifePoint --;

	//affecting gameplay
	int impactSequence = rand() % 5;
	switch (impactSequence)
	{
	case 0:
		//Steering damaged
		m_maximumSpeed *= 0.80;
		m_hud.addEvent("Steering damaged");
		break;
	case 1:
		//navigation damaged
		m_rotationStep *= 0.75;
		m_hud.addEvent("Navigation damaged");
		break;
	case 2:
		//Direction damaged
		m_rotationBugUpdateCount = 0;
		m_rotationBug = 4;
		m_hud.addEvent("Direction damaged");
		break;
	case 3:
		// Structure damaged
		m_tankBase.setScale(1.2, 1.2);
		m_turret.setScale(1.2, 1.2);
		m_hud.addEvent("Structure damaged");
	}
	
	//affecting sprite color 
	sf::Uint8 otherColors = static_cast<sf::Uint8>(m_lifePoint * (255.0 / LIFE_POINTS));
	sf::Color newColor = sf::Color(255,otherColors, otherColors);
	m_tankBase.setColor(newColor);
	m_turret.setColor(newColor);

	//sound stuff 
	m_soundManager.playTankImpact();
	
}

bool Tank::isAlive() const
{
	return m_lifePoint > 0;
}




//////////////////////////////////////////////////////////////
void Tank::handleKeyInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		increaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		increaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		decreaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
	{
		increaseTurretRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
	{
		decreaseTurretRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
	{
		centerTurret();
	}
	if (canFire && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && m_gameplay == GameState::RUNNING_HIT_GAME)
	{
		fire();
	}

}