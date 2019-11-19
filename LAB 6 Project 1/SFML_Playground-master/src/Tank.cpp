#include "Tank.h"
#include <iostream>


Tank::Tank(sf::Texture const& texture, std::vector<sf::Sprite>& wallSprites, std::vector<Target>& targets)
	: m_texture(texture),
	m_wallSprites(wallSprites),
	m_targets(targets),
	m_maximumSpeed(80)
{
	initSprites();
	m_fireTimer.restart(sf::seconds(1.f));
}

void Tank::update(double dt)
{	
	//updating the projectiles
	for (int i = m_projectiles.size() - 1; i >= 0; i--)
	{
		//update 
		m_projectiles.at(i).update(dt);
		//colision 
		if (!(m_projectiles.at(i).isAlive(m_wallSprites))) //colision with wall 
		{
   			m_projectiles.erase(m_projectiles.begin() + i);
		}
		else if (m_projectiles.at(i).hitTarget(m_targets)) // colision with target 
		{
			m_projectiles.erase(m_projectiles.begin() + i);
			m_performances.targetHitted++;
		}
	}

	//updating the timer
	if (m_fireTimer.isExpired())
	{
		canFire = true;
	}

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

	//set rotation 
	m_tankBase.setRotation(m_rotation);
	m_turret.setRotation(m_turretRotation);



	
	//decrease speed 
	m_speed = 0.99 * m_speed;

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



}


void Tank::render(sf::RenderWindow & window) 
{
	for (Projectile& projectile : m_projectiles)
	{
		projectile.render(window);
	}
	window.draw(m_tankBase);
	window.draw(m_turret);
}


void Tank::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);


	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);


}

void Tank::setPosition(sf::Vector2f const& pos)
{
	m_tankBase.setPosition(pos);
	m_turret.setPosition(pos);
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

		m_rotation += 1;
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

		m_rotation -= 1;
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

		m_turretRotation += 1;
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

		m_turretRotation -= 1;
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
	Projectile p;
	p.setSprite(&m_texture);
	p.launch(m_tankBase.getPosition(), m_turretRotation);
	m_projectiles.push_back(p);
	m_performances.projectileFired++;
	canFire = false;
	m_fireTimer.restart(sf::seconds(1.f));
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
	if (canFire && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		fire();
	}

}