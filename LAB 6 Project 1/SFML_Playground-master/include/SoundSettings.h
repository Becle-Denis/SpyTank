#pragma once
#include <SFML/Audio.hpp>
#include "LevelLoader.h"

class SoundSettings
{
private: //Attributes 
	//General 
	float m_masterVol{ 100 };
	float m_music_masterVol{ 100 };
	float m_fx_masterVol{ 100 };

	//listener
	bool m_listener_updatePosition{ true };
	bool m_listener_updateRotation{ true };
	sf::Vector2f m_listener_basePosition{ sf::Vector2f(720,450) };
	double m_listener_baseRotation{ -90 };
	
	//Music
	float m_music_menuMusicVol{ 80 };
	float m_music_levelMusicVol{ 70 };

	//fx
	float m_fx_fireVol{ 100 };
	float m_fx_targetStartVol{ 80 };
	float m_fx_targetImpactVol{ 100 };
	float m_fx_wallImpactVol{ 100 };

public: //functions
	SoundSettings() = default;

	//accesser 
	float menuMusicVol();
	float levelMusicVol();
	float fireVol();
	float targetStartVol();
	float targetImpactVol();
	float wallImpactVol();
	bool updateListenerPosition(sf::Vector2f& position);
	bool updateListenerRotation(double& orientation);

	static SoundSettings loadGeneralSettings();
	static SoundSettings loadFromFile(std::string filePath);

};

