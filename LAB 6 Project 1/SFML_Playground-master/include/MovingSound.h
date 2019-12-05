#pragma once
#include "SoundEffect.h"

struct SpatializedSound; //further declared 


class MovingSound : public SoundEffect
{
private:
	SpatializedSound* m_spSoundPtr;
	bool m_ended{ false };

public:
	MovingSound(SpatializedSound* spSound);

	void updatePosition(sf::Vector2f position);

	void setEnd();

	virtual bool updateEnd() override;


};

