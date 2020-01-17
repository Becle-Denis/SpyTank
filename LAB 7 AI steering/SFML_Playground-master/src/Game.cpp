#include "Game.h"
#include <iostream>
#include "Projectile.h"

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_height, ScreenSize::s_width, 32), "SFML Playground", sf::Style::Default),
	m_targets(m_targetTexture),
	m_tank(m_spriteSheetTexture, m_wallSprites, m_wallSpatialMap, m_targets, m_projectilesPool, m_soundManager),
	m_aiTank(m_spriteSheetTexture, m_wallSprites),
	m_state(GameState::NOT_STARTED), m_projectilesPool(m_spriteSheetTexture, 10, &m_soundManager),
	m_soundManager("./resources/sounds/music/Level1v1.wav")
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

	//set the texts for realTime stats
	m_statTitleText = sf::Text("Hits\nSuccess\nAccuracy", m_fontA, 30);
	m_statTitleText.setFillColor(sf::Color(0, 0, 125));
	m_statTitleText.setPosition(150, 40);

	m_playerStatsText = sf::Text("", m_fontA, 30);
	m_playerStatsText.setFillColor(sf::Color(0, 0, 125));
	m_playerStatsText.setPosition(50, 40);

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

	//initialise the tank 
	m_tank.initialise();

	// Populate the obstacle list and set the AI tank position.
	m_aiTank.init(m_level.m_aiTank.m_position);

	//Constrcut the targets
	m_targets.construct(m_level.m_targets,&m_soundManager);
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	//sound stuff
	m_soundManager.startMenuMusic();

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
int Game::calculateSpatialMapCell(sf::Vector2f gamePosition)
{
	int numberOfspatialDivision = 10;
	return (floor(gamePosition.y / (ScreenSize::s_width / numberOfspatialDivision)) * numberOfspatialDivision) + (floor(gamePosition.x / (ScreenSize::s_height / numberOfspatialDivision))) ;
}

////////////////////////////////////////////////////////////
void Game::generateWalls()
{
	sf::IntRect wallRect(2, 129, 33, 23);
	// Create the Walls 
	for (ObstacleData const& obstacle : m_level.m_obstacles)
	{
		//creating the sprite 
		sf::Sprite sprite;
		sprite.setTexture(m_spriteSheetTexture);
		sprite.setTextureRect(wallRect);
		sprite.setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);

		//adding to the vector 
		m_wallSprites.push_back(sprite);
		
		sf::FloatRect spriteRect = sprite.getGlobalBounds();
		

		//adding to the spatial map 
		//left top corner 
		sf::Vector2f topLeftCorner(spriteRect.left,spriteRect.top);
		int topLeftCell = calculateSpatialMapCell(topLeftCorner);
		m_wallSpatialMap[topLeftCell].push_back(sprite);

		//right top corner 
		sf::Vector2f topRightCorner(spriteRect.left + spriteRect.width, spriteRect.top);
		int topRightCell = calculateSpatialMapCell(topRightCorner);
		m_wallSpatialMap[topRightCell].push_back(sprite);

		//left bottom corner 
		sf::Vector2f bottomLeftCorner(spriteRect.left, spriteRect.top + spriteRect.height);
		int bottomLeftCell = calculateSpatialMapCell(bottomLeftCorner);
		m_wallSpatialMap[bottomLeftCell].push_back(sprite);

		//right bottom corner 
		sf::Vector2f bottomRightCorner(spriteRect.left + spriteRect.width, spriteRect.top + spriteRect.height);
		int bottomRightCell = calculateSpatialMapCell(bottomRightCorner);
		m_wallSpatialMap[bottomRightCell].push_back(sprite);

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
	//stopping the game
	m_state = GameState::OVER;
	m_targets.revealResult();
	m_tank.clearDependantObjects();
	m_bigDisplayedText.setString("Game Over !");

	//Sounds stuff
	m_soundManager.switchToMenuMusic();

	//Seting the performance display 
	UserPerformance actualPerf = m_tank.getPerformance();
	UserPerformance bestPerf = UserPerformance::loadFromFile("./resources/scores/BestOfAllTime.yaml");
	
	std::string title = "\nHit\nTotal Targets\nSuccess\nFired\nAccuracy";
	std::string actualStats = "Player\n" + actualPerf.toStringColumnFull();
	std::string bestStats = "Best\n" + bestPerf.toStringColumnFull();

	if (bestPerf < actualPerf)
	{
		//saving Actual perf 
		actualPerf.saveOnFile("./resources/scores/BestOfAllTime.yaml");

		//Set the new string
		bestStats = "Best\n" + actualPerf.toStringColumnFull();
	}

	
	m_statTitleText = sf::Text(title, m_fontA, 60);
	m_statTitleText.setFillColor(sf::Color::Black);
	m_statTitleText.setPosition(100, 200);

	m_playerStatsText = sf::Text(actualStats, m_fontA, 60);
	m_playerStatsText.setFillColor(sf::Color(0, 0, 150));
	m_playerStatsText.setPosition(700, 200);

	m_gameOverBestStatsText = sf::Text(bestStats, m_fontA, 60);
	m_gameOverBestStatsText.setFillColor(sf::Color(168, 152, 0));
	m_gameOverBestStatsText.setPosition(1000, 200);

}

void Game::start()
{
	m_state = GameState::IN_PROGRESS;
	m_soundManager.switchToLevelMusic();
	m_timerLeft.restart(sf::seconds(60.f));
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	//updating the sound
	m_soundManager.update();

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

			//updating the Ai tank 
			m_aiTank.update(m_tank, dt);

			//updating the stats 
			UserPerformance stats = m_tank.getPerformance();
			m_playerStatsText.setString(stats.toStringColumn());

			
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

	//drawing the stats 
	m_window.draw(m_statTitleText);
	m_window.draw(m_playerStatsText);

	//drawing the target
	m_targets.render(m_window);

	//drawing the tank 
	m_tank.render(m_window);

	//drawing the AI tank 
	m_aiTank.render(m_window);

	//Not Started 
	if (m_state == GameState::NOT_STARTED)
	{
		m_window.draw(m_smokedSprite); //smoked sprite
		m_window.draw(m_bigDisplayedText); // re drawing the text
	}

	// GameOver 
	if (m_state == GameState::OVER)
	{
		m_window.draw(m_smokedSprite); //smoked sprite
		m_window.draw(m_bigDisplayedText); // re drawing the game over text
		m_window.draw(m_statTitleText); // Stats text
		m_window.draw(m_playerStatsText); // Actual stats
		m_window.draw(m_gameOverBestStatsText); // Best stats
	}

	m_window.display();
}




