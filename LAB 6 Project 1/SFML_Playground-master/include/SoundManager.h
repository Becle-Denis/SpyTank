#pragma once
#include <SFML/Audio.hpp>
class SoundManager
{
private:
	sf::Music m_levelMusic;
	sf::Music m_menuMusic;

public:
	SoundManager(std::string filePath);
	void startLevelMusic();
	void stopLevelMusic();
	void startMenuMusic();
	void stopMenuMusic();
};

