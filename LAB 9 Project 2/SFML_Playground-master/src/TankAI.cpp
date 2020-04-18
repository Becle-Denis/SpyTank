#include "TankAi.h"

const sf::Time TankAi::FIRE_RELOAD_TIME = sf::milliseconds(2050);

////////////////////////////////////////////////////////////
TankAi::TankAi(std::vector<sf::Sprite> & wallSprites, std::map<int, std::list< sf::Sprite> >& wallMap, ProjectilePool& projectilesPool) : 
	m_wallSprites(wallSprites)
	, m_steering(0, 0)
	, m_state(AIState::PATROL_MAP)
	, m_leftConeArray(sf::Lines,2)
	, m_rightConeArray(sf::Lines,2)
	, m_projectilesPool(projectilesPool)
	, m_wallSpatialMap(wallMap)
	, m_lightMode(LightMode::DAY)
{
	// Initialises the tank base and turret sprites.
	initSprites();

	//Set the patrol Points 
	m_patrolPoint = {
		sf::Vector2f(100,100),
		sf::Vector2f(500,300),
		sf::Vector2f(1000,500),
		sf::Vector2f(100,650),
		sf::Vector2f(300,500),
		sf::Vector2f(1000,220),
	};
}

////////////////////////////////////////////////////////////
void TankAi::start()
{
	//reseting the tank position 
	m_tankBase.setPosition(m_startingPosition);
	m_turret.setPosition(m_startingPosition);
	m_tankBase.setRotation(0);
	m_turret.setRotation(0);
	m_rotation = 0;
	m_turretRotation = 0;

	//reseting the tank status 
	m_fireTimer.restart(FIRE_RELOAD_TIME);
	m_state = AIState::PATROL_MAP;
	m_patrolPointIndex = rand() % m_patrolPoint.size();
	m_patrolConeRange = MIN_PATROL_CONE_RANGE;
	m_patrolZoneSize = MAX_PATROL_ZONE_SIZE;
	//Vertex Array Color 
	m_leftConeArray[0].color = sf::Color(0, 150, 0, 255);
	m_rightConeArray[0].color = sf::Color(0, 150, 0, 255);
	m_leftConeArray[1].color = sf::Color(0, 205, 0, 50);
	m_rightConeArray[1].color = sf::Color(0, 205, 0, 50);
	
}

