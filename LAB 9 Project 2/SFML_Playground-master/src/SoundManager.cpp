#include "SoundManager.h"

const sf::Time SoundManager::s_MUSIC_TRANSITION_TIME = sf::milliseconds(50);

SoundManager::SoundManager()
{

	//loading Musics 
	if (!m_menuMusic.openFromFile("./resources/sounds/music/SpyTank_Music_Menu.wav"))
	{
		std::string s("Error loading music ./resources/sounds/music/SpyTank_Music_Menu.wav");
		throw std::exception(s.c_str());
	}
	if (!m_missionMusic.openFromFile("./resources/sounds/music/SpyTank_Music_Night.wav"))
	{
		std::string s("Error loading music ./resources/sounds/music/SpyTank_Music_Night.wav");
		throw std::exception(s.c_str());
	}
	if (!m_attackMusic.openFromFile("./resources/sounds/music/SpyTank_Music_Night_Attack.wav"))
	{
		std::string s("Error loading music ./resources/sounds/music/SpyTank_Music_Night_Attack.wav");
		throw std::exception(s.c_str());
	}
	if (!m_discoveredMusic.openFromFile("./resources/sounds/music/SpyTank_Music_Day.wav"))
	{
		std::string s("Error loading music ./resources/sounds/music/SpyTank_Music_Day.wav");
		throw std::exception(s.c_str());
	}
	
	m_attackedTank = 0;
	m_lightOn = true;
	
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

void SoundManager::startMusic()
{
	//launching all music in silence 
	m_menuMusic.setVolume(0);
	m_missionMusic.setVolume(0);
	m_attackMusic.setVolume(0);
	m_discoveredMusic.setVolume(0);

	m_menuMusic.play();
	m_missionMusic.play();
	m_attackMusic.play();
	m_discoveredMusic.play();

	m_menuMusic.setVolume(m_settings.menuMusicVol());
}

void SoundManager::menu()
{
	m_missionInProgress = false;
	m_attackedTank = 0;
	switchToMenuMusic();
}

void SoundManager::mission()
{
	m_missionInProgress = true;
	m_attackedTank = 0;
	switchToMissionMusic();
}

void SoundManager::lightOn()
{
	m_lightOn = true;
	if (m_missionInProgress)
	{
		switchToDiscoveredMusic();
	}
}

void SoundManager::lightOff()
{
	m_lightOn = false;
	if (m_missionInProgress)
	{
		if (m_attackedTank > 0)
		{
			switchToAttackMusic();
		}
		else
		{
			switchToMissionMusic();
		}
	}

}

void SoundManager::tankAttacked()
{
	m_attackedTank++;
	if (!m_lightOn)
	{
		if (m_attackedTank > 0)
		{
			switchToAttackMusic();
		}
		else
		{
			switchToMissionMusic();
		}
	}
}

void SoundManager::tanksafe()
{
	m_attackedTank--;
	if (!m_lightOn)
	{
		if (m_attackedTank > 0)
		{
			switchToAttackMusic();
		}
		else
		{
			switchToMissionMusic();
		}
	}
}

void SoundManager::switchToMenuMusic()
{
	m_menuMusic.setVolume(m_settings.menuMusicVol());
	m_missionMusic.setVolume(0);
	m_attackMusic.setVolume(0);
	m_discoveredMusic.setVolume(0);
}

void SoundManager::switchToAttackMusic()
{ 
	m_menuMusic.setVolume(0);
	m_missionMusic.setVolume(0);
	m_attackMusic.setVolume(m_settings.levelMusicVol());
	m_discoveredMusic.setVolume(0);
}

void SoundManager::switchToDiscoveredMusic()
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::DAY_SWITCH), m_settings.daySwitchVolume());
	m_menuMusic.setVolume(0);
	m_missionMusic.setVolume(0);
	m_attackMusic.setVolume(0);
	m_discoveredMusic.setVolume(m_settings.levelMusicVol());
}

void SoundManager::switchToMissionMusic()
{
	m_menuMusic.setVolume(0);
	m_missionMusic.setVolume(m_settings.levelMusicVol());
	m_attackMusic.setVolume(0);
	m_discoveredMusic.setVolume(0);
}




MovingMotorEffect* SoundManager::tankMotorEffect()
{
	SpatializedSound* motorSoundPtr = playSound(ResourcesManager::getSoundBuffer(SoundBufferName::MOTOR),sf::Vector2f());
	motorSoundPtr->sound.setLoop(true);
	MovingMotorEffect* motorEffectPtr = new MovingMotorEffect(motorSoundPtr, m_settings.motorMaxVolume());

	m_effectsInProgressPtr.push_back(motorEffectPtr);
	return motorEffectPtr;
}

MovingSound* SoundManager::startProjectileSound(sf::Vector2f position)
{
	SpatializedSound* soundPtr = playSound(ResourcesManager::getSoundBuffer(SoundBufferName::PROJECTILE_FLY), position, m_settings.projectileVol());
	soundPtr->sound.setLoop(true);
	MovingSound* effectPtr = new MovingSound(soundPtr);
	m_effectsInProgressPtr.push_back(effectPtr);
	return effectPtr;
}

void SoundManager::playFireSound(sf::Vector2f position)
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::FIRE), position, m_settings.fireVol(),false);
}

void SoundManager::playTargetImpactSound(sf::Vector2f position)
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::TARGET_IMPACT),position,m_settings.targetImpactVol());
}

void SoundManager::playTargetStartSound(sf::Vector2f position)
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::TARGET_START), position, m_settings.targetStartVol());
}

void SoundManager::playWallImpactSound(sf::Vector2f position)
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::WALL_IMPACT), position, m_settings.wallImpactVol());
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
	m_menuMusic.setRelativeToListener(false);
	m_menuMusic.setLoop(true);
	m_missionMusic.setRelativeToListener(false);
	m_missionMusic.setLoop(true);
	m_attackMusic.setRelativeToListener(false);
	m_attackMusic.setLoop(true);
	m_discoveredMusic.setRelativeToListener(false);
	m_discoveredMusic.setLoop(true);

	//mixing 
	m_menuMusic.setVolume(m_settings.menuMusicVol());
	m_missionMusic.setVolume(m_settings.levelMusicVol());
	m_attackMusic.setVolume(m_settings.levelMusicVol());
	m_discoveredMusic.setVolume(m_settings.levelMusicVol());


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

SpatializedSound* SoundManager::playSound(sf::SoundBuffer const& buffer, float volume)
{
	SpatializedSound* soundPtr = new SpatializedSound(buffer, sf::Vector2f(), volume);
	soundPtr->sound.setRelativeToListener(false);
	soundPtr->sound.play();
	m_nonUpdatedSoundInProgressPtr.push_back(soundPtr);
	return soundPtr;
}

void SoundManager::updateSpatialisation(SpatializedSound* sound)
{
	sf::Vector2f positionRelativeToOrigin = sound->position - m_listenerPosition;
	thor::rotate<float>(positionRelativeToOrigin, -(m_listenerRotation + 90));
	sound->sound.setPosition(positionRelativeToOrigin.x, positionRelativeToOrigin.y, 0);
}
