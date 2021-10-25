#pragma once
#ifndef __CTILE_H__
#define __CTILE_H_

#include "CSceneEnums.h"
/// <summary>
/// This tile class will be created for each tile in the map.
/// It will store its tile value corresponding to the tilemap used 
/// and the tile type and also a pointer to a unit that is on it
/// If there's no unit, the the pointer is a nullptr
/// </summary>
class CTile
{
public:
	CTile();
	virtual ~CTile();

	virtual void SetTileValue(unsigned int _input)			{ m_uiTileValue = _input; }
	virtual unsigned int GetTileValue()						{ return m_uiTileValue; }
	virtual void SetTileType(CSceneEnums::TILETYPE _input)	{ m_Type = _input; }
	virtual void UnitEntersTile() {}
	virtual void UnitLeavesTile() {}
	//virtual CUnit* GetUnitOnTile(){}
	virtual bool MovementCostToEnter(unsigned short& _movementCost);
private:
	unsigned int m_uiTileValue;
	CSceneEnums::TILETYPE m_Type;
	//CUnit* m_UnitOnTile;
};

#endif // !__CTILE_H__