#pragma once

#include <SFML/Graphics.hpp>
#include "ScreenSize.h"
#include "LevelLoader.h"
#include "Tank.h"
#include "TankAI.h"
#include "ProjectilePool.h"
#include "TargetManager.h"
#include "UserPerformance.h"
#include "SoundManager.h"
#include <map>
#include <list>
#include "GameState.h"
#include "HUD.h"
#include "ResourcesManager.h"

/// <summary>
/// @author RP
/// @date June 2017
/// @version 1.0
/// 
/// @completed September 2019 / Becle Denis C00254311 / Lab Exercices 
/// </summary>



/// <summary>
/// @brief Main class for the SFML Playground project.
/// 
/// This will be a single class framework for learning about SFML. 
/// Example usage:
///		Game game;
///		game.run();
/// </summary>

class Game
{
protected: //attributes 

	sf::RenderWindow m_window; // main window

	GameState m_state; //state of the game

	LevelData m_level; // Load the game level data.


	Tank m_tank; // An instance representing the player controlled tank.
	TankAi m_aiTank; // An instance representing the AI controlled tank.

	TargetManager m_targets; // Targets 

	ProjectilePool m_projectilesPool; // Pool of Projectile 

	SoundManager m_soundManager; // sounds

	sf::Sprite m_bgSprite; //backgroud sprite

	sf::Sprite m_smokedSprite;

	//Wall sprites storages 
	std::vector<sf::Sprite> m_wallSprites; // Vector 
	std::map<int, std::list< sf::Sprite> > m_wallSpatialMap; // map 


	thor::Timer m_timerLeft; // Timer for the time left in Game

	// The game HUD instance.
	HUD m_hud;


public: // function 
	/// <summary>
	/// @brief Default constructor that initialises the SFML window, 
	///   and sets vertical sync enabled. 
	/// </summary>
	Game();


	/// <summary>
	/// @brief the main game loop.
	/// 
	/// A complete loop involves processing SFML events, updating and drawing all game objects.
	/// The actual elapsed time for a single game loop results (lag) is stored. If this value is 
	///  greater than the notional time for one loop (MS_PER_UPDATE), then additional updates will be 
	///  performed until the lag is less than the notional time for one loop.
	/// The target is one update and one render cycle per game loop, but slower PCs may 
	///  perform more update than render operations in one loop.
	/// </summary>
	void run();

protected: // functions

	/// <summary>
	/// @brief Calculate the spatial cell of the given position
	/// </summary>
	/// <param name="GamePosition">Position to find the cell</param>
	/// <returns>spatial cell nuber</returns>
	int calculateSpatialMapCell(sf::Vector2f gamePosition);

	/// <summary>
	/// @brief Creates the wall sprites and loads them into a vector.
	/// Note that sf::Sprite is considered a light weight class, so 
	///  storing copies (instead of pointers to sf::Sprite) in std::vector is acceptable.
	/// </summary>
	void generateWalls();


	/// <summary>
	/// @brief Placeholder to perform updates to all game objects.
	/// </summary>
	/// <param name="time">update delta time</param>
	void update(double dt);

	/// <summary>
	/// @brief Draws the background and foreground game objects in the SFML window.
	/// The render window is always cleared to black before anything is drawn.
	/// </summary>
	void render();

	/// <summary>
	/// @brief Checks for events.
	/// Allows window to function and exit. 
	/// Events are passed on to the Game::processGameEvents() method.
	/// </summary>	
	void processEvents();

	/// <summary>
	/// @brief Handles all user input.
	/// </summary>
	/// <param name="event">system event</param>
	void processGameEvents(sf::Event&);


	/// <summary>
	/// Set the game state to gameOver
	/// </summary>
	void setGameOver(bool win);

	/// <summary>
	/// Set the Game state to InProgress
	/// </summary>
	void start(GameState newState);
};
