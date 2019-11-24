#pragma once
#include <SFML/Audio.hpp>
class SoundManager
{
private:
	sf::Music m_levelMusic;

public:
	SoundManager(std::string filePath);
	void startLevelMusic();
	void stopLevelMusic();
};

