#include "Game.h"
#include <iostream>

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_height, ScreenSize::s_width, 32), "SFML Playground", sf::Style::Default)
{
	m_window.setVerticalSyncEnabled(true);
	//myTexture was destroyed at the end of the function so it can't be displayed 
	//To fix this issue, the Game member variable m_myTexture was created  
	if (!m_myTexture.loadFromFile("E-100.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	m_sprite.setTexture(m_myTexture);

	//the origin of the sprite is set to the it center, is position is a the top left corner so 3/4 of the sprite is drawn outside the window 
	m_sprite.setOrigin(m_myTexture.getSize().x / 2, m_myTexture.getSize().y / 2);
	
	//changing the position to be visible 
	m_sprite.setPosition(200, 200);

	//rotation of the sprite 
	m_sprite.rotate(90);
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();

		processEvents();

		while (lag > MS_PER_UPDATE)
		{
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}
		update(MS_PER_UPDATE);

		render();
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

////////////////////////////////////////////////////////////
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	if (sf::Event::KeyPressed == event.type)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			m_window.close();
			break;
		case sf::Keyboard::Up:
			// Up key was pressed...
			break;
		default:
			break;
		}
	}
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{

}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));

	m_window.draw(m_sprite);
	

	m_window.display();
}





