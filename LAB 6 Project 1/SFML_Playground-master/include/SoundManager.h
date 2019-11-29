#pragma once
#include <SFML/Audio.hpp>
#include "MathUtility.h"
#include "SoundEffect.h"
#include "FadeIn.h"

class SoundManager
{
private: // Attributes 
	//buffer 
	sf::SoundBuffer m_fireBuffer;
	sf::SoundBuffer m_targetImpactBuffer;
	sf::SoundBuffer m_wallImpactBuffer;
	sf::SoundBuffer m_targetStartBuffer;

	//music
	sf::Music m_levelMusic;
	sf::Music m_menuMusic;
	
	//sounds
	sf::Sound m_fireSound;
	sf::Sound m_targetImpactSound;
	sf::Sound m_targetStartSound;
	sf::Sound m_wallImpactSound;

	//effects
	std::vector<SoundEffect*> m_effectsInProgressPtr;

	//listener
	sf::Vector2f m_listenerPosition;
	double m_listenerRotation;

public: // functions 
	SoundManager(std::string filePath);
	void startLevelMusic();
	void stopLevelMusic();
	void startMenuMusic();
	void stopMenuMusic();
	void playFireSound();
	void playTargetImpactSound(sf::Vector2f position);
	void playTargetStartSound(sf::Vector2f position);
	void playWallImpactSound(sf::Vector2f position);
	void updateListenerPostion(sf::Vector2f position, double rotation = 0);

	void update();

private: //functions 
	void playSound(sf::Sound& sound, sf::Vector2f position = sf::Vector2f(0, 0));
};

