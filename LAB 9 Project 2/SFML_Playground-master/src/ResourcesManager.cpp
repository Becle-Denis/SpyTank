#include "ResourcesManager.h"

thor::ResourceHolder<sf::Texture, TexturesName> ResourcesManager::s_texturesHolder;
thor::ResourceHolder<sf::Font, FontName> ResourcesManager::s_fontHolder;
std::vector<thor::ResourceHolder<sf::SoundBuffer, SoundBufferName>> ResourcesManager::s_soundBuffers;
bool ResourcesManager::s_loaded = false;
const unsigned int ResourcesManager::s_SOUND_DAMAGE_LEVEL = 4;

void ResourcesManager::load()
{
	//TEXTURES 
	//this variable will link the TextureName to the corresponding filepath 
	std::vector<std::pair<TexturesName, std::string>> textureFilesPath;

	// TExtures filepaths
	textureFilesPath.push_back({ TexturesName::SPRITE_SHEET, "./resources/images/SpriteSheet.png" });
	textureFilesPath.push_back({ TexturesName::SPRITE_SHEET_NIGHT, "./resources/images/SpriteSheet_night.png" });
	textureFilesPath.push_back({ TexturesName::TARGET_HIT, "./resources/images/RazorTarget1.png" });
	textureFilesPath.push_back({ TexturesName::TARGET_CATCH, "./resources/images/LumsTarget1.png" });
	textureFilesPath.push_back({ TexturesName::LEVEL_BACKGROUND, "./resources/images/Background.jpg" });
	textureFilesPath.push_back({ TexturesName::LEVEL_BACKGROUND_NIGHT, "./resources/images/Background_night.jpg" });
	textureFilesPath.push_back({ TexturesName::SMOKED, "./resources/images/fumee.jpg" });


	//FONTS
	//this variable will link the FontName to the corresponding filepath 
	std::vector<std::pair<FontName, std::string>> fontsFilePath;

	// Fonts filepaths
	fontsFilePath.push_back({ FontName::MAIN_FONT, "./resources/fonts/8_bit_fortress.ttf" });

	// SOUND BUFFER 
	//This variable will link the SoundBufferName to the corresponding names 
	std::string soundPath = "./resources/sounds/fx/";
	std::string soundEnd = ".wav";
	std::vector<std::pair<SoundBufferName, std::string>> soundBuffersFileNames;

	//Sound buffers filepaths 
	soundBuffersFileNames.push_back({ SoundBufferName::FIRE, "fire3"});
	soundBuffersFileNames.push_back({ SoundBufferName::MOTOR, "Motor2" });
	soundBuffersFileNames.push_back({ SoundBufferName::PROJECTILE_FLY, "projectile2" });
	soundBuffersFileNames.push_back({ SoundBufferName::TARGET_IMPACT, "targetImpact2" });
	soundBuffersFileNames.push_back({ SoundBufferName::TARGET_CATCH, "targetCatch2" });
	soundBuffersFileNames.push_back({ SoundBufferName::TARGET_START, "targetStart2" });
	soundBuffersFileNames.push_back({ SoundBufferName::WALL_IMPACT, "WallImpact1" });
	soundBuffersFileNames.push_back({ SoundBufferName::DAY_SWITCH, "daySwitch1" });
	soundBuffersFileNames.push_back({ SoundBufferName::TANK_IMPACT, "tankImpact1" });

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
		//creating the ressource holders 
		s_soundBuffers.resize(s_SOUND_DAMAGE_LEVEL + 1);
		for (unsigned int levelDamage = 0; levelDamage <= s_SOUND_DAMAGE_LEVEL; levelDamage++)
		{ 
			for (std::pair<SoundBufferName, std::string> const& soundBufferPair : soundBuffersFileNames)
			{
				s_soundBuffers.at(levelDamage).acquire(
					soundBufferPair.first, 
					thor::Resources::fromFile<sf::SoundBuffer>(
						soundPath + soundBufferPair.second + "_" + std::to_string(levelDamage) + "DL" + soundEnd));
			}
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

sf::SoundBuffer const& ResourcesManager::getSoundBuffer(SoundBufferName t_name, unsigned int damageLevel)
{
	if (!s_loaded)
	{
		load();
	}

	if (damageLevel > s_SOUND_DAMAGE_LEVEL)
	{
		damageLevel = s_SOUND_DAMAGE_LEVEL;
	}

	return s_soundBuffers.at(damageLevel)[t_name];
}
