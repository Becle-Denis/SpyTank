#pragma once
#include <SFML/Graphics.hpp>
#include "ScreenSize.h"

//this class will create and render a white noise screen 
//The transparency of the screen will be updatable 
class NoiseScreen
{
public: // public functions
	NoiseScreen();
	~NoiseScreen();

	void updateNoise();

	void cleanNoise();

	void update(double dt);

	void render(sf::RenderWindow& window) const;

private: //private functions

private: //private attibutes 
	
	std::vector<sf::CircleShape> m_points; //store all the points of the screen
	unsigned int m_activePoints;
	int m_alpha;

	static const unsigned int s_NUMBER_OF_POINTS; 
	static const unsigned int s_GAP; 
	static const int s_ALPHA_GAP;
};

