#include "SoundManager.h"

SoundManager::SoundManager(std::string filePath)
{
	//loading buffers and Musics 
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

	if (!m_projectileBuffer.loadFromFile("./resources/sounds/fx/TestPinkNoise.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/TestPinkNoise.wav");
		throw std::exception(s.c_str());
	}

	if (!m_tankMotorBuffer.loadFromFile("./resources/sounds/fx/Motor2.wav"))
	{
		std::string s("Error loading music ./resources/sounds/fx/Motor2.wav");
		throw std::exception(s.c_str());
	}
	
	
	//load settings 
	m_settings = SoundSettings::loadGeneralSettings();
	setSettings();
}

SoundManager::~SoundManager()
{
	for (SoundEffect* e : m_effectsInProgressPtr)
	{
		delete e;
		e = nullptr;
	}
	for (SpatializedSound* s : m_soundsInProgressPtr)
	{
		delete s;
		s = nullptr;
	}
}


void SoundManager::startMenuMusic()
{
	m_menuMusic.setLoop(true);
	m_effectsInProgressPtr.push_back(new FadeIn(m_menuMusic, sf::seconds(5), m_settings.menuMusicVol()));
}

void SoundManager::switchToLevelMusic()
{
	m_levelMusic.setLoop(true);
	m_effectsInProgressPtr.push_back(new CrossFade(m_menuMusic, m_levelMusic, m_settings.menuMusicVol(), sf::seconds(0.25)));
}

void SoundManager::switchToMenuMusic()
{
	m_effectsInProgressPtr.push_back(new CrossFade(m_levelMusic, m_menuMusic, m_settings.levelMusicVol(), sf::seconds(0.25)));
}


MovingMotorEffect* SoundManager::tankMotorEffect()
{
	SpatializedSound* motorSoundPtr = playSound(m_tankMotorBuffer,sf::Vector2f());
	motorSoundPtr->sound.setLoop(true);
	MovingMotorEffect* motorEffectPtr = new MovingMotorEffect(motorSoundPtr, m_settings.motorMaxVolume());

	m_effectsInProgressPtr.push_back(motorEffectPtr);
	return motorEffectPtr;
}

MovingSound* SoundManager::startProjectileSound(sf::Vector2f position)
{
	SpatializedSound* soundPtr = playSound(m_projectileBuffer, position, m_settings.projectileVol());
	soundPtr->sound.setLoop(true);
	MovingSound* effectPtr = new MovingSound(soundPtr);
	m_effectsInProgressPtr.push_back(effectPtr);
	return effectPtr;
}

void SoundManager::playFireSound(sf::Vector2f position)
{
	playSound(m_fireBuffer, position, m_settings.fireVol(),false);
}

void SoundManager::playTargetImpactSound(sf::Vector2f position)
{
	playSound(m_targetImpactBuffer,position,m_settings.targetImpactVol());
}

void SoundManager::playTargetStartSound(sf::Vector2f position)
{
	playSound(m_targetStartBuffer, position, m_settings.targetStartVol());
}

void SoundManager::playWallImpactSound(sf::Vector2f position)
{
	playSound(m_wallImpactBuffer, position, m_settings.wallImpactVol());
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
			m_effectsInProgressPtr.at(i) = nullptr;
			m_effectsInProgressPtr.erase(m_effectsInProgressPtr.begin() + i);
		}
	}
	//update the sounds Spatializations 
	for (int i = m_soundsInProgressPtr.size() - 1; i >= 0; i--)
	{
		updateSpatialisation(m_soundsInProgressPtr.at(i));
		//delete the non playing sounds 
		if (m_soundsInProgressPtr.at(i)->sound.getStatus() != sf::Sound::Status::Playing)
		{
			delete m_soundsInProgressPtr.at(i);
			m_soundsInProgressPtr.at(i) = nullptr;
			m_soundsInProgressPtr.erase(m_soundsInProgressPtr.begin() + i);
		}
	}

	//delete the finished non updated sound 
	for (int i = m_nonUpdatedSoundInProgressPtr.size() - 1; i >= 0; i--)
	{
		if (m_nonUpdatedSoundInProgressPtr.at(i)->sound.getStatus() != sf::Sound::Status::Playing)
		{
			delete m_nonUpdatedSoundInProgressPtr.at(i);
			m_nonUpdatedSoundInProgressPtr.at(i) = nullptr;
			m_nonUpdatedSoundInProgressPtr.erase(m_nonUpdatedSoundInProgressPtr.begin() + i);
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

	//mixing 
	m_levelMusic.setVolume(m_settings.levelMusicVol());
	m_menuMusic.setVolume(m_settings.menuMusicVol());

}

SpatializedSound* SoundManager::playSound(sf::SoundBuffer const& buffer, sf::Vector2f position, float volume, bool SpatializationUpdated)
{
	SpatializedSound* soundPtr = new SpatializedSound(buffer, position, volume);
	updateSpatialisation(soundPtr);
	soundPtr->sound.play();
	if (SpatializationUpdated)
	{
		m_soundsInProgressPtr.push_back(soundPtr);
	}
	else
	{
		m_nonUpdatedSoundInProgressPtr.push_back(soundPtr);
	}
	return soundPtr;
}

void SoundManager::updateSpatialisation(SpatializedSound* sound)
{
	sf::Vector2f positionRelativeToOrigin = sound->position - m_listenerPosition;
	thor::rotate<float>(positionRelativeToOrigin, -(m_listenerRotation + 90));
	sound->sound.setPosition(positionRelativeToOrigin.x, positionRelativeToOrigin.y, 0);
}
