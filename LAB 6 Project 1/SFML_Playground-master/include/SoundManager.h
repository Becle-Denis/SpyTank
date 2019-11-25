#pragma once
#include <SFML/Audio.hpp>
class SoundManager
{
private:
	sf::Music m_levelMusic;
	sf::Music m_menuMusic;
	
	sf::SoundBuffer m_fireBuffer;
	sf::Sound m_fireSound;

	sf::SoundBuffer m_targetBuffer;
	sf::Sound m_targetSound;

public:
	SoundManager(std::string filePath);
	void startLevelMusic();
	void stopLevelMusic();
	void startMenuMusic();
	void stopMenuMusic();
	void playFireSound();
	void playTargetSound(sf::Vector2f position);
};

