#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include "CollisionDetector.h"

/// <summary>
/// @brief A class for target to be hit by the player 
/// 
/// The class will manage Time of apperance of the sprite 
/// </summary>
class Target
{
public:
	Target(sf::Texture const& texture,sf::Vector2f position,sf::Time timeToStart, sf::Time timeOnScreen);

	/// <summary>
	/// Update the state of the Target 
	/// The first Update will launch the timer
	/// </summary>
	void update();

	void render(sf::RenderWindow& window);

	/// <summary>
	/// Change the state of the Target when hitting by a Tank
	/// </summary>
	void hit();

	/// <summary>
	/// Return the life state of the Target
	/// Dead only if displayed and hit or diseaper 
	/// </summary>
	/// <returns>Return true if the actual Target is or will be displayed on screen</returns>
	bool isAlive();

	/// <summary>
	/// return True if the target and the sprite are colliding and the Target is on screen
	/// </summary>
	/// <param name="sprite">sprite to tet collision with</param>
	/// <returns>True if collision</returns>
	bool isColliding(sf::Sprite const& sprite);

private:
	// the sprite of the Target, this will store the position and rotation of the Target
	sf::Sprite m_sprite;

	//The reference of the texture 
	sf::Texture const& m_texture;

	//Timer to deal with time on screen and time to display 
	thor::Timer m_timer;

	//Simply storage values 
	sf::Time m_timeOnScreen;
	sf::Time m_timeToStart;


	// actual state of the Target 
	// 0 Not started
	// 1 Not Yet Displayed 
	// 2 On Screen 
	// -1 Dead (Time out)
	// -2 Dead (hitted by player)
	int m_state;
	
};

