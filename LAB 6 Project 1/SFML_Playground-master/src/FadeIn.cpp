#include "FadeIn.h"

FadeIn::FadeIn(sf::SoundSource& sound, sf::Time fadeTime, float finalVolume) :
	m_sound(sound), m_finalVolume(finalVolume), m_totalTime(fadeTime)
{
	m_sound.setVolume(0);
	m_sound.play();
	m_timer.restart(fadeTime);
}

bool FadeIn::updateEnd()
{
	if (m_timer.isExpired())
	{
		//end of the Fade IN  
		m_sound.setVolume(m_finalVolume);
		return true;
	}
	else
	{
		float passedTime = m_totalTime.asMilliseconds() - m_timer.getRemainingTime().asMilliseconds();
		float vol = m_finalVolume * (passedTime / (m_totalTime.asMilliseconds())); //Volume at t time 
		m_sound.setVolume(vol);
		return false;
	}
}

