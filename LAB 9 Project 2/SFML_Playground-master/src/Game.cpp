#include "Game.h"
#include <iostream>
#include "Projectile.h"

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

const sf::Time Game::s_DAY_TIME = sf::milliseconds(5550);

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_height, ScreenSize::s_width, 32), "SFML Playground", sf::Style::Default),
	m_tank(m_wallSprites, m_wallSpatialMap, m_targets, m_projectilesPool, m_soundManager, m_hud, m_noiseScreen),
	m_aiTank(m_wallSprites, m_wallSpatialMap, m_projectilesPool, m_soundManager),
	m_aiTank2(m_wallSprites, m_wallSpatialMap, m_projectilesPool, m_soundManager),
	m_state(GameState::NOT_STARTED), 
	m_projectilesPool(10, &m_soundManager),
	m_light(LightMode::DAY)
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
	m_bgSprite.setTexture(ResourcesManager::getTexture(TexturesName::LEVEL_BACKGROUND));

	m_smokedSprite.setTexture(ResourcesManager::getTexture(TexturesName::SMOKED));
	m_smokedSprite.setTextureRect(sf::IntRect(0, 0, 960, 540));
	m_smokedSprite.setScale(1.7, 1.7);
	m_smokedSprite.setColor(sf::Color(255, 255, 255, 150));


	// Set the tank position in one corner randmoly.
	switch (rand() % 4)
	{
	case 0:
		m_tank.setStartingPosition(sf::Vector2f(150, 150));
		break;
	case 1:
		m_tank.setStartingPosition(sf::Vector2f(ScreenSize::s_height - 150, 150));
		break;
	case 2:
		m_tank.setStartingPosition(sf::Vector2f(150, ScreenSize::s_width - 150));
		break;
	case 3:
		m_tank.setStartingPosition(sf::Vector2f(ScreenSize::s_height - 150, ScreenSize::s_width - 150));
		break;
	}

	// Construct the wall sprites
	generateWalls();

	//initialise the tank 
	//m_tank.initialise();

	// Populate the obstacle list and set the AI tanks position.
	m_aiTank.init(m_level.m_aiTank.m_position);
	m_aiTank2.init(m_level.m_aiTank.m_position);

	//Constrcut the targets
	m_targets.construct(m_level.m_targets, 8, &m_soundManager);
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	//sound stuff
	m_soundManager.startMusic();

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
	sf::IntRect wallRect(2, 129, 34, 24);
	sf::Texture const& spriteSheetTexture = ResourcesManager::getTexture(TexturesName::SPRITE_SHEET);
	// Create the Walls 
	for (ObstacleData const& obstacle : m_level.m_obstacles)
	{
		//creating the sprite 
		sf::Sprite sprite;
		sprite.setTexture(spriteSheetTexture);
		sprite.setTextureRect(wallRect);
		sprite.setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);

		//adding to the vector 
		m_wallSprites.push_back(sprite);
		
		sf::FloatRect spriteRect = sprite.getGlobalBounds();
		//sprite.getTransform().transformPoint(0, 0);
		

		//adding to the spatial map 
		//left top corner 
		sf::Vector2f topLeftCorner = sprite.getTransform().transformPoint(0, 0);
		int topLeftCell = calculateSpatialMapCell(topLeftCorner);
		m_wallSpatialMap[topLeftCell].push_back(sprite);

		//right top corner 
		sf::Vector2f topRightCorner = sprite.getTransform().transformPoint(33, 0);
		int topRightCell = calculateSpatialMapCell(topRightCorner);
		m_wallSpatialMap[topRightCell].push_back(sprite);

		//left bottom corner 
		sf::Vector2f bottomLeftCorner = sprite.getTransform().transformPoint(0, 23);
		int bottomLeftCell = calculateSpatialMapCell(bottomLeftCorner);
		m_wallSpatialMap[bottomLeftCell].push_back(sprite);

		//right bottom corner 
		sf::Vector2f bottomRightCorner = sprite.getTransform().transformPoint(33, 23);
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

		case sf::Keyboard::Space:
			if (m_state != GameState::RUNNING_HIT_GAME && m_state != GameState::RUNNING_CATCH_GAME)
			{
				start(GameState::RUNNING_CATCH_GAME);
			}
			break;

		case sf::Keyboard::Enter:
			if (m_state != GameState::RUNNING_HIT_GAME && m_state != GameState::RUNNING_CATCH_GAME)
			{
				start(GameState::RUNNING_HIT_GAME);
			}
			break;

		default:
			break;
		}
	}
}

void Game::setGameOver(bool win)
{
	//stopping the game
	if (win)
	{
		m_state = GameState::OVER_WIN;
	}
	else
	{
		m_state = GameState::OVER_LOSE;
	}
	m_targets.revealResult();
	m_tank.clearDependantObjects();
	m_aiTank.clearDependantObjects();
	m_aiTank2.clearDependantObjects();

	//Sounds stuff
	m_soundManager.menu();

	//Seting the performance display 
	UserPerformance actualPerf = m_tank.getPerformance();
	UserPerformance bestPerf = UserPerformance::loadFromFile("./resources/scores/BestOfAllTime.yaml");

	//checking for best Performance 
	if (bestPerf < actualPerf)
	{
		//saving Actual perf 
		actualPerf.saveOnFile("./resources/scores/BestOfAllTime.yaml");

		//Changing the best Perf
		bestPerf = actualPerf;
	}
	
	//setting the hud
	m_noiseScreen.cleanNoise();
	m_hud.setOver(m_state, actualPerf, bestPerf);
	setLightMode(LightMode::DAY);
	

}

