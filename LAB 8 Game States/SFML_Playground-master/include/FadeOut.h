#pragma once
#include "SoundEffect.h"
#include <Thor/Time/CallbackTimer.hpp>

/// <summary>
/// Fade Out sound effect 
/// </summary>
class FadeOut : public SoundEffect
{
private: // Attributes 
	//time 
	thor::Timer m_timer; 
	sf::Time m_totalTime; 
	
	//sound controls
	float m_initialVolume;
	bool m_stopMusic;

	sf::SoundSource& m_sound;


public: //function
	/// <summary>
	/// Create a fade Out effect
	/// </summary>
	/// <param name="sound">Sound to be faded out</param>
	/// <param name="fadeTime">Total time of the fade</param>
	/// <param name="stopOutSound">True is the sound should be stopped at the end of the Fade</param>
	FadeOut(sf::SoundSource& sound, sf::Time fadeTime, bool stopOutSound = true);

	virtual bool updateEnd() override;
};

