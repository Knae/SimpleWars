#include "CAudioManager.h"

CAudioManager::CAudioManager()
{
	for (int i = 0; i < m_iMaxSoundEvents; i++)
	{
		SoundUnit* newUnit = new SoundUnit;
		newUnit->soundPlayer.setBuffer(newUnit->soundBuffer);
		newUnit->soundPlayer.setVolume(m_fDefaultVolume);
		m_vecSoundUnits.push_back(newUnit);
	}
}

CAudioManager::~CAudioManager()
{
	for (auto& unit : m_vecSoundUnits)
	{
		delete unit;
		unit = nullptr;
	}
}

/// <summary>
/// Adds a sound event using the given sound file.
/// </summary>
/// <param name="inSoundFile"></param>
/// <returns></returns>
RESULT CAudioManager::AddSoundEvent(std::string& inSoundFile)
{
	SoundUnit* unitInUse = nullptr;
	for (auto& unit : m_vecSoundUnits)
	{
		if (unit->soundPlayer.getStatus() == sf::Sound::Stopped)
		{
			unitInUse = unit;
			exit;
		}
	}

	if (unitInUse == nullptr)
	{
		//No free soundUnit to use
		printf("\nNo free sound unit to play sound. Requested sound file:\n%s\n",inSoundFile.c_str());
		return NO_SLOT;
	}
	else
	{
		if (unitInUse->soundBuffer.loadFromFile(inSoundFile))
		{
			unitInUse->soundPlayer.play();
			return SUCCESS;
		}
		else
		{	
			printf("\nUnable to load sound file. Requested sound file:\n%s\n", inSoundFile.c_str());
			return FAIL;
		}
	}
}

