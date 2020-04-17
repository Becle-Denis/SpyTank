#include "HUD.h"

////////////////////////////////////////////////////////////
HUD::HUD()
	: m_state(GameState::NOT_STARTED)
{
	sf::Font const& font = ResourcesManager::getFont(FontName::MAIN_FONT);

	m_bigDisplayedText = sf::Text("BLIND TANK", font, 50);
	m_bigDisplayedText.setFillColor(sf::Color::Red);
	m_bigDisplayedText.setPosition(450, 50);

	m_eventText.setFont(font);
	m_eventText.setString("");
	m_eventText.setCharacterSize(25);
	m_eventText.setFillColor(sf::Color(5, 5, 55));
	m_eventText.setPosition(40, 40);
	
	m_startHitGameText.setFont(font);
	m_startHitGameText.setFillColor(sf::Color(0, 85, 180, 255));
	m_startHitGameText.setPosition(275.0f, 695.0f);
	m_startHitGameText.setCharacterSize(40);
	m_startHitGameText.setString("Press Enter to play DAY TRAINING");


	m_startCatchGameText.setFont(font);
	m_startCatchGameText.setFillColor(sf::Color(0, 25, 760, 255));
	m_startCatchGameText.setPosition(270.0f, 625.0f);
	m_startCatchGameText.setCharacterSize(40);
	m_startCatchGameText.setString("Press Space to play NIGHT MISSION");


	m_gameOverBestStatsText.setFont(font);
	m_playerStatsText.setFont(font);
	m_statTitleText.setFont(font);


}

void HUD::start(GameState state)
{
	m_state = state;
	
	m_statTitleText.setString("Hits\nSuccess\nAccuracy");
	m_statTitleText.setCharacterSize(30);
	m_statTitleText.setFillColor(sf::Color(0, 0, 125));
	m_statTitleText.setPosition(150, 40);

	m_playerStatsText.setString("");
	m_playerStatsText.setCharacterSize(30);
	m_playerStatsText.setFillColor(sf::Color(0, 0, 125));
	m_playerStatsText.setPosition(50, 40);

	m_eventText.setString("");


}

void HUD::setOver(GameState state, UserPerformance playerPerf, UserPerformance bestPerf)
{
	std::string bigText = "Game Over";
	std::string title = "";
	std::string actualStats = "";
	std::string bestStats = "";

	if (m_state == GameState::RUNNING_CATCH_GAME)
	{
		if (state == GameState::OVER_LOSE)
		{
			bigText = "You Lose !" ;
		}
		else
		{
			bigText = "You Win !";
		}
	}
	else
	{
		//Set The game Over texts 
		title = "\nHit\nTotal Targets\nSuccess\nFired\nAccuracy";
		actualStats = "Player\n" + playerPerf.toStringColumnFull();
		bestStats = "Best\n" + bestPerf.toStringColumnFull();
	}

	//Setting up the Game Over Results
	sf::Font const& textFont = ResourcesManager::getFont(FontName::MAIN_FONT);

	m_bigDisplayedText.setString(bigText);

	m_statTitleText = sf::Text(title, textFont, 60);
	m_statTitleText.setFillColor(sf::Color::Black);
	m_statTitleText.setPosition(100, 200);

	m_playerStatsText = sf::Text(actualStats, textFont, 60);
	m_playerStatsText.setFillColor(sf::Color(0, 0, 150));
	m_playerStatsText.setPosition(700, 200);

	m_gameOverBestStatsText = sf::Text(bestStats, textFont, 60);
	m_gameOverBestStatsText.setFillColor(sf::Color(168, 152, 0));
	m_gameOverBestStatsText.setPosition(1000, 200);


	

	m_state = state;
}

void HUD::addEvent(std::string stringEvent)
{
	std::string text = m_eventText.getString();
	text += stringEvent + "\n";
	m_eventText.setString(text);
}

////////////////////////////////////////////////////////////
void HUD::update(sf::Time remainingTime, UserPerformance userPerf)
{
	//updating timer 
	m_bigDisplayedText.setString("" + std::to_string((int)remainingTime.asSeconds()) + " seconds left");
	//updating Stats
	m_playerStatsText.setString(userPerf.toStringColumn());

}

void HUD::update(int remainingTarget)
{
	m_bigDisplayedText.setString("" + std::to_string(remainingTarget) + " targets missing");
}

void HUD::render(sf::RenderWindow& window)
{
	switch (m_state)
	{
	case GameState::RUNNING_HIT_GAME:
		window.draw(m_playerStatsText);
		window.draw(m_statTitleText);
		break;

	case GameState::RUNNING_CATCH_GAME:
		window.draw(m_eventText);
		break;

	case GameState::NOT_STARTED:
	case GameState::OVER_LOSE:
	case GameState::OVER_WIN:
		window.draw(m_playerStatsText);
		window.draw(m_statTitleText);
		window.draw(m_gameOverBestStatsText);
		window.draw(m_startCatchGameText);
		window.draw(m_startHitGameText);
		break;
	}

	window.draw(m_bigDisplayedText);
}