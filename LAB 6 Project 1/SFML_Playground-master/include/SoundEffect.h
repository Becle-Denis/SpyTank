#pragma once
#include <SFML/Audio.hpp>

//Abstract class for sound effect
class SoundEffect
{
public:
	SoundEffect() {};

	virtual bool updateEnd() = 0;

	virtual ~SoundEffect() {};

};

