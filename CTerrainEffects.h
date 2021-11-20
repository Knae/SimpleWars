#pragma once
#ifndef  __CTERRAINEFFECTS_H__
#define  __CTERRAINEFFECTS_H__

class CTerrainEffects
{
public:
	const float GetModifierMovement() { return m_ftBonus_Movement; }
	const float GetModifierDamageDealt() { return m_fModifier_DamageDealt; }
	const float GetModifierDamageTaken() { return m_fModifier_DamageTaken; }
	const int GetRangeOffset() { return m_iRangeOffset; }

	void SetModifierMovement(float _input)		{ m_ftBonus_Movement = _input; }
	void SetModifierDamageDealt(float _input)	{ m_fModifier_DamageDealt = _input; }
	void SetModifierDamageTaken(float _input)	{ m_fModifier_DamageTaken = _input; }
	void setRangeOffset(int _input)				{ m_iRangeOffset = _input; }
private:
	float	m_ftBonus_Movement = 0.0f;
	float	m_fModifier_DamageDealt = 1.0f;
	float	m_fModifier_DamageTaken = 1.0f;
	int		m_iRangeOffset = 0;
};

#endif // ! __CTERRAINEFFECTS_H__