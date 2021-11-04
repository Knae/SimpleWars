#include "CTile.h"

CTile::CTile()
{
	m_uiTileValue = 0;
	m_Type = CSceneEnums::TILETYPE::NONE;
	m_UnitOnTile = nullptr;
}

CTile::~CTile()
{

}

void CTile::UnitEntersTile(CUnit* _inPUnit)
{
	m_UnitOnTile = _inPUnit;
}

void CTile::UnitLeavesTile()
{
	m_UnitOnTile = nullptr;
}

bool CTile::MovementCostToEnter(unsigned short& _movementCost)
{
	_movementCost = 1;
	return true;
}

