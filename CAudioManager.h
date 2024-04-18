#pragma once
#ifndef __AUDIOMGR_H__
#define __AUDIOMGR_H__

#include <SFML/Audio.hpp>
#include <vector>

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
		static CAudioManager& getInstance()
		{
			static CAudioManager instance; return instance;
		}
		
		~CAudioManager();

		RESULT AddSoundEvent(std::string& inSoundFile);

	private:
		CAudioManager();

		CAudioManager(CAudioManager const&) = delete;
		void operator=(CAudioManager const&) = delete;

		std::vector<SoundUnit*> m_vecSoundUnits;

		float m_fDefaultVolume = 50.0f;
		int m_iMaxSoundEvents = 20;
};

#endif // __AUDIOMGR_H__