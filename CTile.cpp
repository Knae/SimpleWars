#include "CTile.h"

CTile::CTile()
{
	m_uiTileValue = 0;
	m_eType = CSceneEnums::TILETYPE::NONE;
	m_pUnitOnTile = nullptr;
}

CTile::~CTile()
{

}

void CTile::UnitEntersTile(CUnit* _inPUnit)
{
	m_pUnitOnTile = _inPUnit;
}

void CTile::UnitLeavesTile()
{
	m_pUnitOnTile = nullptr;
}

bool CTile::MovementCostToEnter(unsigned short& _movementCost)
{
	_movementCost = 1;
	return true;
}

