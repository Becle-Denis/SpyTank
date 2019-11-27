#pragma once
#include <SFML/Audio.hpp>
#include "MathUtility.h"

class SoundManager
{
private:
	sf::Music m_levelMusic;
	sf::Music m_menuMusic;
	
	sf::SoundBuffer m_fireBuffer;
	sf::Sound m_fireSound;

	sf::SoundBuffer m_targetBuffer;
	sf::Sound m_targetSound;

	sf::Vector2f m_listenerPosition;
	double m_listenerRotation;

public:
	SoundManager(std::string filePath);
	void startLevelMusic();
	void stopLevelMusic();
	void startMenuMusic();
	void stopMenuMusic();
	void playFireSound();
	void playTargetSound(sf::Vector2f position);
	void updateListenerPostion(sf::Vector2f position, double rotation = 0);
};