////////////////////////////////////////////////////////////
void TankAi::update(Tank& playerTank, double dt)
{
	//updating the projectiles
	for (int i = m_projectilesPtr.size() - 1; i >= 0; i--)
	{
		//update 
		m_projectilesPtr.at(i)->update(dt);

		//Collisions 
		//colision with player 
		if (m_projectilesPtr.at(i)->collideWithSprites({ playerTank.getBaseTank(),playerTank.getTurret() }))
		{
			playerTank.takeImpact();

			//removing the projectile 
			m_projectilesPtr.at(i)->setInactive();
			m_projectilesPtr.erase(m_projectilesPtr.begin() + i);
		}
		else
		{
			//collision with wall or something else
			int lifeState = m_projectilesPtr.at(i)->lifeState(m_wallSpatialMap);
			if (lifeState != -1)
			{
				//removing the projectile 
				m_projectilesPtr.at(i)->setInactive();
				m_projectilesPtr.erase(m_projectilesPtr.begin() + i);
			}
		}
	}

	sf::Vector2f playerTankPosition = playerTank.getPosition();
	double distanceToPlayer = MathUtility::distance(m_tankBase.getPosition(), playerTankPosition);
	sf::Vector2f destination;
	if (m_state == AIState::PATROL_MAP && m_lightMode != LightMode::DAY)
	{
		//Move to the patrol point 
		destination = m_patrolPoint.at(m_patrolPointIndex) - m_tankBase.getPosition();
		// turret rotation
		m_turretRotation = m_turretRotation + PATROL_ROTATION_SPEED * dt;
		if (m_turretRotation > 360)
		{
			m_turretRotation -= 360;
		}
		//Resizing the Vision Cone 
		if (m_patrolConeRange > MIN_PATROL_CONE_RANGE)
		{
			m_patrolConeRange -= PATROL_CONE_RANGE_SPEED * dt;
		}
		if (m_patrolZoneSize < MAX_PATROL_ZONE_SIZE)
		{
			m_patrolZoneSize += PATROL_CONE_ZONE_SPEED * dt;
		}

		//Change patrol point
		if (thor::length(destination) < MAX_SEE_AHEAD)
		{
			m_patrolPointIndex = rand() % m_patrolPoint.size();
		}
	}
	else 
	{
		// seek to the player 
		destination = seek(playerTankPosition);

		//Firing
		if (distanceToPlayer < MAX_PATROL_ZONE_SIZE / 1.5
			&& m_fireTimer.isExpired())
		{
			fire();
			m_fireTimer.restart(FIRE_RELOAD_TIME);
		}


		//rotate the turret in the player direction
		int playerToTurretPostion = MathUtility::pointPositionToLine(m_tankBase.getPosition(), m_tankBase.getPosition() + thor::rotatedVector(sf::Vector2f(100,0),(float) m_turretRotation), playerTankPosition);
		if (playerToTurretPostion < 0)
		{
			m_turretRotation = m_turretRotation - PATROL_ROTATION_SPEED * dt;
			if (m_turretRotation < 0)
			{
				m_turretRotation += 360;
			}
		}
		else if (playerToTurretPostion > 0)
		{
			m_turretRotation = m_turretRotation + PATROL_ROTATION_SPEED * dt;
			if (m_turretRotation > 360)
			{
				m_turretRotation -= 360;
			}
		}

		//Resizing Vision Cone 
		if (distanceToPlayer < MAX_PATROL_ZONE_SIZE / 2 && m_state == AIState::ATTACK_PLAYER)
		{
			if (m_patrolConeRange < MAX_PATROL_CONE_RANGE)
			{
				m_patrolConeRange += PATROL_CONE_RANGE_SPEED * dt;
			}
			if (m_patrolZoneSize > MIN_PATROL_ZONE_SIZE)
			{
				m_patrolZoneSize -= PATROL_CONE_ZONE_SPEED * dt;
			}
		}
	}


	//Tank Movement
	sf::Vector2f acceleration;
	m_steering += thor::unitVector(destination);
	m_steering += collisionAvoidance();
	m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
	// calculating acceleration with the mass 
	acceleration = m_steering / MASS;

	//limiting the speed 
	if (m_state == AIState::ATTACK_PLAYER || m_lightMode == LightMode::DAY)
	{
		m_velocity = MathUtility::truncate(m_velocity + acceleration, SPEED_ATTACK);
	}
	else
	{
		m_velocity = MathUtility::truncate(m_velocity + acceleration, SPEED_NORMAL);
	}
	

	// Now we need to convert our velocity vector into a rotation angle between 0 and 359 degrees.
	// The m_velocity vector works like this: vector(1,0) is 0 degrees, while vector(0, 1) is 90 degrees.
	// So for example, 223 degrees would be a clockwise offset from 0 degrees (i.e. along x axis).
	// Note: we add 180 degrees below to convert the final angle into a range 0 to 359 instead of -PI to +PI
	auto dest = atan2(-1 * m_velocity.y, -1 * m_velocity.x) / thor::Pi * 180 + 180;

	auto currentRotation = m_rotation; 

	// Find the shortest way to rotate towards the player (clockwise or anti-clockwise)
	if (std::round(currentRotation - dest) == 0.0)
	{
		m_steering.x = 0;
		m_steering.y = 0;
	}

	else if ((static_cast<int>(std::round(dest - currentRotation + 360))) % 360 < 180)
	{
		// rotate clockwise
		m_rotation = static_cast<int>((m_rotation)+1) % 360;
	}
	else
	{
		// rotate anti-clockwise
		m_rotation = static_cast<int>((m_rotation)-1) % 360;
	}

	updateMovement(dt);

	//update the cone Vision and Ai State
	adaptConeDisplay();
	

	//update the AI state
	//check if the player is inside the cone and the AI can see it
	if ((MathUtility::pointPositionToLine(m_leftConeArray[0].position, m_leftConeArray[1].position, playerTankPosition) > 0)
		&& (MathUtility::pointPositionToLine(m_rightConeArray[0].position, m_rightConeArray[1].position, playerTankPosition) < 0)
		&& (distanceToPlayer < m_patrolZoneSize) //distance to player 
		&& !CollisionDetector::collisionLineWithObjects(m_tankBase.getPosition(), playerTankPosition, m_wallSprites)) //no obstacle between player and AI
	{
		//Player inside the cone 
		if (m_state != AIState::ATTACK_PLAYER)
		{
			//Vertex Array Red Color 
			m_leftConeArray[0].color = sf::Color(150, 0, 0, 255);
			m_rightConeArray[0].color = sf::Color(150, 0, 0, 255);
			m_leftConeArray[1].color = sf::Color(205, 0, 0, 0);
			m_rightConeArray[1].color = sf::Color(205, 0, 0, 0);
		
			m_state = AIState::ATTACK_PLAYER;
		}
	}
	else
	{
		//player outside the cone 
		if (m_state != AIState::PATROL_MAP)
		{
			//Vertex Array Green Color 
			m_leftConeArray[0].color = sf::Color(0, 150, 0, 255);
			m_rightConeArray[0].color = sf::Color(0, 150, 0, 255);
			m_leftConeArray[1].color = sf::Color(0, 205, 0, 0);
			m_rightConeArray[1].color = sf::Color(0, 205, 0, 0);

			m_state = AIState::PATROL_MAP;
		}
	}
}

