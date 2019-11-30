#pragma once
class SoundSettings
{
private: //Attributes 
	//General 
	float m_masterVol{ 100 };
	float m_music_masterVol{ 100 };
	float m_fx_masterVol{ 100 };
	
	//Music
	float m_music_menuMusicVol{ 80 };
	float m_music_levelMusicVol{ 70 };

	//fx
	float m_fx_fireVol{ 100 };
	float m_fx_targetStartVol{ 80 };
	float m_fx_targetImpactVol{ 100 };
	float m_fx_wallImpactVol{ 100 };

public: //functions
	SoundSettings() = default;

	//accesser 
	float menuMusicVol();
	float levelMusicVol();
	float fireVol();
	float targetStartVol();
	float targetImpactVol();
	float wallImpactVol();

};

