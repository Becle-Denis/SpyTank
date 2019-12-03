#pragma once
#include "SoundEffect.h"
#include <Thor/Time/CallbackTimer.hpp>

class FadeIn : public SoundEffect
{
private: // Attributes 
	thor::Timer m_timer;
	sf::Time m_totalTime;
	float m_finalVolume;
	sf::SoundSource& m_sound;


public: //function
	FadeIn(sf::SoundSource& sound, sf::Time fadeTime, float finalVolume);

	virtual bool updateEnd() override;
};

