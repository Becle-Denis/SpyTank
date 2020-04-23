#include "NoiseScreen.h"

const unsigned int NoiseScreen::s_NUMBER_OF_POINTS = 1000;
const unsigned int NoiseScreen::s_GAP = 200;
const int NoiseScreen::s_ALPHA_GAP = 50;

NoiseScreen::NoiseScreen() :
	m_activePoints(0),
	m_alpha(0)
{
	m_points.resize(s_NUMBER_OF_POINTS);
	for (sf::CircleShape& shape : m_points)
	{
		//setting radius
		shape.setRadius(1.5f);

		//setting color 
		sf::Uint8 newWhiteColor = rand() % 255;
		shape.setFillColor(sf::Color(newWhiteColor, newWhiteColor, newWhiteColor));
	}

}

NoiseScreen::~NoiseScreen()
{
}

void NoiseScreen::updateNoise()
{
	//changing actives points 
	m_activePoints += s_GAP;
	if (m_activePoints > s_NUMBER_OF_POINTS)
	{
		m_activePoints = s_NUMBER_OF_POINTS;
	}

	//changing alpha 
	m_alpha += s_ALPHA_GAP;
	if (m_alpha > 255)
	{
		m_alpha = 255;
	}

	//setting alpha 
	sf::Uint8 alpha = static_cast<sf::Uint8>(m_alpha);
	for (sf::CircleShape& shape : m_points)
	{
		sf::Color color = shape.getFillColor();
		color.a = m_alpha;
		shape.setFillColor(color);
	}
}

void NoiseScreen::cleanNoise()
{
	m_activePoints = 0;
	m_alpha = 0;

	//setting alpha 
	sf::Uint8 alpha = static_cast<sf::Uint8>(m_alpha);
	for (sf::CircleShape& shape : m_points)
	{
		sf::Color color = shape.getFillColor();
		color.a = m_alpha;
		shape.setFillColor(color);
	}
}

void NoiseScreen::update(double dt)
{
	// updating each points 
	for (unsigned int i = 0; i < m_activePoints; i++)
	{
		//updating position 
		m_points.at(i).setPosition(rand() % ScreenSize::s_height, rand() % ScreenSize::s_width);
	}
}

void NoiseScreen::render(sf::RenderWindow& window) const
{
	for (unsigned int i = 0; i < m_activePoints; i++)
	{
		window.draw(m_points.at(i));
	}
}
