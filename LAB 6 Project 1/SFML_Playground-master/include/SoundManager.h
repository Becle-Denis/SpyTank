#pragma once
#include <SFML/Audio.hpp>
#include "MathUtility.h"
#include "SoundEffect.h"
#include "FadeIn.h"
#include "FadeOut.h"
#include "CrossFade.h"
#include "SoundSettings.h"
#include "CrossFade.h"
#include "MovingSound.h"

/// <summary>
/// Represent a sound in a local position for the game world 
/// </summary>
struct SpatializedSound
{
	sf::Vector2f position; //Local position
	sf::Sound sound; 

	SpatializedSound(sf::SoundBuffer const& buffer, sf::Vector2f position, float volume = 0, bool relativeToListener = true)
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
	//Sound buffers 
	sf::SoundBuffer m_fireBuffer;
	sf::SoundBuffer m_targetImpactBuffer;
	sf::SoundBuffer m_wallImpactBuffer;
	sf::SoundBuffer m_targetStartBuffer;
	sf::SoundBuffer m_projectileBuffer;

	//musics
	sf::Music m_levelMusic;
	sf::Music m_menuMusic;
	
	//sounds 
	std::vector<SpatializedSound*> m_soundsInProgressPtr; //Storage for spatializerd sound

	//Sound effects
	std::vector<SoundEffect*> m_effectsInProgressPtr; //Storage for sound Effects 

	//listener
	sf::Vector2f m_listenerPosition; 
	double m_listenerRotation;
	bool m_updatePosition; //if true Position the listener position is updated
	bool m_updateRotation; //if true Position the listener rotation is updated 

	//settings 
	SoundSettings m_settings; //Storage for the sound settings 

public: // functions 
	/// <summary>
	/// Create a SoundManager for a Level
	/// </summary>
	/// <param name="filePath">filePath to the level Music</param>
	SoundManager(std::string filePath);
	~SoundManager();

	//Music stuff
	void startMenuMusic();

	/// <summary>
	/// Stop the Menu music and start the level Music with a crossFade effect 
	/// </summary>
	void switchToLevelMusic(); 
	
	/// <summary>
	/// Stop the Level music and start the menu Music with a crossFade effect 
	/// </summary>
	void switchToMenuMusic();

	//Fx stuff
	MovingSound* startProjectileSound(sf::Vector2f position);
	void playFireSound(sf::Vector2f position);
	void playTargetImpactSound(sf::Vector2f position);
	void playTargetStartSound(sf::Vector2f position);
	void playWallImpactSound(sf::Vector2f position);

	//Listener 
	/// <summary>
	/// Move the listener Position to the Listener variables 
	/// </summary>
	/// <param name="position">New Local position of the Listener</param>
	/// <param name="rotation">New Local rotation of the LIstener</param>
	void updateListenerPostion(sf::Vector2f position, double rotation = 0);

	/// <summary>
	/// Update All the sound effects and update the spatilisation of all the playing sounds
	/// </summary>
	void update();

private: //functions 
	void setSettings(); //Set the sounds settings according to m_settings 

	/// <summary>
	/// Create a Spatialized sound and play it 
	/// </summary>
	/// <param name="buffer">Reference to the actual Sound buffer</param>
	/// <param name="position">Local Position of the sound</param>
	/// <param name="volume">Volume of the sound</param>
	/// <param name="relativeToListener">Is Spatialized or Not</param>
	SpatializedSound* playSound(sf::SoundBuffer const& buffer, sf::Vector2f position = sf::Vector2f(0, 0), float volume = 0, bool relativeToListener = true);
	
	/// <summary>
	/// Update the position of the sound according to the position of the listener 
	/// </summary>
	/// <param name="sound">Spatialized sound to update</param>
	void updateSpatialisation(SpatializedSound* sound);
};

