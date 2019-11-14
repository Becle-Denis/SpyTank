#include "Game.h"
#include <iostream>
#include "Projectile.h"

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_height, ScreenSize::s_width, 32), "SFML Playground", sf::Style::Default),
	m_tank(m_tankTexture2,m_wallSprites),m_state(GameState::IN_PROGRESS)
{
	//loading the level data from the YAML file corresponding
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

	m_window.setVerticalSyncEnabled(true); // limit the framerate of the window 
	
	//loading the backgroud texture to the backgroud sprite 
	if (!m_bgTexture.loadFromFile(m_level.m_background.m_fileName))
	{
		std::string s("Error loading background texture");
		throw std::exception(s.c_str());
	}
	m_bgSprite.setTexture(m_bgTexture);
	
	
	//loading the tank texture 
	if (!m_tankTexture2.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	//loading the sprite sheet texture 
	if (!m_spriteSheetTexture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading texture");
		throw std::exception(s.c_str());
	}

	//loading the font 
	if (!m_fontA.loadFromFile("./resources/fonts/8_bit_fortress.ttf"))
	{
		std::cout << "problem loading font file" << std::endl;
	}
	m_timerText = sf::Text("",m_fontA,50);
	m_timerText.setFillColor(sf::Color::Red);
	m_timerText.setPosition(400,80);

	// Now the level data is loaded, set the tank position.
	m_tank.setPosition(m_level.m_tank.m_position);

	// Construct the wall sprites
	generateWalls();
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;
	m_timeLeft = 60;

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
void Game::generateWalls()
{
	sf::IntRect wallRect(2, 129, 33, 23);
	// Create the Walls 
	for (ObstacleData const& obstacle : m_level.m_obstacles)
	{
		sf::Sprite sprite;
		sprite.setTexture(m_spriteSheetTexture);
		sprite.setTextureRect(wallRect);
		sprite.setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);
		m_wallSprites.push_back(sprite);
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

		default:
			break;
		}
	}
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	if (m_timeLeft <= 0)
	{
		m_state = GameState::STOPPED;
	}

	if (m_state == GameState::IN_PROGRESS)
	{
		m_timeLeft -= dt / 1000;
		m_timerText.setString("Time left "+std::to_string((int) m_timeLeft)+" seconds");
		m_tank.update(dt);
	}
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	
	//drawing the backgroud
	m_window.draw(m_bgSprite);

	//drawing the timer
	m_window.draw(m_timerText);

	//drawing the wall
	for (sf::Sprite const & wallSprite : m_wallSprites)
	{
		m_window.draw(wallSprite);
	}

	//drawing the tank 
	m_tank.render(m_window);

	
	m_window.display();
}





