#include "SoundSettings.h"

float SoundSettings::menuMusicVol() const
{
	return (m_masterVol / 100) * (m_music_masterVol / 100) * m_music_menuMusicVol;
}

float SoundSettings::levelMusicVol() const
{
	return (m_masterVol / 100) * (m_music_masterVol / 100)* m_music_levelMusicVol;
}

float SoundSettings::fireVol() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_fireVol;
}

float SoundSettings::targetStartVol() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_targetStartVol;
}

float SoundSettings::targetImpactVol() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_targetStartVol;
}

float SoundSettings::wallImpactVol() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_wallImpactVol;
}

float SoundSettings::projectileVol() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_projectileVol;
}

float SoundSettings::motorMaxVolume() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_motorVol;
}

float SoundSettings::daySwitchVolume() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_daySwitch;
}

float SoundSettings::targetCatchVol() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_targetCatch;
}

float SoundSettings::tankImpactVol() const
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_tankImpact;
}


bool SoundSettings::updateListenerPosition(sf::Vector2f& position) const
{
	position = m_listener_basePosition;
	return m_listener_updatePosition;
}

bool SoundSettings::updateListenerRotation(double& orientation) const
{
	orientation = m_listener_baseRotation;
	return m_listener_updateRotation;
}

SoundSettings SoundSettings::loadGeneralSettings()
{
	return loadFromFile("./resources/sounds/settings/GeneralSoundSettings.yaml");
}

SoundSettings SoundSettings::loadFromFile(std::string filePath)
{
	SoundSettings settings;
	try
	{
		YAML::Node baseNode = YAML::LoadFile(filePath);
		if (baseNode.IsNull())
		{
			std::string message("Loading Sound Settings : File: " + filePath + " not found\nDefault Sounds settings loaded");
			throw std::exception(message.c_str());
		}

		//Magic here
		settings.m_masterVol = baseNode["general"]["masterVolume"].as<float>();
		settings.m_music_masterVol = baseNode["general"]["musicMasterVolume"].as<float>();
		settings.m_fx_masterVol = baseNode["general"]["fxMasterVolume"].as<float>();

		settings.m_listener_updatePosition = baseNode["listener"]["updatePosition"].as<bool>();
		settings.m_listener_updateRotation = baseNode["listener"]["updateRotation"].as<bool>();
		settings.m_listener_basePosition = sf::Vector2f(
			baseNode["listener"]["basePositionX"].as<float>(),
			baseNode["listener"]["basePositionY"].as<float>()
		);
		settings.m_listener_baseRotation = baseNode["listener"]["baseRotation"].as<float>();

		settings.m_music_menuMusicVol = baseNode["music"]["menuVolume"].as<float>();
		settings.m_music_levelMusicVol = baseNode["music"]["levelVolume"].as<float>();

		settings.m_fx_fireVol = baseNode["fx"]["fireVolume"].as<float>();
		settings.m_fx_targetStartVol = baseNode["fx"]["targetStartVolume"].as<float>(); 
		settings.m_fx_targetImpactVol = baseNode["fx"]["targetImpactVolume"].as<float>();
		settings.m_fx_targetCatch = baseNode["fx"]["targetCatchVolume"].as<float>();
		settings.m_fx_wallImpactVol = baseNode["fx"]["wallImpactVolume"].as<float>();
		settings.m_fx_projectileVol = baseNode["fx"]["projectileVolume"].as<float>();
		settings.m_fx_motorVol = baseNode["fx"]["motorMaximumVolume"].as<float>();
		settings.m_fx_daySwitch = baseNode["fx"]["daySwitchVolume"].as<float>();
		settings.m_fx_tankImpact = baseNode["fx"]["tankImpactVolume"].as<float>();

	}
	catch (YAML::ParserException& e)
	{
		std::string message(e.what());
		message = "Loading Sound Settings :  YAML Parser Error: " + message;
		std::cout << message << std::endl;
	}
	catch (std::exception& e)
	{
		std::string message(e.what());
		message = "Loading Sound Settings : Unexpected Error: " + message;
		std::cout << message << std::endl;
	}
	return settings; 
}
