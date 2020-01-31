#include "HUD.h"

////////////////////////////////////////////////////////////
HUD::HUD(sf::Font& hudFont)
	: m_textFont(hudFont), m_over(false)
{
	m_bigDisplayedText = sf::Text("BLIND TANK", m_textFont, 50);
	m_bigDisplayedText.setFillColor(sf::Color::Red);
	m_bigDisplayedText.setPosition(450, 50);

	//set the texts for realTime stats
	m_statTitleText = sf::Text("Hits\nSuccess\nAccuracy", m_textFont, 30);
	m_statTitleText.setFillColor(sf::Color(0, 0, 125));
	m_statTitleText.setPosition(150, 40);

	m_playerStatsText = sf::Text("", m_textFont, 30);
	m_playerStatsText.setFillColor(sf::Color(0, 0, 125));
	m_playerStatsText.setPosition(50, 40);
}

void HUD::setOver(GameState state, UserPerformance playerPerf, UserPerformance bestPerf)
{
	m_over = true;

	if (state == GameState::OVER_LOSE)
	{
		m_bigDisplayedText.setString("You Lose !");
	}
	else
	{
		m_bigDisplayedText.setString("You Win !");
	}


	//Set The game Over texts 
	std::string title = "\nHit\nTotal Targets\nSuccess\nFired\nAccuracy";
	std::string actualStats = "Player\n" + playerPerf.toStringColumnFull();
	std::string bestStats = "Best\n" + bestPerf.toStringColumnFull();

	m_statTitleText = sf::Text(title, m_textFont, 60);
	m_statTitleText.setFillColor(sf::Color::Black);
	m_statTitleText.setPosition(100, 200);

	m_playerStatsText = sf::Text(actualStats, m_textFont, 60);
	m_playerStatsText.setFillColor(sf::Color(0, 0, 150));
	m_playerStatsText.setPosition(700, 200);

	m_gameOverBestStatsText = sf::Text(bestStats, m_textFont, 60);
	m_gameOverBestStatsText.setFillColor(sf::Color(168, 152, 0));
	m_gameOverBestStatsText.setPosition(1000, 200);
	
}

////////////////////////////////////////////////////////////
void HUD::update(sf::Time remainingTime, UserPerformance userPerf)
{
	//updating timer 
	m_bigDisplayedText.setString("" + std::to_string((int)remainingTime.asSeconds()) + " seconds left");

	//updating Stats
	m_playerStatsText.setString(userPerf.toStringColumn());
}

void HUD::render(sf::RenderWindow& window)
{
	window.draw(m_playerStatsText);
	window.draw(m_statTitleText);

	if (m_over)
	{
		window.draw(m_gameOverBestStatsText);
	}
	window.draw(m_bigDisplayedText);
}