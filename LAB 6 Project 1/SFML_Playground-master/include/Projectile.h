#pragma once
#include <SFML/Graphics.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include "Target.h"

/// <summary>
/// @brief Projectile class 
/// 
/// Manage projectile displacement 
/// </summary>
class Projectile
{
public:
	void launch(sf::Vector2f position, double rotation);

	void setSprite(sf::Texture const* texturePtr);
	
	void setInactive();

	bool isActive();

	//update the displacement of the projectile 
	void update(double dt);

	//check for collision on wall, or projectile outside the map
	bool isAlive(std::vector<sf::Sprite>& wallSprites);

	//check for collision on Target 
	//return true if the projectile hit a target 
	//Call hit() on the hitted target and reveal on the next target
	bool hitTarget(std::vector<Target>& targets);

	//draw the projectile
	void render(sf::RenderWindow& window);
private:
	
	//sprite of the projectile 
	sf::Sprite m_projectileSprite;

	//texture
	sf::Texture const* m_texturePtr;

	// The tank speed.
	static double s_projectile_speed;

	// The rotation of the projectile.
	double m_rotation{ 0.0 };

};



