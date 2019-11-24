#include "SoundManager.h"

SoundManager::SoundManager(std::string filePath)
{
	if (!m_levelMusic.openFromFile(filePath))
	{
		std::string s("Error loading music "+ filePath);
		throw std::exception(s.c_str());
	}
	m_levelMusic.setRelativeToListener(false);
	m_levelMusic.setVolume(70);
}

void SoundManager::startLevelMusic()
{
	m_levelMusic.play();
}

void SoundManager::stopLevelMusic()
{
	m_levelMusic.stop();
}
