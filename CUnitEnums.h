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
		float m_tfHPBonus;
		float m_tfAtkBonus;
		float m_tfMoveBonus;
		int m_tiRangeBonus;
	};

	struct UnitRecord
	{
		float m_tfHP;
		float m_tfAtk;
		float m_tfMove;
		int m_tiRange;
		int m_tiSpriteWidth;
		int m_tiSpriteHeight;
		int m_tiSpriteTop;
		int m_tiSpriteLeft;
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

	enum class FACTION
	{
		NONE,
		TALONS,
		LYNXES,
		URSINE
	};
};


#endif // !__CUNITENUMS_H__