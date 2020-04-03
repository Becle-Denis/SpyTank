#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include "MathUtility.h"
#include "OrientedBoundingBox.h"

/// <summary>
/// @brief Collision detection class for bounding box collisions using the Separating Axis Theorem (SAT)
///
/// @authors Nick Koirala (original version), ahnonay (SFML2 compatibility)
/// 
/// (c) 2009 - LittleMonkey Ltd
/// 
/// Hosted on github at the following URL: 
///
///https://github.com/SFML/SFML/wiki/Source:-Simple-Collision-Detection-for-SFML-2
/// </summary>


class CollisionDetector
{
public:
	bool static collision(const sf::Sprite& object1, const sf::Sprite& object2);
	bool static collision(const sf::Shape& object1, const sf::Sprite& object2);
	bool static collision(const sf::Shape& object1, const sf::Shape& object2);

	//Return true if there is a collision with an obstacle between the two points 
	bool static collisionLineWithObjects(sf::Vector2f const& pointA, sf::Vector2f const& pointB, std::vector<sf::Sprite> const & obsctacles);
	
	//return the position of the collision with some precision, increasing the precision will take more time to compute but it will be more precise 
	//You must check first if there is a collision 
	sf::Vector2f static findCollisionPointLineWithObjects(sf::Vector2f pointA, sf::Vector2f pointB, std::vector<sf::Sprite> const& obsctacles, int precision = 5);

private:
	//Adjust the two point to find in which half of the line is the collision
	void static findHalfCollidingWithLine(sf::Vector2f& pointA, sf::Vector2f& pointB, std::vector<sf::Sprite> const& obsctacles);
	bool static collision(OrientedBoundingBox& OBB1, OrientedBoundingBox& OBB2);
};
