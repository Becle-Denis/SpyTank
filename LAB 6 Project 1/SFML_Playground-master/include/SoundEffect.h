#pragma once
#include <SFML/Audio.hpp>

//Abstract class for sound effect
class SoundEffect
{
public:
	SoundEffect(sf::SoundSource& sound) :
		m_sound(sound)
	{ };

	virtual bool updateEnd() = 0;

	virtual ~SoundEffect() {};

protected:
	sf::SoundSource& m_sound;
};

