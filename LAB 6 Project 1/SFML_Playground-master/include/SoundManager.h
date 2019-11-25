#pragma once
#include <SFML/Audio.hpp>
class SoundManager
{
private:
	sf::Music m_levelMusic;
	sf::Music m_menuMusic;
	sf::SoundBuffer m_fireBuffer;
	sf::Sound m_fireSound;

public:
	SoundManager(std::string filePath);
	void startLevelMusic();
	void stopLevelMusic();
	void startMenuMusic();
	void stopMenuMusic();
	void playFireSound();
};

