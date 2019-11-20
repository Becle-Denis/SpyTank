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
	TargetManager(sf::Texture const& targetTexture); //Create all the target
	~TargetManager(); // Delete all the target 

	void construct(std::vector<TargetData>& targetData);

	void update(double dt);

	int checkForCollision(sf::Sprite& const sprite); 
	void hit(int index);

	void render(sf::RenderWindow& window);

private: // functions 
	bool isOneOnScreen();
	int getNextIndex();
};

