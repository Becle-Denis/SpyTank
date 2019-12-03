#include "SoundSettings.h"

float SoundSettings::menuMusicVol()
{
	return (m_masterVol / 100) * (m_music_masterVol / 100) * m_music_menuMusicVol;
}

float SoundSettings::levelMusicVol()
{
	return (m_masterVol / 100) * (m_music_masterVol / 100)* m_music_levelMusicVol;
}

float SoundSettings::fireVol()
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_fireVol;
}

float SoundSettings::targetStartVol()
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_targetStartVol;
}

float SoundSettings::targetImpactVol()
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_targetStartVol;
}

float SoundSettings::wallImpactVol()
{
	return (m_masterVol / 100) * (m_fx_masterVol / 100) * m_fx_wallImpactVol;
}

bool SoundSettings::updateListenerPosition(sf::Vector2f& position)
{
	position = m_listener_basePosition;
	return m_listener_updatePosition;
}

bool SoundSettings::updateListenerRotation(double& orientation)
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

		settings.m_fx_fireVol = baseNode["fx"]["fireVolume"].as<float>();;
		settings.m_fx_targetStartVol = baseNode["fx"]["targetStartVolume"].as<float>();;
		settings.m_fx_targetImpactVol = baseNode["fx"]["targetImpactVolume"].as<float>();;
		settings.m_fx_wallImpactVol = baseNode["fx"]["wallImpactVolume"].as<float>();;

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
