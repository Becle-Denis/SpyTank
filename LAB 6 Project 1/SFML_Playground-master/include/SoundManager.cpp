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

	if (!m_targetBuffer.loadFromFile("./resources/sounds/fx/targetImpact2.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/targetImpact2.wav");
		throw std::exception(s.c_str());
	}
	m_targetSound.setBuffer(m_targetBuffer);

	if (!m_wallImpactBuffer.loadFromFile("./resources/sounds/fx/WallImpact1.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/WallImpact1.wav");
		throw std::exception(s.c_str());
	}
	m_wallImpactSound.setBuffer(m_wallImpactBuffer);

	//mixing 
	m_levelMusic.setRelativeToListener(false);
	m_levelMusic.setVolume(70);
	m_menuMusic.setRelativeToListener(false);
	m_menuMusic.setVolume(80);
	m_fireSound.setVolume(100);
	m_targetSound.setVolume(100);
	m_targetSound.setRelativeToListener(true);
	m_targetSound.setMinDistance(100);
	m_wallImpactSound.setVolume(80);
	m_wallImpactSound.setRelativeToListener(false);
	//m_wallImpactSound.setMinDistance(100);

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
	sf::Vector2f positionRelativeToOrigin = position - m_listenerPosition;
	thor::rotate<float>(positionRelativeToOrigin, -(m_listenerRotation + 90));
	m_targetSound.setPosition(positionRelativeToOrigin.x, positionRelativeToOrigin.y,0);
	m_targetSound.play();
}

void SoundManager::playWallImpactSound(sf::Vector2f position)
{
	m_wallImpactSound.play();
}

void SoundManager::updateListenerPostion(sf::Vector2f position, double rotation)
{
	m_listenerRotation = rotation;
	m_listenerPosition = position;
}
