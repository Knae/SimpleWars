#pragma once
#ifndef __AUDIOMGR_H__
#define __AUDIOMGR_H__

#include <SFML/Audio.hpp>
#include <vector>

//Each unit represents a sound 
// that can be played simultaneously
struct SoundUnit
{
	sf::SoundBuffer soundBuffer;
	sf::Sound soundPlayer;
};

enum RESULT
{
	SUCCESS,
	FAIL,
	NO_SLOT
};

class CAudioManager
{
	public:
		static CAudioManager& getRef()
		{
			static CAudioManager instance; return instance;
		}
		
		~CAudioManager();

		RESULT AddSoundEvent(std::string& inSoundFile);

	private:
		CAudioManager();

		CAudioManager(CAudioManager const&) = delete;
		void operator=(CAudioManager const&) = delete;

		sf::Music m_backgroundMusic;
		std::vector<SoundUnit*> m_vecSoundUnits;

		float m_fDefaultVolume = 25.0f;
		int m_iMaxSoundEvents = 20;
};

#endif // __AUDIOMGR_H__