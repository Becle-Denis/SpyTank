#pragma once
#include "SoundEffect.h"
#include <Thor/Time/CallbackTimer.hpp>

class FadeIn : public SoundEffect
{
private: // Attributes 
	//time 
	thor::Timer m_timer;
	sf::Time m_totalTime;

	//sound controls 
	float m_finalVolume;
	
	sf::SoundSource& m_sound;


public: //function
	/// <summary>
	/// Create a fade In effect, start the sound
	/// </summary>
	/// <param name="sound">Sound to be faded In</param>
	/// <param name="fadeTime">Total time of the fade</param>
	/// <param name="finalVolume">Final volume of the sound at the end of the fade</param>
	FadeIn(sf::SoundSource& sound, sf::Time fadeTime, float finalVolume);

	virtual bool updateEnd() override;
};

