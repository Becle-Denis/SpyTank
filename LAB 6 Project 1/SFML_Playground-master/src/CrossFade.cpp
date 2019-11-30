#include "CrossFade.h"

CrossFade::CrossFade(sf::SoundSource& outSound, sf::SoundSource& inSound, int inVol, sf::Time fadeTime, bool stopMusic)
	: SoundEffect(outSound),
	m_in(inSound, fadeTime, inVol),
	m_out(outSound,fadeTime,stopMusic)
	
{

}

bool CrossFade::updateEnd()
{
	return m_in.updateEnd() && m_out.updateEnd();
}
