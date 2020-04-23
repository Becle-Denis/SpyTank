#pragma once
#include "SoundEffect.h"

struct SpatializedSound; //further declared 

// Sound effect capable of update the local position of a Sound 
class MovingSound : public SoundEffect
{
private:
	std::shared_ptr<SpatializedSound> m_spSoundPtr;
	bool m_ended{ false };

public:
	MovingSound(std::shared_ptr<SpatializedSound> spSound);

	void updatePosition(sf::Vector2f position);

	//Set the end of the effect, stop the sound, the object may be deleted after this 
	void setEnd();

	virtual bool updateEnd() override;


};

