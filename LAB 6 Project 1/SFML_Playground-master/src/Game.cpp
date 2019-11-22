#include "Game.h"
#include <iostream>
#include "Projectile.h"

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_height, ScreenSize::s_width, 32), "SFML Playground", sf::Style::Default),
	m_targets(m_targetTexture),
	m_tank(m_spriteSheetTexture, m_wallSprites, m_targets, m_projectilesPool),
	m_state(GameState::NOT_STARTED), m_projectilesPool(m_spriteSheetTexture,10)
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

	//loading the sprite sheet texture 
	if (!m_spriteSheetTexture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s("Error loading Sprite sheet texture");
		throw std::exception(s.c_str());
	}

	//loading the Target Texture
	if (!m_targetTexture.loadFromFile("./resources/images/RazorTarget1.png"))
	{
		std::string s("Error loading Target texture");
		throw std::exception(s.c_str());
	}

	//loading the Smoked Texture
	if (!m_smokedScreenTexture.loadFromFile("./resources/images/fumee.jpg"))
	{
		std::string s("Error loading Smoked texture");
		throw std::exception(s.c_str());
	}
	m_smokedSprite.setTexture(m_smokedScreenTexture);
	m_smokedSprite.setTextureRect(sf::IntRect(0, 0, 960, 540));
	m_smokedSprite.setScale(1.7, 1.7);
	m_smokedSprite.setColor(sf::Color(255, 255, 255, 150));

	//loading the font 
	if (!m_fontA.loadFromFile("./resources/fonts/8_bit_fortress.ttf"))
	{
		std::cout << "problem loading font file" << std::endl;
	}
	m_bigDisplayedText = sf::Text("BLIND TANK",m_fontA,50);
	m_bigDisplayedText.setFillColor(sf::Color::Red);
	m_bigDisplayedText.setPosition(450,50);

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

	//Constrcut the targets
	m_targets.construct(m_level.m_targets);
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
		case sf::Keyboard::Enter:
			if (m_state == GameState::NOT_STARTED)
			{
				start();
			}
		default:
			break;
		}
	}
}

void Game::setGameOver()
{
	m_state = GameState::OVER;
	m_targets.revealResult();
	m_bigDisplayedText.setString("Game Over !");

	TankPerformance perf = m_tank.getPerformance();

	std::string title = "\nHit\nTotal Targets\nSuccess\nFired\nAccuracy";
	std::string actualStats = "Player\n"
		+ std::to_string(perf.targetHitted) + "\n" 
		+ std::to_string(perf.totalNumberOfTarget) + "\n" 
		+ std::to_string(perf.sucess) + " %" + "\n"
		+ std::to_string(perf.projectileFired) + "\n"
		+ std::to_string(perf.accuracy) + " %";
	std::string bestStats = "Best\n"
		+ std::to_string(perf.targetHitted) + "\n"
		+ std::to_string(perf.totalNumberOfTarget) + "\n"
		+ std::to_string(perf.sucess) + " %" + "\n"
		+ std::to_string(perf.projectileFired) + "\n"
		+ std::to_string(perf.accuracy) + " %";

	m_gameOverStatsText = sf::Text(title, m_fontA, 60);
	m_gameOverStatsText.setFillColor(sf::Color::Black);
	m_gameOverStatsText.setPosition(100, 200);

	m_gameOverPLayerStatsText = sf::Text(actualStats, m_fontA, 60);
	m_gameOverPLayerStatsText.setFillColor(sf::Color(168,152,0));
	m_gameOverPLayerStatsText.setPosition(700, 200);

	m_gameOverBestStatsText = sf::Text(bestStats, m_fontA, 60);
	m_gameOverBestStatsText.setFillColor(sf::Color(0, 0, 150));
	m_gameOverBestStatsText.setPosition(1000, 200);

	std::cout << actualStats << std::endl;
}

void Game::start()
{
	m_state = GameState::IN_PROGRESS;
	m_timerLeft.restart(sf::seconds(5.f));
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	if (m_state == GameState::IN_PROGRESS)
	{
		if (m_timerLeft.isExpired())
		{
			setGameOver();
		}
		else
		{
			//updating the timer
			sf::Time timeLeft = m_timerLeft.getRemainingTime();
			m_bigDisplayedText.setString("" + std::to_string((int)timeLeft.asSeconds()) + " seconds left");

			//updating the targets
			m_targets.update(dt);

			//updating the tank 
			m_tank.update(dt);


		}
	}

}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	
	//drawing the backgroud
	m_window.draw(m_bgSprite);

	
	//drawing the wall
	for (sf::Sprite const & wallSprite : m_wallSprites)
	{
		m_window.draw(wallSprite);
	}

	//drawing the timer
	m_window.draw(m_bigDisplayedText);

	//drawing the target
	m_targets.render(m_window);

	//drawing the tank 
	m_tank.render(m_window);

	//Not Started 
	if (m_state == GameState::NOT_STARTED)
	{
		m_window.draw(m_smokedSprite); //smoked sprite
		m_window.draw(m_bigDisplayedText); // re drawing the game over text
	}

	// GameOver 
	if (m_state == GameState::OVER)
	{
		m_window.draw(m_smokedSprite); //smoked sprite
		m_window.draw(m_bigDisplayedText); // re drawing the game over text
		m_window.draw(m_gameOverStatsText); // Stats text
		m_window.draw(m_gameOverPLayerStatsText); // Actual stats
		m_window.draw(m_gameOverBestStatsText); // Best stats
	}

	m_window.display();
}





