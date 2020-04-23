#include "SoundManager.h"

const sf::Time SoundManager::s_MUSIC_TRANSITION_TIME = sf::milliseconds(50);
const unsigned int SoundManager::s_DAMAGE_LEVELS = 4;

SoundManager::SoundManager() 
{
	std::string soundPath = "./resources/sounds/music/";
	std::string soundEnd = ".wav";
	//loading Musics 
	if (!m_menuMusic.openFromFile(soundPath + "SpyTank_Music_Menu_0DL" + soundEnd))
	{
		std::string s("Error loading music" + soundPath + "SpyTank_Music_Menu_0DL" + soundEnd);
		throw std::exception(s.c_str());
	}

	//creating musics arrays
	m_missionMusics = new sf::Music[s_DAMAGE_LEVELS + 1];
	m_attackMusics = new sf::Music[s_DAMAGE_LEVELS + 1];
	m_discoveredMusics = new sf::Music[s_DAMAGE_LEVELS + 1];

	//loadings musics in vectors 
	for (unsigned int damageLevel = 0; damageLevel <= s_DAMAGE_LEVELS; damageLevel++)
	{
		
		std::string fileEnd = "_" + std::to_string(damageLevel) + "DL" + soundEnd;
		if (!m_missionMusics[damageLevel].openFromFile(soundPath +  "SpyTank_Music_Night" + fileEnd))
		{
			std::string s("Error loading music" + soundPath +  "SpyTank_Music_Night" + fileEnd);
			throw std::exception(s.c_str());
		}
		if (!m_attackMusics[damageLevel].openFromFile(soundPath + "SpyTank_Music_Night_Attack" + fileEnd))
		{
			std::string s("Error loading music " + soundPath + "SpyTank_Music_Night_Attack" + fileEnd);
			throw std::exception(s.c_str());
		}
		if (!m_discoveredMusics[damageLevel].openFromFile(soundPath + "SpyTank_Music_Day" + fileEnd))
		{
			std::string s("Error loading music " + soundPath + "SpyTank_Music_Day" + fileEnd);
			throw std::exception(s.c_str());
		}
		
	}
	
	m_attackedTank = 0;
	m_damageLevel = 0;
	m_lightOn = true;
	
	//load settings 
	m_settings = SoundSettings::loadGeneralSettings();
	setSettings();
}

SoundManager::~SoundManager()
{

	delete[] m_missionMusics;
	delete[] m_attackMusics;
	delete[] m_discoveredMusics;
	
}

void SoundManager::startMusic()
{
	//launching all music in silence 
	m_menuMusic.setVolume(0);
	m_menuMusic.play();

	for (unsigned int damageLevel = 0; damageLevel <= s_DAMAGE_LEVELS; damageLevel++)
	{
		m_missionMusics[damageLevel].setVolume(0);
		m_missionMusics[damageLevel].play();
		m_attackMusics[damageLevel].setVolume(0);
		m_attackMusics[damageLevel].play();
		m_discoveredMusics[damageLevel].setVolume(0);
		m_discoveredMusics[damageLevel].play();
	}
	
	

	m_menuMusic.setVolume(m_settings.menuMusicVol());
}

void SoundManager::menu()
{
	m_missionInProgress = false;
	m_attackedTank = 0;
	m_damageLevel = 0;
	//mutting all musics
	for (unsigned int damageLevel = 0; damageLevel <= s_DAMAGE_LEVELS; damageLevel++)
	{
		m_missionMusics[damageLevel].setVolume(0);
		m_attackMusics[damageLevel].setVolume(0);
		m_discoveredMusics[damageLevel].setVolume(0);
	}

	switchToMenuMusic();
}

void SoundManager::mission()
{
	m_missionInProgress = true;
	m_attackedTank = 0;
	m_damageLevel = 0;
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
	m_missionMusics[m_damageLevel].setVolume(0);
	m_attackMusics[m_damageLevel].setVolume(0);
	m_discoveredMusics[m_damageLevel].setVolume(0);
}

void SoundManager::switchToAttackMusic()
{ 
	m_menuMusic.setVolume(0);
	m_missionMusics[m_damageLevel].setVolume(0);
	m_attackMusics[m_damageLevel].setVolume(m_settings.levelMusicVol());
	m_discoveredMusics[m_damageLevel].setVolume(0);
}

void SoundManager::switchToDiscoveredMusic()
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::DAY_SWITCH, m_damageLevel), m_settings.daySwitchVolume());
	m_menuMusic.setVolume(0);
	m_missionMusics[m_damageLevel].setVolume(0);
	m_attackMusics[m_damageLevel].setVolume(0);
	m_discoveredMusics[m_damageLevel].setVolume(m_settings.levelMusicVol());
}

void SoundManager::switchToMissionMusic()
{
	m_menuMusic.setVolume(0);
	m_missionMusics[m_damageLevel].setVolume(m_settings.levelMusicVol());
	m_attackMusics[m_damageLevel].setVolume(0);
	m_discoveredMusics[m_damageLevel].setVolume(0);
}

