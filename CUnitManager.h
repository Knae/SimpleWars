#pragma once
#ifndef  _CUNITMANAGER_H__
#define _CUNITMANAGER_H__

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "CParseConfigCommon.h"
#include "CUnit.h"
#include "CTerrainEffects.h"

/// <summary>
/// This manager will manage units available to both players.
/// NOTES:*Should have made a common manager class*
/// </summary>
class CUnitManager : public CParseConfigCommon
{
public:
	virtual ~CUnitManager();

	static CUnitManager& GetRef() { static CUnitManager m_umThis; return m_umThis; }

	static void ParseConfig(const std::string& _inFilePath, const std::string& _inFactionConfigPath, const std::string& _inTerrainSettingsPath);
	static void DisplayUnits(sf::RenderWindow& _inWindow);
	static CUnit* CreateUnit(CUnitEnums::TYPE _inType, CUnitEnums::FACTION _inFaction, CUnitEnums::SIDE _inSide);
	static bool MoveUnit(CUnit* _inUnit, sf::Vector2u _inPosition);
	static bool MoveUnit(CUnit* _inUnit, sf::Vector2f _inPosition);
	static bool Attack(CUnit* _inAttackinUnit, CUnit* _inDefendingPlayer);
	static void ClearUnits();
	static void Update(double& _inElapsedTime);
	static bool CheckIfAnyUnitsLeft(CUnitEnums::SIDE _inSide);
	static void SwitchTurns();

private:

	CUnitManager();
	CUnitManager(CUnitManager const&) = delete;
	void operator=(CUnitManager const&) = delete;

	static std::vector<CUnit*> m_vecCurrentUnits_Blue;
	static std::vector<CUnit*> m_vecCurrentUnits_Red;

	static std::string m_strUnitTexturePath_Blue;
	static std::string m_strUnitTexturePath_Red;
	static sf::Texture* m_pUnitTexture_Blue;
	static sf::Texture* m_pUnitTexture_Red;

	static std::map<CUnitEnums::TYPE, CUnitEnums::UnitRecord*> m_mapUnitStats;
	static std::map<CUnitEnums::TYPE, CTerrainEffects> m_mapTileTerrainEffects;
	static std::map<CUnitEnums::FACTION, CUnitEnums::StatBonus_Add*> m_mapFactionsBonuses;
};
#endif // ! _CUNITMANAGER_H__