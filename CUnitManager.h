#pragma once
#ifndef  _CUNITMANAGER_H__
#define _CUNITMANAGER_H__
#include "CUnit.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "CParseConfigCommon.h"

/// <summary>
/// This manager will manage units available to both players.
/// NOTES:*Should have made a common manager class*
/// </summary>
class CUnitManager : public CParseConfigCommon
{
public:
	virtual ~CUnitManager();

	static CUnitManager& GetRef() { static CUnitManager m_umThis; return m_umThis; }

	static void ParseConfig(const std::string& _inFilePath, const std::string& _inFactionConfigPath);
	static void CreateUnit(CUnitEnums::TYPE _inType);
	static bool MoveUnit();
	static bool Attack(CUnit* _inAttackinUnit, CUnit* _inDefendingPlayer);
	static void ClearUnits();
	static bool CheckIfAnyUnitsLeft(CUnitEnums::SIDE _inSide);

private:

	CUnitManager();
	CUnitManager(CUnitManager const&) = delete;
	void operator=(CUnitManager const&) = delete;

	static std::vector<CUnit*> m_CurrentUnits_Blue;
	static std::vector<CUnit*> m_CurrentUnits_Red;

	static std::string m_UnitTexturePath_Blue;
	static std::string m_UnitTexturePath_Red;
	static std::map<CUnitEnums::TYPE, CUnitEnums::UnitRecord*> m_UnitStats;
	static std::map<CUnitEnums::FACTION, CUnitEnums::StatBonus_Add*> m_FactionsBonuses;
};

#endif // ! _CUNITMANAGER_H__