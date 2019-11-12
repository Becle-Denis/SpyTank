#pragma once
#include <SFML/Graphics.hpp>
#include "CollisionDetector.h"
#include "MathUtility.h"
#include "Projectile.h"
#include <Thor/Time/CallbackTimer.hpp>


/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:	
	/// <summary>
	/// @brief Constructor that stores drawable state (texture, sprite) for the tank.
	/// Stores references to the texture and container of wall sprites. 
	/// Creates sprites for the tank base and turret from the supplied texture.
	/// </summary>
	/// <param name="texture">A reference to the sprite sheet texture</param>
	///< param name="texture">A reference to the container of wall sprites</param>  
	Tank(sf::Texture const& texture, std::vector<sf::Sprite>& wallSprites);

	void update(double dt);
	void render(sf::RenderWindow & window);
	void setPosition(sf::Vector2f const& pos);

	

	
private:
	void initSprites();
	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	sf::Texture const & m_texture;

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

	// A reference to the container of wall sprites.
	std::vector<sf::Sprite>& m_wallSprites;

	// True if the tank can fire
	bool canFire{ true };

	// Timer for the time between two fire
	thor::Timer m_fireTimer;

	// container of the tank projectile
	std::vector<Projectile> m_projectiles;

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