////////////////////////////////////////////////////////////
bool TankAi::collidesWithPlayer(Tank const & playerTank) const
{
    // Checks if the AI tank has collided with the player tank.
    if (CollisionDetector::collision(m_turret, playerTank.getTurret()) ||
        CollisionDetector::collision(m_tankBase, playerTank.getBaseTank()))
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////
void TankAi::render(sf::RenderWindow & window)
{
	window.draw(m_tankBase);

	if (m_lightMode == LightMode::NIGHT)
	{
		window.draw(m_leftConeArray);
		window.draw(m_rightConeArray);
	}

	for (Projectile* projPtr : m_projectilesPtr)
	{
		projPtr->render(window);
	}

	window.draw(m_turret);
}

////////////////////////////////////////////////////////////
void TankAi::init(sf::Vector2f position)
{
	m_startingPosition = position;
	m_tankBase.setPosition(m_startingPosition);
	m_turret.setPosition(m_startingPosition);

	for (sf::Sprite const wallSprite : m_wallSprites)
	{
		sf::CircleShape circle(wallSprite.getTextureRect().width * 1.5f);
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(wallSprite.getPosition());
		m_obstacles.push_back(circle);
	}
}


////////////////////////////////////////////////////////////
sf::Vector2f TankAi::seek(sf::Vector2f playerPosition) const
{
	return (playerPosition - m_tankBase.getPosition()); 
}

////////////////////////////////////////////////////////////
sf::Vector2f TankAi::collisionAvoidance()
{
	auto headingRadians = thor::toRadian(m_rotation);
	sf::Vector2f headingVector(std::cos(headingRadians) * MAX_SEE_AHEAD, std::sin(headingRadians) * MAX_SEE_AHEAD);
	m_ahead = m_tankBase.getPosition() + headingVector;
	m_halfAhead = m_tankBase.getPosition() + (headingVector * 0.5f);
	const sf::CircleShape mostThreatening = findMostThreateningObstacle();
	sf::Vector2f avoidance(0, 0);
	if (mostThreatening.getRadius() != 0.0)
	{		
		double distanceAheadFromObstacleCenter = MathUtility::distance(m_ahead, mostThreatening.getPosition());
		bool outOfDeadZone = ! ((distanceAheadFromObstacleCenter > mostThreatening.getRadius() * 0.95) && (distanceAheadFromObstacleCenter < mostThreatening.getRadius() * 1.05));
		if (outOfDeadZone)
		{
			avoidance.x = m_ahead.x - mostThreatening.getPosition().x;
			avoidance.y = m_ahead.y - mostThreatening.getPosition().y;
			avoidance = thor::unitVector(avoidance);
			avoidance *= MAX_AVOID_FORCE;
		}
	}
	return avoidance;
}

////////////////////////////////////////////////////////////
const sf::CircleShape TankAi::findMostThreateningObstacle()
{
	
	sf::CircleShape mostThreatening; //set to 0 radius, (null)
	for (sf::CircleShape obstacle : m_obstacles)
	{
		bool collides = MathUtility::lineIntersectsCircle(m_ahead, m_halfAhead, obstacle);

		if (collides && (mostThreatening.getRadius() == 0 ||
			MathUtility::distance(m_tankBase.getPosition(),obstacle.getPosition()) < MathUtility::distance(m_tankBase.getPosition(),mostThreatening.getPosition())))
		{
			mostThreatening = obstacle; //finding the new most threating obstacle 
		}
	}


	return mostThreatening;
}

void TankAi::adaptConeDisplay()
{
	//Update cone Vertex Positions  
	sf::Vector2f tankPos = m_tankBase.getPosition();
	m_leftConeArray[0].position = tankPos;
	m_rightConeArray[0].position = tankPos;

	//Full vision
	m_leftConeArray[1].position = tankPos + thor::rotatedVector(sf::Vector2f(m_patrolZoneSize, 0), (float)m_turretRotation - (m_patrolConeRange / 2));
	m_rightConeArray[1].position = tankPos + thor::rotatedVector(sf::Vector2f(m_patrolZoneSize, 0), (float)m_turretRotation + (m_patrolConeRange / 2));

	//Check For obstacles in the lines  
	//left line
	if (CollisionDetector::collisionLineWithObjects(tankPos, m_leftConeArray[1].position, m_wallSprites))
	{
		//collision in the line 
		m_leftConeArray[1].position = CollisionDetector::findCollisionPointLineWithObjects(tankPos, m_leftConeArray[1].position, m_wallSprites);
	}
	//right line 
	if (CollisionDetector::collisionLineWithObjects(tankPos, m_rightConeArray[1].position, m_wallSprites))
	{
		m_rightConeArray[1].position = CollisionDetector::findCollisionPointLineWithObjects(tankPos, m_rightConeArray[1].position, m_wallSprites);
	}

}

void TankAi::fire()
{
	Projectile* newProjectilePtr = m_projectilesPool.getProjectile();
	if (newProjectilePtr != nullptr)
	{
		newProjectilePtr->launch(m_tankBase.getPosition(), m_turretRotation, 350.0);
		m_projectilesPtr.push_back(newProjectilePtr);
	}
	else
	{
		std::cout << "Error no Projectile available" << std::endl;
	}
}


void TankAi::clearDependantObjects()
{
	//deleting the projectiles 
	for (Projectile* projPtr : m_projectilesPtr)
	{
		projPtr->setInactive();
		projPtr = nullptr;
	}
	m_projectilesPtr.clear();
}

void TankAi::setLightMode(LightMode mode)
{
	m_lightMode = mode;
	if (m_lightMode == LightMode::DAY)
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
void TankAi::initSprites()
{
	sf::Texture const& texture = ResourcesManager::getTexture(TexturesName::SPRITE_SHEET);

	// Initialise the tank base
	m_tankBase.setTexture(texture);
	sf::IntRect baseRect(103, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turret.setTexture(texture);
	sf::IntRect turretRect(122, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
}


////////////////////////////////////////////////////////////
void TankAi::updateMovement(double dt)
{
	double speed = thor::length(m_velocity);
	sf::Vector2f newPos(m_tankBase.getPosition().x + std::cos(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000),
		m_tankBase.getPosition().y + std::sin(MathUtility::DEG_TO_RAD  * m_rotation) * speed * (dt / 1000));
	m_tankBase.setPosition(newPos.x, newPos.y);
	m_tankBase.setRotation(m_rotation);
	m_turret.setPosition(m_tankBase.getPosition());
	m_turret.setRotation(m_turretRotation);
}