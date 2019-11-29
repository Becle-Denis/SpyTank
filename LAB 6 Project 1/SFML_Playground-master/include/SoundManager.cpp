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
	
	if (!m_targetImpactBuffer.loadFromFile("./resources/sounds/fx/targetImpact2.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/targetImpact2.wav");
		throw std::exception(s.c_str());
	}
	
	if (!m_targetStartBuffer.loadFromFile("./resources/sounds/fx/TargetSonarStart.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/TargetSonarStart.wav");
		throw std::exception(s.c_str());
	}
	
	if (!m_wallImpactBuffer.loadFromFile("./resources/sounds/fx/WallImpact1.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/WallImpact1.wav");
		throw std::exception(s.c_str());
	}
	
	//setting buffer
	m_fireSound.setBuffer(m_fireBuffer);
	m_targetImpactSound.setBuffer(m_targetImpactBuffer);
	m_targetStartSound.setBuffer(m_targetStartBuffer);
	m_wallImpactSound.setBuffer(m_wallImpactBuffer);

	//setting relative 
	m_levelMusic.setRelativeToListener(false);
	m_menuMusic.setRelativeToListener(false);
	
	m_targetImpactSound.setRelativeToListener(true);
	m_targetStartSound.setRelativeToListener(true);
	m_wallImpactSound.setRelativeToListener(true);


	//setting distances 
	m_targetImpactSound.setMinDistance(100);
	m_targetStartSound.setMinDistance(100);
	m_wallImpactSound.setMinDistance(100);

	//mixing 
	m_levelMusic.setVolume(70);
	m_menuMusic.setVolume(80);

	m_fireSound.setVolume(100);
	m_targetImpactSound.setVolume(100);
	m_targetStartSound.setVolume(100);
	m_wallImpactSound.setVolume(100);

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

void SoundManager::playTargetImpactSound(sf::Vector2f position)
{
	playSound(m_targetImpactSound, position);
}

void SoundManager::playTargetStartSound(sf::Vector2f position)
{
	playSound(m_targetStartSound, position);
}

void SoundManager::playWallImpactSound(sf::Vector2f position)
{
	playSound(m_wallImpactSound, position);
}

void SoundManager::updateListenerPostion(sf::Vector2f position, double rotation)
{
	m_listenerRotation = rotation;
	m_listenerPosition = position;
}

void SoundManager::playSound(sf::Sound& sound, sf::Vector2f position)
{
	sf::Vector2f positionRelativeToOrigin = position - m_listenerPosition;
	thor::rotate<float>(positionRelativeToOrigin, -(m_listenerRotation + 90));
	sound.setPosition(positionRelativeToOrigin.x, positionRelativeToOrigin.y, 0);
	sound.play();
}
