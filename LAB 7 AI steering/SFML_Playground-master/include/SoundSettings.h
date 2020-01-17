#pragma once
#include <SFML/Audio.hpp>
#include "LevelLoader.h"

class SoundSettings
{
private: //Attributes 
	//General 
	float m_masterVol{ 100 }; //MASTER
	float m_music_masterVol{ 100 }; //MUSIC MASTER
	float m_fx_masterVol{ 100 }; //FX MASTER 

	//listener
	bool m_listener_updatePosition{ true }; //if true Position the listener position is updated
	bool m_listener_updateRotation{ true }; //if true Position the listener rotation is updated 
	sf::Vector2f m_listener_basePosition{ sf::Vector2f(720,450) }; //need to be set if m_listener_updatePosition false
	double m_listener_baseRotation{ -90 }; //need to be set if m_listener_updateRotation false
	
	//Music
	float m_music_menuMusicVol{ 80 }; //Menu 
	float m_music_levelMusicVol{ 70 }; //Level 

	//fx
	float m_fx_fireVol{ 100 }; //fire
	float m_fx_targetStartVol{ 80 }; // target start 
	float m_fx_targetImpactVol{ 100 }; // target impact 
	float m_fx_wallImpactVol{ 100 }; // wall impact 
	float m_fx_projectileVol{ 80 }; // projectile volume
	float m_fx_motorVol{ 80 }; // motor max volume

public: //functions
	SoundSettings() = default; 

	//accessers 
	//volume accessers
	float menuMusicVol() const;
	float levelMusicVol() const;
	float fireVol() const;
	float targetStartVol() const;
	float targetImpactVol() const;
	float wallImpactVol() const;
	float projectileVol() const;
	float motorMaxVolume() const;

	//Listener accessers 
	/// <summary>
	/// accesser to the Listener Position settings 
	/// </summary>
	/// <param name="position">Return by reference the base position of the listener</param>
	/// <returns>True if the listener position should be updated</returns>
	bool updateListenerPosition(sf::Vector2f& position) const;

	/// <summary>
	/// accesser to the Listener Rotation settings 
	/// </summary>
	/// <param name="orientation">Return by reference the base rotation of the listener</param>
	/// <returns>True if the listener rotation should be updated</returns>
	bool updateListenerRotation(double& orientation) const;

	/// <summary>
	/// Load the general settings stored in a YAML file,
	/// if fail load the default settings 
	/// </summary>
	/// <returns>SoundSettings loaded</returns>
	static SoundSettings loadGeneralSettings();

	
	/// <summary>
	/// Load the general settings stored in YAML file corresponding,
	/// if fail load the default settings  
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns>SoundSettings loaded</returns>
	static SoundSettings loadFromFile(std::string filePath);

};

