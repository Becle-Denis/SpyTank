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
	Target* m_targets; // Target storage array 
	int m_number_of_targets; // size of the array
	sf::Texture const& m_texture; //Texture for the targets 

public: // functions 
	TargetManager(sf::Texture const& targetTexture); //Construct the manager
	~TargetManager(); // Delete all the target 

	/// <summary>
	/// Create all the target according to the data
	/// also delete the previous target if the function is call twice or more
	/// </summary>
	/// <param name="targetData">Data of the target to be created</param>
	void construct(std::vector<TargetData>& targetData);

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
	int checkForCollision(sf::Sprite const& sprite); 
	
	/// <summary>
	/// Hit the target corresponding to the index, 
	/// Reveal the next target to be on screen if there is no more target on screen
	/// </summary>
	/// <param name="index">Index of the target to hit </param>
	void hit(int index);

	/// <summary>
	/// Draw all the targets
	/// </summary>
	/// <param name="window">Window on which the target will be drawn</param>
	void render(sf::RenderWindow& window);

	/// <summary>
	/// Reveal the rsult of all the targets 
	/// </summary>
	void revealResult();

	/// <summary>
	/// Return the number of targets 
	/// </summary>
	/// <returns>Number of targets</returns>
	int getNumberOfDisplayedTarget();

private: // functions 
	/// <summary>
	/// retrun true is there is One or More targets displayed on screen
	/// </summary>
	bool isOneOnScreen();

	/// <summary>
	/// Calculate the next target waiting to be displayed on screen
	/// </summary>
	/// <returns>the index of the next target, -1 if there is no more target</returns>
	int getNextIndex();
};

