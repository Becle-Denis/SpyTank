#pragma once
#include <SFML/Graphics.hpp>
#include "MathUtility.h"

/// <summary>
/// @brief Projectile class 
/// 
/// Manage projectile displacement 
/// </summary>
class Projectile
{
public:
	Projectile(sf::Texture& texture, sf::Vector2f position, double rotation);

	//update the displacement of the projectile 
	void update(double dt);

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

