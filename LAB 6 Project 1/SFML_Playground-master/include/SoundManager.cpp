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

	if (!m_menuMusic.openFromFile("./resources/sounds/music/MenuMusic.ogg"))
	{
		std::string s("Error loading music ./resources/sounds/music/MenuMusic.ogg");
		throw std::exception(s.c_str());
	}
	m_menuMusic.setRelativeToListener(false);
	m_menuMusic.setVolume(80);
}

void SoundManager::startLevelMusic()
{
	m_levelMusic.play();
}

void SoundManager::stopLevelMusic()
{
	m_levelMusic.stop();
}

void SoundManager::startMenuMusic()
{
	m_menuMusic.play();
	m_menuMusic.setLoop(true);
}

void SoundManager::stopMenuMusic()
{
	m_menuMusic.stop();
}
