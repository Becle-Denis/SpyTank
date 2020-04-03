#include "CollisionDetector.h"
#include <iostream>

bool CollisionDetector::collision(const sf::Sprite& object1, const sf::Sprite& object2) {
	OrientedBoundingBox OBB1(object1);
	OrientedBoundingBox OBB2(object2);
	return collision(OBB1, OBB2);
}

bool CollisionDetector::collision(const sf::Shape& object1, const sf::Sprite& object2)
{
	OrientedBoundingBox OBB1(object1);
	OrientedBoundingBox OBB2(object2);
	return collision(OBB1, OBB2);
}

bool CollisionDetector::collision(const sf::Shape& object1, const sf::Shape& object2)
{
	OrientedBoundingBox OBB1(object1);
	OrientedBoundingBox OBB2(object2);
	return collision(OBB1, OBB2);
}

bool CollisionDetector::collisionLineWithObjects(sf::Vector2f const& pointA, sf::Vector2f const& pointB, std::vector<sf::Sprite> const& obstacles)
{
	//for this we will create a rectangle shape between the point A and B  
	//and check for collision between this and the obstacles.
	//creating the shape
	sf::RectangleShape targetLine;
	//set the rotation between the point A and B
	targetLine.setRotation(thor::polarAngle(pointB - pointA));
	//set the length according to the distance betwwen the points 
	targetLine.setSize(sf::Vector2f(MathUtility::distance(pointA, pointB), 2.0f));
	//set the position of the shape
	targetLine.setPosition(pointA);

	//check for collision between the shape and the obstacles 
	bool obstacleFound = false;
	for (sf::Sprite const& obstacle : obstacles)
	{
		//checking for collision 
		if (CollisionDetector::collision(targetLine, obstacle))
		{
			obstacleFound = true;
			break;
		}
	}

	return obstacleFound;
}

sf::Vector2f CollisionDetector::findCollisionPointLineWithObjects(sf::Vector2f pointA, sf::Vector2f pointB, std::vector<sf::Sprite> const& obsctacles, int precision)
{	
	//For this we will frame the collision point by checking in wich half the collision happen 
	//We will repeat the process with the new lower and higher value. 
	for (int i = 0; i < precision; i++)
	{
		findHalfCollidingWithLine(pointA, pointB, obsctacles);
	}
	return pointB;
}



void CollisionDetector::findHalfCollidingWithLine(sf::Vector2f& pointA, sf::Vector2f& pointB, std::vector<sf::Sprite> const& obsctacles)
{
	//calculating the mid point 
	sf::Vector2f midPoint;
	midPoint.x = (pointA.x + pointB.x) / 2;
	midPoint.y = (pointA.y + pointB.y) / 2;
	if (collisionLineWithObjects(pointA, midPoint, obsctacles))
	{
		//collision in the first half
		pointB = midPoint;
	}
	else
	{
		//collision in the second half
		pointA = midPoint;
	}
}

bool CollisionDetector::collision(OrientedBoundingBox& OBB1, OrientedBoundingBox& OBB2)
{
	// Create the four distinct axes that are perpendicular to the edges of the two rectangles
	sf::Vector2f Axes[4] = {
		sf::Vector2f(OBB1.Points[1].x - OBB1.Points[0].x,
		OBB1.Points[1].y - OBB1.Points[0].y),
		sf::Vector2f(OBB1.Points[1].x - OBB1.Points[2].x,
		OBB1.Points[1].y - OBB1.Points[2].y),
		sf::Vector2f(OBB2.Points[0].x - OBB2.Points[3].x,
		OBB2.Points[0].y - OBB2.Points[3].y),
		sf::Vector2f(OBB2.Points[0].x - OBB2.Points[1].x,
		OBB2.Points[0].y - OBB2.Points[1].y)
	};

	for (int i = 0; i < 4; i++) // For each axis...
	{
		float MinOBB1, MaxOBB1, MinOBB2, MaxOBB2;

		// ... project the points of both OBBs onto the axis ...
		OBB1.ProjectOntoAxis(Axes[i], MinOBB1, MaxOBB1);
		OBB2.ProjectOntoAxis(Axes[i], MinOBB2, MaxOBB2);

		// ... and check whether the outermost projected points of both OBBs overlap.
		// If this is not the case, the Seperating Axis Theorem states that there can be no collision between the rectangles
		if (!((MinOBB2 <= MaxOBB1) && (MaxOBB2 >= MinOBB1)))
			return false;
	}

	return true;	// Collision detected!
}
