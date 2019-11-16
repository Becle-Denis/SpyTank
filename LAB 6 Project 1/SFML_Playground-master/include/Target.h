#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include "CollisionDetector.h"

enum TargetState { DeadByTime, DeadByHit, NotStarted, NotDisplayed, OnScreen, Blinking };

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
	/// <returns>Remaining Time on screen</returns>
	sf::Time hit();


	/// <summary>
	/// return True if the target and the sprite are colliding and the Target is on screen
	/// </summary>
	/// <param name="sprite">sprite to tet collision with</param>
	/// <returns>True if collision</returns>
	bool isColliding(sf::Sprite const& sprite);

	/// <summary>
	/// Display the target on screen and add the bonus time 
	/// </summary>
	void reveal(sf::Time bonusTime = sf::seconds(0));

private:
	// the sprite of the Target, this will store the position and rotation of the Target
	sf::Sprite m_sprite;

	//The reference of the texture 
	sf::Texture const& m_texture;

	//Timer to deal with time on screen and time to display 
	thor::Timer m_timer;

	//Timer for blinking
	thor::Timer m_blinkingTimer;

	//Simply storage values for times 
	sf::Time m_timeOnScreen;
	sf::Time m_timeToStart;
	static const sf::Time S_BLINKING_TIME;

	// actual state of the Target 
	TargetState m_state;

	// boolean for displaying the Target 
	bool m_isDisplayed;
};

