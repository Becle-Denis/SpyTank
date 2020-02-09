#include "TankAi.h"

////////////////////////////////////////////////////////////
TankAi::TankAi(sf::Texture const & texture, std::vector<sf::Sprite> & wallSprites)
	: m_texture(texture)
	, m_wallSprites(wallSprites)
	, m_steering(0, 0)
	, m_state(AIState::PATROL_MAP)
	, m_leftConeArray(sf::Lines,2)
	, m_rightConeArray(sf::Lines,2)
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

	//Vertex Array Color 
	m_leftConeArray[0].color = sf::Color(0, 150, 0,255);
	m_rightConeArray[0].color = sf::Color(0, 150, 0, 255);
	m_leftConeArray[1].color = sf::Color(0, 205, 0, 50);
	m_rightConeArray[1].color = sf::Color(0, 205, 0, 50);
}

////////////////////////////////////////////////////////////
void TankAi::start()
{
	m_tankBase.setPosition(m_startingPosition);
	m_turret.setPosition(m_startingPosition);
	m_tankBase.setRotation(0);
	m_turret.setRotation(0);
	m_lifePoint = LIFE_POINTS;
	m_rotation = 0;

	m_state = AIState::ATTACK_PLAYER;
	m_patrolPointIndex = rand() % m_patrolPoint.size();
}

////////////////////////////////////////////////////////////
void TankAi::update(Tank const& playerTank, double dt)
{
	sf::Vector2f destination;
	if (m_state == AIState::PATROL_MAP)
	{
		destination = m_patrolPoint.at(m_patrolPointIndex) - m_tankBase.getPosition();
		m_turretRotation = m_turretRotation + PATROL_ROTATION_SPEED * dt;
		if (m_turretRotation > 360)
		{
			m_turretRotation -= 360;
		}
		if (thor::length(destination) < MAX_SEE_AHEAD)
		{
			m_patrolPointIndex = rand() % m_patrolPoint.size();
		}
	}

	if (m_state == AIState::ATTACK_PLAYER)
	{
		destination = seek(playerTank.getPosition());
		
		m_turretRotation = m_rotation;
	}


	//Tank Movement
	sf::Vector2f acceleration;
	m_steering += thor::unitVector(destination);
	m_steering += collisionAvoidance();
	m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
	// calculating acceleration with the mass 
	acceleration = m_steering / MASS;
	m_velocity = MathUtility::truncate(m_velocity + acceleration, MAX_SPEED);
	

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
	//Update cone Vertex Positions  
	sf::Vector2f tankPos = m_tankBase.getPosition();
	m_leftConeArray[0].position = tankPos;
	m_rightConeArray[0].position = tankPos;

	m_leftConeArray[1].position = tankPos + thor::rotatedVector(sf::Vector2f(PATROL_ZONE_SIZE,0),(float) m_turretRotation - (m_patrolConeRange / 2));
	m_rightConeArray[1].position = tankPos + thor::rotatedVector(sf::Vector2f(PATROL_ZONE_SIZE, 0), (float)m_turretRotation + (m_patrolConeRange / 2));
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
	// TODO: Don't draw if off-screen...
	window.draw(m_tankBase);

	window.draw(m_leftConeArray);
	window.draw(m_rightConeArray);

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
std::pair<sf::Sprite, sf::Sprite> TankAi::getSprites()
{
	return std::pair<sf::Sprite, sf::Sprite>(m_tankBase,m_turret);
}

////////////////////////////////////////////////////////////
void TankAi::takeDamage(float damage)
{
	if (damage > 0)
	{
		m_lifePoint -= damage;
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

////////////////////////////////////////////////////////////
float TankAi::getLifePoint() const
{
	return m_lifePoint;
}

////////////////////////////////////////////////////////////
void TankAi::initSprites()
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(103, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);

	// Initialise the turret
	m_turret.setTexture(m_texture);
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