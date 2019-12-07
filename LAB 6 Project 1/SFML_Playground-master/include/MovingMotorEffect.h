#pragma once
#include "SoundEffect.h"
#include "MovingSound.h"

struct SpatializedSound; //further declared 


class MovingMotorEffect : public SoundEffect
{
private: //Attributes 
	SpatializedSound* m_spSoundPtr;
	MovingSound m_movingEffect;
	float m_pitchCoef;
	float m_maxVol;

public: //functions 
	MovingMotorEffect(SpatializedSound* spSound,float maxVol, float pitchCoef = 1.5);

	void updateState(sf::Vector2f position, float speedPercentage);

	void setEnd();

	virtual bool updateEnd() override;
};

