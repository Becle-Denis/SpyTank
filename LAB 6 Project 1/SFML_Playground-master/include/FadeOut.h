#pragma once
#include "SoundEffect.h"
#include <Thor/Time/CallbackTimer.hpp>

class FadeOut : public SoundEffect
{
private: // Attributes 
	thor::Timer m_timer;
	sf::Time m_totalTime;
	float m_initialVolume;
	bool m_stopMusic;
	sf::SoundSource& m_sound;


public: //function
	FadeOut(sf::SoundSource& sound, sf::Time fadeTime, bool stopMusic = true);

	virtual bool updateEnd() override;
};

