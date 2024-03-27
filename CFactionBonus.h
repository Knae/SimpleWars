#pragma once
#ifndef  __CFACTIONBONUSES_H__
#define  __CFACTIONBONUSES_H__

#include <string>

class CFactionBonus
{
public:
	const float GetModifierMovement()		{ return m_ftBonus_Movement; }
	const float GetModifierDamageDealt()	{ return m_fModifier_DamageDealt; }
	const int GetRangeOffset()				{ return m_iRangeOffset; }
	const std::string GetName()				{ return m_strFactionName; }

	void SetModifierMovement(float _input) { m_ftBonus_Movement = _input; }
	void SetModifierDamageDealt(float _input) { m_fModifier_DamageDealt = _input; }
	void SetRangeOffset(int _input) { m_iRangeOffset = _input; }
	void SetName(std::string _input) { m_strFactionName = _input; }
private:
	float	m_ftBonus_Movement = 0.0f;
	float	m_fModifier_DamageDealt = 1.0f;
	int		m_iRangeOffset = 0;
	std::string m_strFactionName;
};

#endif // ! __CFACTIONBONUSES_H__