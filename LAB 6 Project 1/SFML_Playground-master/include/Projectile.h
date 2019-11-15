#pragma once
#include <SFML/Graphics.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"

/// <summary>
/// @brief Projectile class 
/// 
/// Manage projectile displacement 
/// </summary>
class Projectile
{
public:
	Projectile(sf::Texture const& texture, sf::Vector2f position, double rotation);

	//copy contructor, needed for operator= 
	Projectile(const Projectile & p);

	//=operator, needed for vector::erase, because of const menber texture
	Projectile& operator=(Projectile const& rhs);

	//update the displacement of the projectile 
	void update(double dt);

	//check for collision on wall, or projectile outside the map
	bool isAlive(std::vector<sf::Sprite>& wallSprites);

	//draw the projectile
	void render(sf::RenderWindow& window);
private:
	
	//sprite of the projectile 
	sf::Sprite m_projectileSprite;

	//texture
	sf::Texture const& m_texture;

	// The tank speed.
	static double s_projectile_speed;

	// The rotation of the projectile.
	double m_rotation{ 0.0 };

};



