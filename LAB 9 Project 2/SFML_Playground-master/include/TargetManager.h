#pragma once
#include "Target.h"
#include "LevelLoader.h"

/// <summary>
/// @brief A class for store and Manage all the target of a Level
///
/// </summary>
class TargetManager
{
private: // Attributes 
	std::vector<Target> m_targets; // targets storage
	int m_numberMinOfTargets; //minimum value for easy mode; 

	int m_numberofActiveTargets; 


public: // functions 
	TargetManager(); //Construct the manager
	~TargetManager(); // Delete all the target 

	/// <summary>
	/// Create all the target according to the data
	/// also delete the previous target if the function is call twice or more
	/// </summary>
	/// <param name="targetData">Data of the target to be created</param>
	void construct(std::vector<TargetData>& targetData, int minNumberOfTarget, SoundManager* soundManager = nullptr);


	/// <summary>
	/// Start each Target
	/// </summary>
	void start(bool timed, sf::Texture const& targetTexture, bool smallNumber = false);

	/// <summary>
	/// Update all the targets
	/// </summary>
	/// <param name="dt">delta time between the last update</param>
	void update(double dt);

	/// <summary>
	/// Check if the sprite is colliding with one Target 
	/// </summary>
	/// <param name="sprite">Sprite to check</param>
	/// <returns>the index of the Target colliding with the sprite, - 1 if no collision</returns>
	int checkForCollision(sf::Sprite const& sprite) const; 
	
	/// <summary>
	/// Hit the target corresponding to the index, 
	/// Reveal the next target to be on screen if there is no more target on screen
	/// </summary>
	/// <param name="index">Index of the target to hit </param>
	void capture(int index, bool hitted);

	/// <summary>
	/// Draw all the targets
	/// </summary>
	/// <param name="window">Window on which the target will be drawn</param>
	void render(sf::RenderWindow& window) const;

	/// <summary>
	/// Reveal the result of all the targets 
	/// </summary>
	void revealResult();

	/// <summary>
	/// Return the number of targets 
	/// </summary>
	/// <returns>Number of targets</returns>
	int getNumberOfActiveTarget() const;

private: // functions 
	/// <summary>
	/// retrun true is there is One or More targets displayed on screen
	/// </summary>
	bool isOneOnScreen() const;

	/// <summary>
	/// Calculate the next target waiting to be displayed on screen
	/// </summary>
	/// <returns>the index of the next target, -1 if there is no more target</returns>
	int getNextIndex() const;
};