void SoundManager::increaseDamageLevel()
{
	if (m_settings.damagedMusicOn())
	{
		//updating damage level 
		int lastLevelDamage = m_damageLevel;
		m_damageLevel++;
		int newLevelDamage = m_damageLevel;

		//updating music volumes 
		if (m_missionMusics[lastLevelDamage].getVolume() != 0.0f)
		{
			m_missionMusics[newLevelDamage].setVolume(m_missionMusics[lastLevelDamage].getVolume());
			m_missionMusics[lastLevelDamage].setVolume(0.0f);
		}

		if (m_attackMusics[lastLevelDamage].getVolume() != 0.0f)
		{
			m_attackMusics[newLevelDamage].setVolume(m_attackMusics[lastLevelDamage].getVolume());
			m_attackMusics[lastLevelDamage].setVolume(0.0f);
		}

		if (m_discoveredMusics[lastLevelDamage].getVolume() != 0.0f)
		{
			m_discoveredMusics[newLevelDamage].setVolume(m_discoveredMusics[lastLevelDamage].getVolume());
			m_discoveredMusics[lastLevelDamage].setVolume(0.0f);
		}
	}
}




std::shared_ptr<MovingMotorEffect> SoundManager::tankMotorEffect()
{
	std::shared_ptr<SpatializedSound> motorSoundPtr = playSound(ResourcesManager::getSoundBuffer(SoundBufferName::MOTOR),sf::Vector2f());
	motorSoundPtr->sound.setLoop(true);
	std::shared_ptr<MovingMotorEffect> motorEffectPtr = std::make_shared<MovingMotorEffect>(motorSoundPtr, m_settings.motorMaxVolume());

	m_effectsInProgressPtr.push_back(motorEffectPtr);
	return motorEffectPtr;
}

std::shared_ptr<MovingSound> SoundManager::startProjectileSound(sf::Vector2f position)
{
	std::shared_ptr<SpatializedSound> soundPtr = playSound(ResourcesManager::getSoundBuffer(SoundBufferName::PROJECTILE_FLY, m_damageLevel), position, m_settings.projectileVol());
	soundPtr->sound.setLoop(true);
	std::shared_ptr<MovingSound> effectPtr = std::make_shared<MovingSound>(soundPtr);
	m_effectsInProgressPtr.push_back(effectPtr);
	return effectPtr;
}

void SoundManager::playFireSound(sf::Vector2f position, bool updatePosition)
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::FIRE, m_damageLevel), position, m_settings.fireVol(), updatePosition);
}


void SoundManager::playTargetSound(sf::Vector2f position, bool hitted)
{
	if (hitted)
	{
		playSound(ResourcesManager::getSoundBuffer(SoundBufferName::TARGET_IMPACT, m_damageLevel), position, m_settings.targetImpactVol());
	}
	else
	{
		playSound(ResourcesManager::getSoundBuffer(SoundBufferName::TARGET_CATCH, m_damageLevel), position, m_settings.targetCatchVol());
	}
}

void SoundManager::playTargetStartSound(sf::Vector2f position)
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::TARGET_START, m_damageLevel), position, m_settings.targetStartVol());
}

void SoundManager::playWallImpactSound(sf::Vector2f position)
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::WALL_IMPACT, m_damageLevel), position, m_settings.wallImpactVol());
}

void SoundManager::playTankImpact()
{
	playSound(ResourcesManager::getSoundBuffer(SoundBufferName::TANK_IMPACT, m_damageLevel), m_settings.tankImpactVol());
	increaseDamageLevel();
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
			//remove the effect 
			m_effectsInProgressPtr.erase(m_effectsInProgressPtr.begin() + i);
		}
	}
	//update the sounds Spatializations 
	for (int i = m_soundsInProgressPtr.size() - 1; i >= 0; i--)
	{
		updateSpatialisation(m_soundsInProgressPtr.at(i));
		//remove the non playing sounds 
		if (m_soundsInProgressPtr.at(i)->sound.getStatus() != sf::Sound::Status::Playing)
		{
			m_soundsInProgressPtr.erase(m_soundsInProgressPtr.begin() + i);
		}
	}

	//removes the finished non updated sound 
	for (int i = m_nonUpdatedSoundInProgressPtr.size() - 1; i >= 0; i--)
	{
		if (m_nonUpdatedSoundInProgressPtr.at(i)->sound.getStatus() != sf::Sound::Status::Playing)
		{
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
	m_menuMusic.setLoop(true);;
	//for each musics 
	for (unsigned int damageLevel = 0; damageLevel <= s_DAMAGE_LEVELS; damageLevel++)
	{
		m_missionMusics[damageLevel].setRelativeToListener(false);
		m_missionMusics[damageLevel].setLoop(true);
		m_attackMusics[damageLevel].setRelativeToListener(false);
		m_attackMusics[damageLevel].setLoop(true);
		m_discoveredMusics[damageLevel].setRelativeToListener(false);
		m_discoveredMusics[damageLevel].setLoop(true);
	}


}

std::shared_ptr<SpatializedSound> SoundManager::playSound(sf::SoundBuffer const& buffer, sf::Vector2f position, float volume, bool SpatializationUpdated)
{
	std::shared_ptr<SpatializedSound> soundPtr = std::make_shared<SpatializedSound>(buffer, position, volume);
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

std::shared_ptr<SpatializedSound> SoundManager::playSound(sf::SoundBuffer const& buffer, float volume)
{
	std::shared_ptr<SpatializedSound> soundPtr = std::make_shared<SpatializedSound>(buffer, sf::Vector2f(), volume);
	soundPtr->sound.setRelativeToListener(false);
	soundPtr->sound.play();
	m_nonUpdatedSoundInProgressPtr.push_back(soundPtr);
	return soundPtr;
}

void SoundManager::updateSpatialisation(std::shared_ptr<SpatializedSound> sound)
{
	sf::Vector2f positionRelativeToOrigin = sound->position - m_listenerPosition;
	thor::rotate<float>(positionRelativeToOrigin, -(m_listenerRotation + 90));
	sound->sound.setPosition(positionRelativeToOrigin.x, positionRelativeToOrigin.y, 0);
}
