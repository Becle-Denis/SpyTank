#include "SoundManager.h"

SoundManager::SoundManager(std::string filePath)
{
	//loading
	if (!m_levelMusic.openFromFile(filePath))
	{
		std::string s("Error loading music "+ filePath);
		throw std::exception(s.c_str());
	}
	
	if (!m_menuMusic.openFromFile("./resources/sounds/music/MenuMusic.ogg"))
	{
		std::string s("Error loading music ./resources/sounds/music/MenuMusic.ogg");
		throw std::exception(s.c_str());
	}

	if (!m_fireBuffer.loadFromFile("./resources/sounds/fx/fire1.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/fire1.wav");
		throw std::exception(s.c_str());
	}
	m_fireSound.setBuffer(m_fireBuffer);

	if (!m_targetBuffer.loadFromFile("./resources/sounds/fx/targetImpact1.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/targetImpact1.wav");
		throw std::exception(s.c_str());
	}
	m_targetSound.setBuffer(m_targetBuffer);

	//mixing 
	m_levelMusic.setRelativeToListener(false);
	m_levelMusic.setVolume(70);
	m_menuMusic.setRelativeToListener(false);
	m_menuMusic.setVolume(80);
	m_fireSound.setVolume(100);
	m_targetSound.setVolume(100);
	m_targetSound.setRelativeToListener(false);
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

void SoundManager::playFireSound()
{
	m_fireSound.play();
}

void SoundManager::playTargetSound(sf::Vector2f position)
{
	//m_targetSound.setPosition(sf::Vector3f(position.x,position.y,0));
	m_targetSound.play();
}
