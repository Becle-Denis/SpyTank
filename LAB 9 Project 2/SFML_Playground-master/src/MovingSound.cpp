#include "MovingSound.h"
#include "SoundManager.h"

MovingSound::MovingSound(std::shared_ptr<SpatializedSound> spSound)
	: m_spSoundPtr(spSound)
{

}

void MovingSound::updatePosition(sf::Vector2f position)
{
	m_spSoundPtr->position = position;
}

void MovingSound::setEnd()
{
	m_spSoundPtr->sound.stop();
	m_ended = true;
}

bool MovingSound::updateEnd()
{
	return m_ended;
}
