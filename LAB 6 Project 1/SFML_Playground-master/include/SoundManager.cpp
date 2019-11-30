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

	setSettings();
}


void SoundManager::startMenuMusic()
{
	m_menuMusic.setLoop(true);
	m_effectsInProgressPtr.push_back(new FadeIn(m_menuMusic, sf::seconds(5), m_settings.menuMusicVol()));
}

void SoundManager::switchToLevelMusic()
{
	m_levelMusic.setLoop(true);
	m_effectsInProgressPtr.push_back(new FadeIn(m_levelMusic, sf::seconds(0.25), m_settings.menuMusicVol()));
	m_effectsInProgressPtr.push_back(new FadeOut(m_menuMusic, sf::seconds(0.25)));
	
}

void SoundManager::switchToMenuMusic()
{
	m_effectsInProgressPtr.push_back(new FadeIn(m_menuMusic, sf::seconds(0.25), m_settings.levelMusicVol()));
	m_effectsInProgressPtr.push_back(new FadeOut(m_levelMusic, sf::seconds(0.25)));
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
	if (m_updatePosition)
	{
		m_listenerPosition = position;
	}
	if (m_updateRotation)
	{
		m_listenerRotation = rotation;
	}
	
}

void SoundManager::update()
{
	//update the audio effets 
	for (int i = m_effectsInProgressPtr.size() - 1; i >= 0; i--)
	{
		if (m_effectsInProgressPtr.at(i)->updateEnd())
		{
			//delete the effect 
			delete m_effectsInProgressPtr.at(i);
			m_effectsInProgressPtr.erase(m_effectsInProgressPtr.begin() + i);
		}
	}
}

void SoundManager::setSettings()
{
	//setting Listener 
	sf::Vector2f listenerPosition;
	if (m_settings.updateListenerPosition(listenerPosition))
	{
		m_updatePosition = true;
	}
	else
	{
		m_listenerPosition = listenerPosition;
		m_updatePosition = false;
	}

	double rotation;
	if (m_settings.updateListenerRotation(rotation))
	{
		m_updateRotation = true;
	}
	else
	{
		m_listenerRotation = rotation;
		m_updateRotation = false;
	}


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
	m_levelMusic.setVolume(m_settings.levelMusicVol());
	m_menuMusic.setVolume(m_settings.menuMusicVol());


	m_fireSound.setVolume(m_settings.fireVol());
	m_targetImpactSound.setVolume(m_settings.targetImpactVol());
	m_targetStartSound.setVolume(m_settings.targetStartVol());
	m_wallImpactSound.setVolume(m_settings.wallImpactVol());


}

void SoundManager::playSound(sf::Sound& sound, sf::Vector2f position)
{
	sf::Vector2f positionRelativeToOrigin = position - m_listenerPosition;
	thor::rotate<float>(positionRelativeToOrigin, -(m_listenerRotation + 90));
	sound.setPosition(positionRelativeToOrigin.x, positionRelativeToOrigin.y, 0);
	sound.play();
}
