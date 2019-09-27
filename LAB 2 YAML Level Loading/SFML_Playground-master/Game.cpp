#include "Game.h"
#include <iostream>

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_height, ScreenSize::s_width, 32), "SFML Playground", sf::Style::Default)
{
	//loadiibg the level data from the YAML file corresponding
	int currentLevel = 1;
	// Will generate an exception if level loading fails.
	try
	{
		LevelLoader::load(currentLevel, m_level);
	}
	catch (std::exception& e)
	{
		std::cout << "Level Loading failure." << std::endl;
		std::cout << e.what() << std::endl;
		throw e;
	}

	m_window.setVerticalSyncEnabled(true);

	//loading the backgroud texture to the backgroud sprite 
	if (!m_bgTexture.loadFromFile("Background.jpg"))
	{
		std::string s("Error loading background texture");
		throw std::exception(s.c_str());
	}
	m_bgSprite.setTexture(m_bgTexture);

	//loading the tank texture to the tank sprite 
	if (!m_tankTexture.loadFromFile("E-100.png"))
	{
		std::string s("Error loading tank texture");
		throw std::exception(s.c_str());
	}
	m_tankSprite.setTexture(m_tankTexture);
	m_tankSprite.setOrigin(m_tankTexture.getSize().x / 2, m_tankTexture.getSize().y / 2);

	//set the position of the tank according to the level data 
	m_tankSprite.setPosition(m_level.m_tank.m_position);

	if (!m_spriteSheetTexture.loadFromFile("SpriteSheet.png"))
	{
		std::string errorMsg("Error loading texture");
		throw std::exception(errorMsg.c_str());
	}

	// loading the sprites texture of the wall and creating wall obastacle sprite
	// Extract the wall image from the spritesheet.
	sf::Sprite sprite;
	sf::IntRect wallRect(2, 129, 33, 23);
	sprite.setTexture(m_spriteSheetTexture);
	sprite.setTextureRect(wallRect);
	// Loop through each Obstacle instance - recall that Obstacles are structs
	for (auto& obstacle : m_level.m_obstacles)
	{
		// Position the wall sprite using the obstacle data
		sprite.setPosition(obstacle.m_position);
		sprite.rotate(obstacle.m_rotation);
		m_sprites.push_back(sprite);
	}
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
	//drawing the backgroud
	m_window.draw(m_bgSprite);

	//drawing the obstacles 
	for (sf::Sprite sprite : m_sprites)
	{
		m_window.draw(sprite);
	}

	//drawing the tank 
	m_window.draw(m_tankSprite);
	
	m_window.display();
}





