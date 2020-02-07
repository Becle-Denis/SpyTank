#include "CrossFade.h"

CrossFade::CrossFade(sf::SoundSource& outSound, sf::SoundSource& inSound, int inVol, sf::Time fadeTime, bool stopMusic) :
	m_in(inSound, fadeTime, inVol),
	m_out(outSound,fadeTime,stopMusic)
	
{

}

bool CrossFade::updateEnd()
{
	bool inEnded = m_in.updateEnd();
	bool outEnded = m_out.updateEnd();
	return inEnded && outEnded;
}
