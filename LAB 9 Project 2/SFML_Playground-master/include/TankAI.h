#pragma once
#include "MathUtility.h"
#include "CollisionDetector.h"
#include "Projectile.h"
#include "ProjectilePool.h"
#include "ResourcesManager.h"
#include "GameState.h"
#include "SoundManager.h"
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <Thor/Time/Timer.hpp>
#include <iostream>
#include <queue>

class Tank; //Forward Reference

class TankAi
{
public:
	/// <summary>
	/// @brief Constructor that stores a reference to the obstacle container.
	/// Initialises steering behaviour to seek (player) mode, sets the AI tank position and
	///  initialises the steering vector to (0,0) meaning zero force magnitude.
	/// </summary>
	/// <param name="texture">A reference to the sprite sheet texture</param>
	///< param name="wallSprites">A reference to the container of wall sprites</param>
	TankAi(std::vector<sf::Sprite> & wallSprites, std::map<int, std::list< sf::Sprite> >& wallMap, ProjectilePool & projectilesPool, SoundManager & soundManager);

	void start();

	/// <summary>
	/// @brief Steers the AI tank towards the player tank avoiding obstacles along the way.
	/// Gets a vector to the player tank and sets steering and velocity vectors towards
	/// the player if current behaviour is seek. If behaviour is stop, the velocity vector
	/// is set to 0. Then compute the correct rotation angle to point towards the player tank. 
	/// If the distance to the player tank is < MAX_SEE_AHEAD, then the behaviour is changed from seek to stop.
	/// Finally, recalculate the new position of the tank base and turret sprites.
	/// </summary>
	/// <param name="playerTank">A reference to the player tank</param>
	/// <param name="dt">update delta time</param>
	void update(Tank & playerTank, double dt);

	/// <summary>
	/// @brief Checks for collision between the AI and player tanks.
	///
	/// </summary>
	/// <param name="player">The player tank instance</param>
	/// <returns>True if collision detected between AI and player tanks.</returns>
	bool collidesWithPlayer(Tank const& playerTank) const;

	/// <summary>
	/// @brief Draws the tank base and turret.
	///
	/// </summary>
	/// <param name="window">The SFML Render window</param>
	void render(sf::RenderWindow & window);

	/// <summary>
	/// @brief Initialises the obstacle container and sets the tank base/turret sprites to the specified position.
	/// <param name="position">An x,y position</param>
	/// </summary>
	void init(sf::Vector2f position);

	void clearDependantObjects();

	/// <summary>
	/// Set the Tank Light Mode
	/// </summary>
	void setLightMode(LightMode mode);

private:
	void initSprites();

	void updateMovement(double dt);

	sf::Vector2f seek(sf::Vector2f playerPosition) const;

	sf::Vector2f collisionAvoidance();

	const sf::CircleShape findMostThreateningObstacle();

	void adaptConeDisplay();

	// A sprite for the tank base.
	sf::Sprite m_tankBase;

	// A sprite for the turret
	sf::Sprite m_turret;

	// A reference to the container of wall sprites.
	std::vector<sf::Sprite> & m_wallSprites;
	std::map<int, std::list< sf::Sprite> > & m_wallSpatialMap;

	// A reference to the sound manager
	SoundManager& m_soundManager;

	// The current rotation as applied to tank base
	double m_rotation{ 0.0 };
	
	// The current rotation as applied to tank turret
	double m_turretRotation{ 0.0 };

	// Current velocity.
	sf::Vector2f m_velocity;

	// Steering vector.
	sf::Vector2f m_steering;

	// The ahead vector.
	sf::Vector2f m_ahead;

	// The half-ahead vector.
	sf::Vector2f m_halfAhead;

	// The maximum see ahead range.
	static float constexpr MAX_SEE_AHEAD{ 50.0f };

	// The maximum avoidance turn rate.
	static float constexpr MAX_AVOID_FORCE{ 50.0f };

	// maximum steering force of the tank
	static float constexpr MAX_FORCE{ 10.0f };

	// the mass of the tank, used for acceleration calculation 
	static float constexpr MASS{ 2.0f };

	static float constexpr PATROL_ROTATION_SPEED{ 0.03f };

	static float constexpr MAX_PATROL_ZONE_SIZE{ 550.0f };

	static float constexpr MIN_PATROL_ZONE_SIZE{ 300.0f };

	static float constexpr MIN_PATROL_CONE_RANGE{ 65.f };

	static float constexpr MAX_PATROL_CONE_RANGE{ 120.f };

	static float constexpr PATROL_CONE_RANGE_SPEED{ 0.05f };
	
	static float constexpr PATROL_CONE_ZONE_SPEED{ 0.05f };

	static const sf::Time FIRE_RELOAD_TIME;

	// The maximum speed for this tank.
	static float constexpr SPEED_ATTACK{ 75.0f };
	static float constexpr SPEED_NORMAL{ 40.0f };

	//Original Starting position 
	sf::Vector2f m_startingPosition;

	// A container of circles that represent the obstacles to avoid.
	std::vector<sf::CircleShape> m_obstacles;

	unsigned int m_patrolPointIndex{ 0 };
	std::vector<sf::Vector2f> m_patrolPoint;

	float m_patrolConeRange;
	float m_patrolZoneSize;

	sf::VertexArray m_leftConeArray;
	sf::VertexArray m_rightConeArray;

	enum class AIState
	{
		PATROL_MAP,
		ATTACK_PLAYER
	} m_state;

	LightMode m_lightMode;

	ProjectilePool& m_projectilesPool; //Reference to the pool of projectile 
	std::vector<std::shared_ptr<Projectile>> m_projectilesPtr; // container of the tank projectile (pointers)

	thor::Timer m_fireTimer; // Timer for the time between two fire

	//fire a projectile int he direction of the turret
	void fire();

};

#include "Tank.h"