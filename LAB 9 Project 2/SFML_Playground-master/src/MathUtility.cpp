#include "MathUtility.h"

namespace MathUtility
{
	////////////////////////////////////////////////////////////
	double distance(sf::Vector2f v1, sf::Vector2f v2)
	{
		return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
	}

	////////////////////////////////////////////////////////////
	bool lineIntersectsCircle(sf::Vector2f ahead, sf::Vector2f halfAhead, sf::CircleShape circle)
	{
		return distance(circle.getPosition(), ahead) <= circle.getRadius() ||
			distance(circle.getPosition(), halfAhead) <= circle.getRadius();
	}

	////////////////////////////////////////////////////////////
	sf::Vector2f truncate(sf::Vector2f v, float const max)
	{		
		if (thor::length(v) > max)
		{
			v = thor::unitVector(v);			
			v *= max;
		}	

		return v;
	}

	int pointPositionToLine(sf::Vector2f lineStartPoint, sf::Vector2f lineEndPoint, sf::Vector2f point)
	{
		return (((lineEndPoint.x - lineStartPoint.x) * (point.y - lineStartPoint.y)) - ((lineEndPoint.y - lineStartPoint.y) * (point.x - lineStartPoint.x))) ;
	}

}
