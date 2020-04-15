#include "ResourcesManager.h"

thor::ResourceHolder<sf::Texture, TexturesName> ResourcesManager::s_texturesHolder;
thor::ResourceHolder<sf::Font, FontName> ResourcesManager::s_fontHolder;
bool ResourcesManager::s_loaded = false;

void ResourcesManager::load()
{
	//TEXTURES 
	//this variable will link the TextureName to the corresponding filepath 
	std::vector<std::pair<TexturesName, std::string>> texturesFilesPath;

	// TExtures filepaths
	texturesFilesPath.push_back({ TexturesName::SPRITE_SHEET, "./resources/images/SpriteSheet.png" });
	texturesFilesPath.push_back({ TexturesName::TARGET, "./resources/images/RazorTarget1.png" });
	texturesFilesPath.push_back({ TexturesName::LEVEL_BACKGROUND, "./resources/images/Background.jpg" });
	texturesFilesPath.push_back({ TexturesName::SMOKED, "./resources/images/fumee.jpg" });


	//FONTS
	//this variable will link the FontName to the corresponding filepath 
	std::vector<std::pair<FontName, std::string>> fontsFilesPath;

	// ADD NEW FONT HERE
	fontsFilesPath.push_back({ FontName::MAIN_FONT, "./resources/fonts/8_bit_fortress.ttf" });


	//loading each ressoruces 
	try
	{
		//loading the textures 
		for (std::pair<TexturesName, std::string> const& texturePair : texturesFilesPath)
		{
			s_texturesHolder.acquire(texturePair.first, thor::Resources::fromFile<sf::Texture>(texturePair.second));
		}

		//loading the fonts  
		for (std::pair<FontName, std::string> const& texturePair : fontsFilesPath)
		{
			s_fontHolder.acquire(texturePair.first, thor::Resources::fromFile<sf::Font>(texturePair.second));
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
