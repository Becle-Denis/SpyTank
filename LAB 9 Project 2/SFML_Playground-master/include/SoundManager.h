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
#include "MovingMotorEffect.h"
#include "ResourcesManager.h"

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

	//musics
	sf::Music m_menuMusic;
	sf::Music m_missionMusic;
	sf::Music m_attackMusic;
	sf::Music m_discoveredMusic;
	
	int m_attackedTank;
	bool m_missionInProgress;
	bool m_lightOn;

	//sounds 
	std::vector<SpatializedSound*> m_soundsInProgressPtr; //Storage for spatializerd sound
	std::vector<SpatializedSound*> m_nonUpdatedSoundInProgressPtr; //Storage for spatialized sound moving with the listener

	//Sound effects
	std::vector<SoundEffect*> m_effectsInProgressPtr; //Storage for sound Effects 

	//listener
	sf::Vector2f m_listenerPosition; 
	double m_listenerRotation;
	bool m_updatePosition; //if true Position the listener position is updated
	bool m_updateRotation; //if true Position the listener rotation is updated 

	//settings 
	SoundSettings m_settings; //Storage for the sound settings 

	static const sf::Time s_MUSIC_TRANSITION_TIME;

public: // functions 
	/// <summary>
	/// Create a SoundManager for a Level
	/// </summary>
	/// <param name="filePath">filePath to the level Music</param>
	SoundManager();
	~SoundManager();

	//Music stuff
	void startMusic();
	void menu();
	void mission();
	void lightOn();
	void lightOff();
	void tankAttacked();
	void tanksafe();






	// Updated fx, returns pointer to sound effect 
	MovingMotorEffect* tankMotorEffect();
	MovingSound* startProjectileSound(sf::Vector2f position);

	//FX
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

	//music stuff
	void switchToMenuMusic();
	void switchToAttackMusic();
	void switchToDiscoveredMusic();
	void switchToMissionMusic();


	void setSettings(); //Set the sounds settings according to m_settings 

	/// <summary>
	/// Create a Spatialized sound and play it 
	/// </summary>
	/// <param name="buffer">Reference to the actual Sound buffer</param>
	/// <param name="position">Local Position of the sound</param>
	/// <param name="volume">Volume of the sound</param>
	/// <param name="relativeToListener">Is Spatialized or Not</param>
	SpatializedSound* playSound(sf::SoundBuffer const& buffer, sf::Vector2f position = sf::Vector2f(0, 0), float volume = 0, bool SpatializationUpdated = true);
	
	/// <summary>
	/// Update the position of the sound according to the position of the listener 
	/// </summary>
	/// <param name="sound">Spatialized sound to update</param>
	void updateSpatialisation(SpatializedSound* sound);
};

