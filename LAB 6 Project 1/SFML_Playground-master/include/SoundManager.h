#pragma once
#include <SFML/Audio.hpp>
#include "MathUtility.h"
#include "SoundEffect.h"
#include "FadeIn.h"
#include "FadeOut.h"
#include "CrossFade.h"
#include "SoundSettings.h"

struct SpatializedSound
{
	sf::Vector2f position;
	sf::Sound sound;

	SpatializedSound(sf::SoundBuffer& buffer, sf::Vector2f position, float volume = 0, bool relativeToListener = true)
	{
		sound.setBuffer(buffer);
		sound.setRelativeToListener(relativeToListener);
		sound.setMinDistance(100);
		sound.setVolume(volume);
		this->position = sf::Vector2f(position.x, position.y);
	}
};


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
	std::vector<SpatializedSound*> m_soundsInProgressPtr;

	//effects
	std::vector<SoundEffect*> m_effectsInProgressPtr;

	//listener
	sf::Vector2f m_listenerPosition;
	double m_listenerRotation;
	bool m_updatePosition;
	bool m_updateRotation;

	//settings 
	SoundSettings m_settings;

public: // functions 
	SoundManager(std::string filePath);
	~SoundManager();
	void startMenuMusic();
	void switchToLevelMusic();
	void switchToMenuMusic();
	void playFireSound();
	void playTargetImpactSound(sf::Vector2f position);
	void playTargetStartSound(sf::Vector2f position);
	void playWallImpactSound(sf::Vector2f position);
	void updateListenerPostion(sf::Vector2f position, double rotation = 0);

	void update();

private: //functions 
	void setSettings();
	void playSound(sf::SoundBuffer& buffer, sf::Vector2f position = sf::Vector2f(0, 0), float volume = 0, bool relativeToListener = true);
	void updateSpatialisation(SpatializedSound* sound);
};

