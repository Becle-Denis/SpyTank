#pragma once
#include "SoundEffect.h"
#include "FadeIn.h"
#include "FadeOut.h"

class CrossFade : public SoundEffect
{
private:
	FadeIn m_in;
	FadeOut m_out;

public:
	CrossFade(sf::SoundSource& outSound, sf::SoundSource& inSound, int inVol, sf::Time fadeTime, bool stopMusic = true);

	virtual bool updateEnd() override;
};

