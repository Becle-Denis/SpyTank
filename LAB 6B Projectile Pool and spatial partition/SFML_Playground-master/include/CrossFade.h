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
	/// <summary>
	/// Create a CrossFade effect between to sounds 
	/// </summary>
	/// <param name="outSound">Fade out sound</param>
	/// <param name="inSound">Fade in sound</param>
	/// <param name="inVol">Fade in final volume</param>
	/// <param name="fadeTime">Time of the crossfade</param>
	/// <param name="stopOutSound">True if the fadeOut sound should be stopped</param>
	CrossFade(sf::SoundSource& outSound, sf::SoundSource& inSound, int inVol, sf::Time fadeTime, bool stopOutSound = true);

	virtual bool updateEnd() override;
};

