#include "Game.h"
#include <iostream>
#include "Projectile.h"

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_height, ScreenSize::s_width, 32), "SFML Playground", sf::Style::Default),
	m_tank(m_tankTexture2, m_wallSprites), m_state(GameState::IN_PROGRESS)
{

	//seed the random 
	srand(time(nullptr));
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

	//loading the Target Texture
	if (!m_targetTexture.loadFromFile("./resources/images/Hoodblaster.png"))
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
	m_timerText.setPosition(450,50);

	// Set the tank position in one corner randmoly.
	switch (rand() % 4)
	{
	case 0:
		m_tank.setPosition(sf::Vector2f(150, 150));
		break;
	case 1:
		m_tank.setPosition(sf::Vector2f(ScreenSize::s_height - 150, 150));
		break;
	case 2:
		m_tank.setPosition(sf::Vector2f(150, ScreenSize::s_width - 150));
		break;
	case 3:
		m_tank.setPosition(sf::Vector2f(ScreenSize::s_height - 150, ScreenSize::s_width - 150));
		break;
	}

	// Construct the wall sprites
	generateWalls();

	// Construct the Target 
	generateTarget();
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;
	m_timerLeft.restart(sf::seconds(60.f));

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
void Game::generateTarget()
{
	for (TargetData const& target : m_level.m_targets)
	{
		Target t(m_targetTexture,target.m_position,sf::seconds(target.secondsToStart),sf::seconds(target.secondsOnScreen));
		m_targets.push_back(t);
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
	if (m_timerLeft.isExpired())
	{
		m_state = GameState::STOPPED;
	}
	else 
	{
		//updating the timer
		sf::Time timeLeft = m_timerLeft.getRemainingTime();
		m_timerText.setString(""+std::to_string((int) timeLeft.asSeconds())+" seconds left");

		//updating the tank 
		m_tank.update(dt);

		//updating the targets
		for (Target& target : m_targets)
		{
			target.update();
		}
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

	//drawing the target
	for (Target& target : m_targets)
	{
		target.render(m_window);
	}

	m_window.display();
}





