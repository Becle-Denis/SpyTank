#include "FadeOut.h"

FadeOut::FadeOut(sf::SoundSource& sound, sf::Time fadeTime, bool stopMusic) :
	m_sound(sound),m_totalTime(fadeTime),m_stopMusic(stopMusic)
{
	m_initialVolume = m_sound.getVolume();
	m_timer.restart(fadeTime);
}

bool FadeOut::updateEnd()
{
	if (m_timer.isExpired())
	{
		if (m_stopMusic)
		{
			m_sound.stop();
		}
		return true;
	}
	else
	{
		float remainingTime = m_timer.getRemainingTime().asMilliseconds();
		float vol = m_initialVolume * (remainingTime / (m_totalTime.asMilliseconds()));
		m_sound.setVolume(vol);
		return false;
	}
}
