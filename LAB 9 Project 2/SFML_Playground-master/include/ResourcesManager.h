#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

enum class TexturesName {
	SPRITE_SHEET,
	TARGET,
	LEVEL_BACKGROUND,
	SMOKED
	
};

enum class FontName {
	MAIN_FONT
};

// This SINGLETON class will hold the principal ressources of the Game 
// It will use the Thor ressource manager 
class ResourcesManager
{
public: //API static functions

	/// <summary>
	/// This function will return the texture corresponding
	/// </summary>
	/// <param name="name">Name of the texture </param>
	/// <returns>const reference to the texture</returns>
	static sf::Texture const& getTexture(TexturesName t_name);

	/// <summary>
	/// This function will return the font corresponding 
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	static sf::Font const& getFont(FontName t_name);


private:

	/// <summary>
	/// This function loads all the resources into the ressource manager. 
	/// This function should be called Only ONE TIME before accessing a ressource 
	/// </summary>
	static void load();

private:
	static thor::ResourceHolder<sf::Texture, TexturesName> s_texturesHolder; // this will hold all the textures 
	static thor::ResourceHolder<sf::Font, FontName> s_fontHolder; // this will hold all the font

	static bool s_loaded;

};

