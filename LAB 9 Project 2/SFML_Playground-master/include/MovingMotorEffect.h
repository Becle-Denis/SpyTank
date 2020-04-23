#pragma once
#include "SoundEffect.h"
#include "MovingSound.h"

struct SpatializedSound; //further declared 

// Sound effect capable of simulate a motor sound,
// THe volume and pitch of the sound will be updated with the speed percentage 
// Position updatable 
class MovingMotorEffect : public SoundEffect
{
private: //Attributes 
	std::shared_ptr<SpatializedSound> m_spSoundPtr;
	MovingSound m_movingEffect;
	float m_pitchCoef;
	float m_maxVol;

public: //functions 
	MovingMotorEffect(std::shared_ptr<SpatializedSound> spSound,float maxVol, float pitchCoef = 1.5);

	// Position : Game local position
	// Speed percentage, Must be POSITIVE, 
	void updateState(sf::Vector2f position, float speedPercentage);

	//Set the end of the effect, stop the sound, the object may be deleted after this 
	void setEnd();

	virtual bool updateEnd() override;
};

