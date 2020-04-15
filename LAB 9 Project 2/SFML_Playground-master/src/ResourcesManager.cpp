#include "ResourcesManager.h"

thor::ResourceHolder<sf::Texture, TexturesName> ResourcesManager::s_texturesHolder;
thor::ResourceHolder<sf::Font, FontName> ResourcesManager::s_fontHolder;
thor::ResourceHolder<sf::SoundBuffer, SoundBufferName> ResourcesManager::s_soundBufferHolder;
bool ResourcesManager::s_loaded = false;

void ResourcesManager::load()
{
	//TEXTURES 
	//this variable will link the TextureName to the corresponding filepath 
	std::vector<std::pair<TexturesName, std::string>> textureFilesPath;

	// TExtures filepaths
	textureFilesPath.push_back({ TexturesName::SPRITE_SHEET, "./resources/images/SpriteSheet.png" });
	textureFilesPath.push_back({ TexturesName::TARGET, "./resources/images/RazorTarget1.png" });
	textureFilesPath.push_back({ TexturesName::LEVEL_BACKGROUND, "./resources/images/Background.jpg" });
	textureFilesPath.push_back({ TexturesName::SMOKED, "./resources/images/fumee.jpg" });


	//FONTS
	//this variable will link the FontName to the corresponding filepath 
	std::vector<std::pair<FontName, std::string>> fontsFilePath;

	// Fonts filepaths
	fontsFilePath.push_back({ FontName::MAIN_FONT, "./resources/fonts/8_bit_fortress.ttf" });

	// SOUND BUFFER 
	//This variable will link the SoundBufferName to the corresponding filepath
	std::vector<std::pair<SoundBufferName, std::string>> soundBuffersFilePaths;

	//Sound buffers filepaths 
	soundBuffersFilePaths.push_back({ SoundBufferName::FIRE, "./resources/sounds/fx/fire1.wav"});
	soundBuffersFilePaths.push_back({ SoundBufferName::MOTOR, "./resources/sounds/fx/Motor2.wav" });
	soundBuffersFilePaths.push_back({ SoundBufferName::PROJECTILE_FLY, "./resources/sounds/fx/TestPinkNoise.wav" });
	soundBuffersFilePaths.push_back({ SoundBufferName::TARGET_IMPACT, "./resources/sounds/fx/targetImpact2.wav" });
	soundBuffersFilePaths.push_back({ SoundBufferName::TARGET_START, "./resources/sounds/fx/TargetSonarStart.wav" });
	soundBuffersFilePaths.push_back({ SoundBufferName::WALL_IMPACT, "./resources/sounds/fx/WallImpact1.wav" });

	//loading each ressoruces 
	try
	{
		//loading the textures 
		for (std::pair<TexturesName, std::string> const& texturePair : textureFilesPath)
		{
			s_texturesHolder.acquire(texturePair.first, thor::Resources::fromFile<sf::Texture>(texturePair.second));
		}

		//loading the fonts  
		for (std::pair<FontName, std::string> const& texturePair : fontsFilePath)
		{
			s_fontHolder.acquire(texturePair.first, thor::Resources::fromFile<sf::Font>(texturePair.second));
		}

		//loading the sound buffers 
		for (std::pair<SoundBufferName, std::string> const& soundBufferPair : soundBuffersFilePaths)
		{
			s_soundBufferHolder.acquire(soundBufferPair.first, thor::Resources::fromFile<sf::SoundBuffer>(soundBufferPair.second));
		}
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << "Error loading the ressources : " << e.what() << std::endl;
	}

	s_loaded = true;
}

sf::Texture const& ResourcesManager::getTexture(TexturesName t_name)
{
	if (!s_loaded)
	{
		load();
	}
	return s_texturesHolder[t_name];
}

sf::Font const& ResourcesManager::getFont(FontName t_name)
{
	if (!s_loaded)
	{
		load();
	}
	return s_fontHolder[t_name];
}

sf::SoundBuffer const& ResourcesManager::getSoundBuffer(SoundBufferName t_name)
{
	if (!s_loaded)
	{
		load();
	}
	return s_soundBufferHolder[t_name];
}
