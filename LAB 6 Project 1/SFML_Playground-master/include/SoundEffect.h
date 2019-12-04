#pragma once
#include <SFML/Audio.hpp>

//Interface for sound effect
class SoundEffect
{
public:
	SoundEffect() {};

	/// <summary>
	/// update the effect 
	/// </summary>
	/// <returns>True if the effect is ended and can be deleted</returns>
	virtual bool updateEnd() = 0;

	virtual ~SoundEffect() {};

};

