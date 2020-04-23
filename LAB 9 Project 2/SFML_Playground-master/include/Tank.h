#pragma once
#include <SFML/Graphics.hpp>
#include "CollisionDetector.h"
#include "MathUtility.h"
#include "ProjectilePool.h"
#include "Projectile.h"
#include "TargetManager.h"
#include "UserPerformance.h"
#include "SoundManager.h"
#include <Thor/Time/CallbackTimer.hpp>
#include "MovingMotorEffect.h"
#include "GameState.h"
#include "HUD.h"
#include "ScreenSize.h"
#include "ResourcesManager.h"
#include "NoiseScreen.h"

/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
private: // Attributes

	//Outsides References
	std::vector<sf::Sprite>& m_wallSprites; // A reference to the container of wall sprites.	 
	TargetManager& m_targets; // A Reference to the container of Targets 
	ProjectilePool& m_projectilesPool; // Reference to the Pool of projectiles
	std::map<int, std::list< sf::Sprite> >& m_wallSpatialMap; // Reference to the Spatial map of projectiles
	SoundManager& m_soundManager; // Reference to the soundManager
	MovingMotorEffect* m_motorSound; // Pointer to the motor sound effect
	HUD& m_hud; // Reference to the HUD
	NoiseScreen& m_noiseScreen; // Reference to the noise screen  

	//Sprites 
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	
	//Tank fire ttributes 
	std::vector<std::shared_ptr<Projectile>> m_projectilesPtr; // container of the tank projectile (pointers)
	bool canFire{ true }; // True if the tank can fire
	thor::Timer m_fireTimer; // Timer for the time between two fire

	// Performances 
	UserPerformance m_performances;

	//Tank Movement attributes 
	// The tank speed.
	double m_speed{ 0.0 };

	// The current rotation as applied to tank base.
	double m_rotation{ 0.0 };

	// The current rotation to the turret 
	double m_turretRotation{ 0.0 };

	// The maximum speed of the tank
	double m_maximumSpeed{ 80 };

	// enable or disable the rotation of the tank to deal with collision
	bool m_enableRotation{ true };

	// the previous position of the tank sprite
	sf::Vector2f m_previousPosition;

	// the previous speed of the tank
	double m_previousSpeed{ 0.0 };

	// The previous rotation as applied to tank base.
	double m_previousRotation{ 0.0 };

	// The previous rotation to the turret 
	double m_previousTurretRotation{ 0.0 };

	//Original Starting position 
	sf::Vector2f m_startingPosition;

	//Actual gameplay of the tank
	GameState m_gameplay;

	static int constexpr LIFE_POINTS{ 5 };

	int m_lifePoint{ 5 };

	float m_rotationStep{ 1.0f };

	float m_decreasingFactor{ 0.99f };

	int m_rotationBug{ -1 };
	int m_rotationBugUpdateCount{ 0 };

	int m_capturedTarget{ 0 };