void Game::start(GameState newState)
{
	if (newState == GameState::RUNNING_CATCH_GAME || newState == GameState::RUNNING_HIT_GAME)
	{
		m_state = newState;
		m_hud.start(newState);
		m_timerLeft.restart(sf::seconds(60.f));
		if (newState == GameState::RUNNING_CATCH_GAME)
		{
			m_targets.start(false, ResourcesManager::getTexture(TexturesName::TARGET_CATCH), true);
			m_dayTimer = thor::Timer();
			m_nightMissionTime.restart();
			m_lastTankCapturedItem = 0;
			m_aiTank.start();
			m_aiTank2.start();
			setLightMode(LightMode::NIGHT);
		}
		else 
		{
			m_targets.start(true, ResourcesManager::getTexture(TexturesName::TARGET_HIT));
			setLightMode(LightMode::DAY);
		}

		m_tank.initialise(newState);
		m_soundManager.mission();

	}
}

void Game::setLightMode(LightMode mode)
{
	if (mode != m_light)
	{
		m_light = mode;

		//Game management 
		if (m_light == LightMode::DAY)
		{
			m_bgSprite.setTexture(ResourcesManager::getTexture(TexturesName::LEVEL_BACKGROUND));
			sf::Texture const& newSpriteSheetTexture = ResourcesManager::getTexture(TexturesName::SPRITE_SHEET);
			for (sf::Sprite& sprite : m_wallSprites)
			{
				sprite.setTexture(newSpriteSheetTexture);
			}
			m_soundManager.lightOn();
		}
		else
		{
			m_bgSprite.setTexture(ResourcesManager::getTexture(TexturesName::LEVEL_BACKGROUND_NIGHT));
			sf::Texture const& newSpriteSheetTexture = ResourcesManager::getTexture(TexturesName::SPRITE_SHEET_NIGHT);
			for (sf::Sprite& sprite : m_wallSprites)
			{
				sprite.setTexture(newSpriteSheetTexture);
			}
			m_soundManager.lightOff();
		}

		m_tank.setLightMode(m_light);
		m_aiTank.setLightMode(m_light);
		m_aiTank2.setLightMode(m_light);
		m_projectilesPool.setLightMode(m_light);
	}
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	//updating the sound
	m_soundManager.update();

	switch (m_state)
	{
	case GameState::RUNNING_HIT_GAME:
		if (m_timerLeft.isExpired()) //Winning game over
		{
			setGameOver(true);
		}
		else
		{
			//updating the targets
			m_targets.update(dt);

			//updating the tank 
			m_tank.update(dt);

			//updating the HUD
			m_hud.update(m_timerLeft.getRemainingTime(), m_tank.getPerformance());
		}
		break;

	case GameState::RUNNING_CATCH_GAME:
		int capturedTarget = m_tank.getNumberOfCapturedTarget();
		int targetLeft = m_targets.getNumberOfActiveTarget() - capturedTarget;
		if (targetLeft <= 0) //Winning game over
		{
			setGameOver(true);
		}
		else if ( m_aiTank.collidesWithPlayer(m_tank) || m_aiTank2.collidesWithPlayer(m_tank) || !m_tank.isAlive() ) //Losing
		{
			setGameOver(false);
		}
		else
		{
			//updating the targets
			m_targets.update(dt);

			//updating the tank 
			m_tank.update(dt);

			//updating the Ai tank 
			m_aiTank.update(m_tank, dt);
			m_aiTank2.update(m_tank, dt);

			//updating the HUD
			m_hud.update(targetLeft,m_dayTimer,m_nightMissionTime);

			//updating the noise screen 
			m_noiseScreen.update(dt);

			//checking for light change 
			if (m_lastTankCapturedItem != capturedTarget)
			{
				//new Captured Target 
				setLightMode(LightMode::DAY);
				m_dayTimer.restart(s_DAY_TIME);
			}
			else if (m_light == LightMode::DAY && m_dayTimer.isExpired())
			{
				setLightMode(LightMode::NIGHT);
			}
			m_lastTankCapturedItem = capturedTarget;
		}
		break;
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
	//drawing the HUD
	m_hud.render(m_window);

	//drawing the target
	m_targets.render(m_window);

	//drawing the tank 
	m_tank.render(m_window);

	if (m_state == GameState::RUNNING_CATCH_GAME)
	{
		//drawing the AI tank 
		m_aiTank.render(m_window);
		m_aiTank2.render(m_window);

		//dariwng the noise screen 
		m_noiseScreen.render(m_window);
	}

	//Not Started 
	if (m_state != GameState::RUNNING_HIT_GAME && m_state != GameState::RUNNING_CATCH_GAME)
	{
		m_window.draw(m_smokedSprite); //smoked sprite
		m_hud.render(m_window);; // re drawing the HUD
	}

	m_window.display();
}





