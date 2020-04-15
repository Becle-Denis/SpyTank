#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include "CollisionDetector.h"
#include "SoundManager.h"
#include "ResourcesManager.h"

enum TargetState { DeadByTime, DeadByHit, NotInitialised, NotStarted, NotDisplayed, OnScreen, BlinkingOnScreen };

/// <summary>
/// @brief A class for target to be hit by the player 
/// 
/// The class will manage Time of apperance of the sprite 
/// </summary>
class Target
{
public:
	Target();

	/// <summary>
	/// Initialise the target, set the Sprite qnd position
	/// </summary>
	/// <param name="texture">Texture of the Target</param>
	/// <param name="position">Position of the target</param>
	/// <param name="timeToStart">Time of first apperance of the target</param>
	/// <param name="timeOnScreen">Time of the target to be on screen</param>
	void init(sf::Vector2f position, sf::Time timeToStart, sf::Time timeOnScreen, SoundManager* soundManager = nullptr);


	/// <summary>
	/// Launch the timer
	/// </summary>
	void start(bool timed = true);

	/// <summary>
	/// Update the state of the Target 
	/// The first Update will launch the timer
	/// </summary>
	void update();

	/// <summary>
	/// Draw the target if it should be drawn (Status onScreen)
	/// </summary>
	/// <param name="window"></param>
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

	/// <summary>
	/// Make the Target visible qnd colored
	/// Green if was dead by hit 
	/// Red else
	/// </summary>
	void setResult();

	/// <summary>
	/// Check the state of the Target 
	/// </summary>
	/// <returns>True if the target is Onscreen or Blinking</returns>
	bool isOnScreen();

	/// <summary>
	/// Check the state of the Target, and eventually indicate the waintingTime
	/// </summary>
	/// <param name="waitingTime">Return Reference, Time left to display the target</param>
	/// <returns>True if the state is notDisplayed</returns>
	bool waintingToBeDisplayed(sf::Time& waitingTime);

	sf::Vector2f getPosition();

private:
	// the sprite of the Target, this will store the position and rotation of the Target
	sf::Sprite m_sprite;

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

	// boolean to store if the target is timed or not
	bool m_timed;

	//soundManager
	SoundManager* m_soundManagerPtr;
};

