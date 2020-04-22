#pragma once
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

enum class TexturesName {
	SPRITE_SHEET,
	SPRITE_SHEET_NIGHT,
	TARGET_HIT,
	TARGET_CATCH,
	LEVEL_BACKGROUND,
	LEVEL_BACKGROUND_NIGHT,
	SMOKED
	
};

enum class FontName {
	MAIN_FONT
};

enum class SoundBufferName {
	FIRE,
	TARGET_IMPACT,
	TARGET_CATCH,
	TARGET_START,
	WALL_IMPACT,
	PROJECTILE_FLY,
	MOTOR,
	DAY_SWITCH,
	TANK_IMPACT
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

	/// <summary>
	/// This function will return the Sound buffer corresponding 
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	static sf::SoundBuffer const& getSoundBuffer(SoundBufferName t_name, unsigned int damageLevel = 0);



private:

	/// <summary>
	/// This function loads all the resources into the ressource manager. 
	/// This function should be called Only ONE TIME before accessing a ressource 
	/// </summary>
	static void load();

private:
	static thor::ResourceHolder<sf::Texture, TexturesName> s_texturesHolder; // this will hold all the textures 
	static thor::ResourceHolder<sf::Font, FontName> s_fontHolder; // this will hold all the font
	static std::vector<thor::ResourceHolder<sf::SoundBuffer, SoundBufferName> > s_soundBuffers; // this will hold all the sound buffers classed by damage level

	static const unsigned int s_SOUND_DAMAGE_LEVEL;
	static bool s_loaded;

};