public:	//functions
	/// <summary>
	/// @brief Constructor that stores drawable state (texture, sprite) for the tank.
	/// Stores references to the texture and container of wall sprites. 
	/// Creates sprites for the tank base and turret from the supplied texture.
	/// </summary>
	/// <param name="texture">A reference to the sprite sheet texture</param>
	///< param name="texture">A reference to the container of wall sprites</param>  
	Tank(std::vector<sf::Sprite>& wallSprites, std::map<int, std::list< sf::Sprite> >& wallMap, TargetManager& targets, ProjectilePool& projectilePool, SoundManager& soundManager, HUD& hud, NoiseScreen& noiseScreen);

	void initialise(GameState gameRunningState);

	/// <summary>
	/// @brief Update the tank attributes and dependant objects(projectile) and handle the player input
	/// </summary>
	/// <param name="dt">Time elpased since the last update</param>
	void update(double dt);
	/// <summary>
	/// @brief Render the Tank and dependant object
	/// </summary>
	/// <param name="window">window where to draw the Tank</param>
	void render(sf::RenderWindow & window) const;

	/// <summary>
	/// @brief Set the Position of the tank
	/// </summary>
	/// <param name="pos">New position of the tank</param>
	void setStartingPosition(sf::Vector2f const& pos);

	/// <summary>
	/// Calculate and return the tank performances
	/// </summary>
	/// <returns>Actual tank performances</returns>
	UserPerformance& getPerformance();

	/// <summary>
	/// @brief Reads the player tank position.
	/// <returns>The tank base position.</returns>
	/// </summary>
	sf::Vector2f getPosition() const;

	/// <summary>
	/// @brief Return the tank base sprite 
	/// </summary>
	/// <returns>sf::Sprite Tank base Sprite</returns>
	sf::Sprite getBaseTank() const;

	/// <summary>
	/// @brief Return the tank turret sprite 
	/// </summary>
	/// <returns>sf::Sprite Tank turret Sprite</returns>
	sf::Sprite getTurret() const;

	void clearDependantObjects();
	
	/// <summary>
	/// Apply the damage to the tank health
	/// </summary>
	/// <param name="damage">damage applied to the tank</param>
	void takeImpact();

	/// <summary>
	/// Return the tank lifePoint
	/// </summary>
	/// <returns>Tank actual life Points</returns>
	bool isAlive() const;

	int getNumberOfCapturedTarget() const;

	/// <summary>
	/// Set the Tank Light Mode
	/// </summary>
	void setLightMode(LightMode mode);

private: // functions 
	/// <summary>
	/// @brief Initialise the tank and turret sprite 
	/// </summary>
	void initSprites();

	/// <summary>
	/// @brief Processes control keys and applies speed/rotation as appropriate.
	/// </summary>
	void handleKeyInput();

	/// <summary>
	/// @brief Increases the speed by 1, max speed is capped at 100
	/// </summary>
	void increaseSpeed();

	/// <summary>
	/// @brief Decreases the speed by 1, min speed is capped at -100.
	/// </summary>
	void decreaseSpeed();

	/// <summary>
	/// @brief Increases the rotation by 1 degree, wraps to 0 degrees after 359, also increase turret rotation.
	/// </summary>
	void increaseRotation();

	/// <summary>
	/// @brief Decreases the rotation by 1 degree, wraps to 359 degrees after 0, also decrease turret rotation.
	/// </summary>
	void decreaseRotation();

	/// <summary>
	/// @brief Increases the turret rotation by 1 degree,  wraps to 0 degrees after 359
	/// </summary>
	void increaseTurretRotation();

	/// <summary>
	/// @brief Decreases the turret rotation by 1 degree, wraps to 359 degrees after 0
	/// </summary>
	void decreaseTurretRotation();

	/// <summary>
	/// @brief Put the rotation of the turret at the center of the tank 
	/// </summary>
	void centerTurret();

	/// <summary>
	/// @brief Checks for collisions between the tank and the walls.
	/// </summary>
	/// <returns>True if collision detected between tank and wall.</returns>
	bool checkWallCollision();

	void limitInsideMap();

	/// <summary>
	/// @brief Stops the tank if moving and applies a small increase in speed in the opposite direction of travel.
	/// If the tank speed is currently 0, the rotation is set to a value that is less than the previous rotation value
	///  (scenario: tank is stopped and rotates into a wall, so it gets rotated towards the opposite direction).
	/// If the tank is moving, further rotations are disabled and the previous tank position is restored.
	/// The tank speed is adjusted so that it will travel slowly in the opposite direction. The tank rotation 
	///  is also adjusted as above if necessary (scenario: tank is both moving and rotating, upon wall collision it's 
	///  speed is reversed but with a smaller magnitude, while it is rotated in the opposite direction of it's 
	///  pre-collision rotation).
	/// </summary>
	void deflect();

	void adjustRotation();

	//fire a projectile 
	void fire();
};
