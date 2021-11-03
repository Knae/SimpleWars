#pragma once
#ifndef __CUNITENUMS_H__
#define __CUNITENUMS_H__

/// <summary>
/// Enums class used specifically by the unit class
/// </summary>

class CUnitEnums
{
public:
	struct StatBonus_Add
	{
		float m_tfAtkBonus;
		float m_tfMoveBonus;
		float m_tfRangeBonus;
	};

	struct StatBonus_Multiply
	{
		float m_tfAtkBonus;
		float m_tfMoveBonus;
		float m_tfRangeBonus;
		float m_tfIncDamgeBonus;
	};

	enum class TYPE
	{
		NONE,
		INFANTRY,
		TANK,
		ARTILLERY
	};

	enum class STATE
	{
		NONE,
		IDLE,
		MOVE,
		ATTACK,
		DESTROYED
	};

	enum class DIRECTION
	{
		NONE,
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

	enum class SIDE
	{
		NONE,
		BLUE,
		RED
	};
};


#endif // !__CUNITENUMS_H__