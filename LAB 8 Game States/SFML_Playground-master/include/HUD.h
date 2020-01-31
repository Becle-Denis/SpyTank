#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "UserPerformance.h"

/// <summary>
/// @brief A basic HUD implementation.
/// 
/// Shows current game state only.
/// </summary>

class HUD
{
public:
	/// <summary>
	/// @brief Default constructor that stores a font for the HUD and initialises the general HUD appearance.
	/// </summary>
	HUD(sf::Font& hudFont);

	/// <summary>
	/// Start or restart the HUD
	/// </summary>
	void start();

	/// <summary>
	/// Set the HUD Game Over text
	/// </summary>
	/// <param name="state">State of the Game</param>
	/// <param name="playerPerf">Player actual performance</param>
	/// <param name="bestPerf">Over All best Performance</param>
	void setOver(GameState state, UserPerformance playerPerf, UserPerformance bestPerf);

	/// <summary>
	/// @brief Checks the current game state and sets the appropriate status text on the HUD.
	/// </summary>
	/// <param name="gameState">The current game state</param>
	void update(sf::Time remainingTime, UserPerformance userPerf);

	/// <summary>
	/// @brief Draws the HUD outline and text.
	///
	/// </summary>
	/// <param name="window">The SFML Render window</param>
	void render(sf::RenderWindow& window);


private:
	bool m_over;

	// The font for this HUD.
	sf::Font& m_textFont;

	//Text 
	sf::Text m_bigDisplayedText; //text for the timer
	sf::Text m_statTitleText; //text for displaying the performances titles
	sf::Text m_playerStatsText; // text for displaying the actual player performances 
	sf::Text m_gameOverBestStatsText; //text for displaying the best player performances 


};

