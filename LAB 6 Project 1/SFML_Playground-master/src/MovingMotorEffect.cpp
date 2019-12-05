#include "MovingMotorEffect.h"
#include "SoundManager.h"

MovingMotorEffect::MovingMotorEffect(SpatializedSound* spSound, float maxVol, float pitchCoef) :
	m_spSoundPtr(spSound),m_movingEffect(spSound),m_pitchCoef(pitchCoef), m_maxVol(maxVol)
{
}

void MovingMotorEffect::updateState(sf::Vector2f position, float speedPercentage)
{
	m_movingEffect.updatePosition(position);
	m_spSoundPtr->sound.setPitch(speedPercentage * m_pitchCoef);
	m_spSoundPtr->sound.setVolume(m_maxVol * speedPercentage);
}

void MovingMotorEffect::setEnd()
{
	m_movingEffect.setEnd();
}

bool MovingMotorEffect::updateEnd()
{
	return m_movingEffect.updateEnd();
}
