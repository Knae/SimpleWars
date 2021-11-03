#pragma once
#ifndef  _CUNITMANAGER_H__
#define _CUNITMANAGER_H__
#include "CUnit.h"
#include <vector>

/// <summary>
/// This manager will manage units available to both players.
/// </summary>
class CUnitManager
{
public:
	virtual ~CUnitManager();

	static CUnitManager& GetRef() { static CUnitManager m_umThis; return m_umThis; }

	void ParseConfigUnits(std::string& _inFilePath);
	void CreateUnit(CUnitEnums::TYPE _inType);
	bool MoveUnit();
	bool Attack();
	void ClearUnits();
	bool CheckIfAnyUnitsLeft(CUnitEnums::SIDE _inSide);

private:

	CUnitManager();
	CUnitManager(CUnitManager const&) = delete;
	void operator=(CUnitManager const&) = delete;

	std::vector<CUnit*> CurrentUnits_Blue;
	std::vector<CUnit*> CurrentUnits_Red;

};

#endif // ! _CUNITMANAGER_H__