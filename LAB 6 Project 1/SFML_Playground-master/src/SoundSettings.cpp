#include "SoundSettings.h"

float SoundSettings::menuMusicVol()
{
	return (m_masterVol / 100) * (m_music_masterVol / 100) * m_music_menuMusicVol;
}

float SoundSettings::levelMusicVol()
{
	return (m_masterVol / 100) * (m_music_masterVol / 100)* m_music_levelMusicVol;
}

float SoundSettings::fireVol()
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_fireVol;
}

float SoundSettings::targetStartVol()
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_targetStartVol;
}

float SoundSettings::targetImpactVol()
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_targetStartVol;
}

float SoundSettings::wallImpactVol()
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_wallImpactVol;
}

bool SoundSettings::updateListenerPosition(sf::Vector2f& position)
{
	position = m_listener_basePosition;
	return m_listener_updatePosition;
}

bool SoundSettings::updateListenerRotation(double& orientation)
{
	orientation = m_listener_baseRotation;
	return m_listener_updateRotation;
}
