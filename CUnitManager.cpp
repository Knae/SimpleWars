#include "CUnitManager.h"

std::vector<CUnit*> CUnitManager::m_CurrentUnits_Blue;
std::vector<CUnit*> CUnitManager::m_CurrentUnits_Red;
std::string CUnitManager::m_UnitTexturePath_Blue;
std::string CUnitManager::m_UnitTexturePath_Red;
std::map<CUnitEnums::TYPE, CUnitEnums::UnitRecord*> CUnitManager::m_UnitStats;
std::map<CUnitEnums::FACTION, CUnitEnums::StatBonus_Add*> CUnitManager::m_FactionsBonuses;

bool CUnitManager::MoveUnit()
{
	return false;
}

bool CUnitManager::Attack(CUnit* _inAttackinUnit, CUnit* _inDefendingPlayer)
{
	return false;
}

void CUnitManager::ClearUnits()
{
	/// <summary>
	/// TODO: CHECK THESE!
	/// </summary>
	for (auto& element : m_CurrentUnits_Blue)
	{
		delete element;
		element = nullptr;
	}

	for (auto& element : m_CurrentUnits_Red)
	{
		delete element;
		element = nullptr;
	}

	m_CurrentUnits_Blue.clear();
	m_CurrentUnits_Red.clear();
}

bool CUnitManager::CheckIfAnyUnitsLeft(CUnitEnums::SIDE _inSide)
{
	return false;
}

CUnitManager::CUnitManager()
{
}

CUnitManager::~CUnitManager()
{
}

void CUnitManager::ParseConfig(const std::string& _inUnitConfigPath, const std::string& _inFactionConfigPath)
{
	std::fstream currentConfig;
	std::string currentLine;

	currentConfig.open(_inUnitConfigPath);
	if(currentConfig.is_open())
	{
		while (std::getline(currentConfig, currentLine))
		{
			if (currentLine.compare("<UnitTextureSource>") == 0)
			{
				std::getline(currentConfig, currentLine);
				std::string readPath;
				std::string readLabel;
				while (currentLine.compare("</UnitTextureSource>") != 0)
				{
					//Getting path to unit sprite sheet files
					std::cout << "\nReading sprite sheet path\n";
					readLabel = ParseLineGetLabel(currentLine, readPath);
					if (readLabel.compare("Red") == 0)
					{
						m_UnitTexturePath_Red = readPath;
					}
					else if (readLabel.compare("Blue") == 0)
					{
						m_UnitTexturePath_Blue = readPath;
					}
					std::getline(currentConfig, currentLine);
				}
			}
			else if (currentLine.compare("<Unit>") == 0)
			{
				//Parsing data for unit
				std::cout << "\nReading config for a unit.\n";
				CUnitEnums::TYPE currentType = CUnitEnums::TYPE::NONE;
				CUnitEnums::UnitRecord* currentUnitStats = new CUnitEnums::UnitRecord;
				std::string readValue;
				std::string readLabel;

				std::getline(currentConfig, currentLine);
				while (currentLine.compare("</Unit>") != 0)
				{
					if (currentLine.compare("<Type>") == 0)
					{
						while (currentLine.compare("</Type>") != 0)
						{
							ParseLineGetLabel(currentLine, readValue);
							if (readValue.compare("Tank") == 0)
							{
								currentType = CUnitEnums::TYPE::TANK;
							}
							else if (readValue.compare("Artillery") == 0)
							{
								currentType = CUnitEnums::TYPE::ARTILLERY;
							}
							else if (readValue.compare("Infantry") == 0)
							{
								currentType = CUnitEnums::TYPE::INFANTRY;
							}
							else
							{
								currentType = CUnitEnums::TYPE::NONE;
							}
							std::getline(currentConfig, currentLine);
						}
					}
					else if (currentLine.compare("<Stats>") == 0)
					{
						std::getline(currentConfig, currentLine);
						while (currentLine.compare("</Stats>") != 0)
						{
							readLabel = ParseLineGetLabel(currentLine, readValue);
							if (readLabel.compare("HP") == 0)
							{
								currentUnitStats->m_tfHP = stof(readValue);
							}
							else if (readLabel.compare("ATK") == 0)
							{
								currentUnitStats->m_tfAtk = stof(readValue);
							}
							else if (readLabel.compare("MOV") == 0)
							{
								currentUnitStats->m_tfMove = stof(readValue);
							}
							else if (readLabel.compare("RGE") == 0)
							{
								currentUnitStats->m_tiRange = stoi(readValue);
							}
							std::getline(currentConfig, currentLine);
						}
					}
					else if (currentLine.compare("<SpriteInfo>") == 0)
					{
						//std::getline(currentConfig, currentLine);
						while (currentLine.compare("</SpriteInfo>") != 0)
						{
							readLabel = ParseLineGetLabel(currentLine, readValue);
							if (readLabel.compare("Width") == 0)
							{
								currentUnitStats->m_tiSpriteWidth = stof(readValue);
							}
							else if (readLabel.compare("Height") == 0)
							{
								currentUnitStats->m_tiSpriteHeight = stof(readValue);
							}
							else if (readLabel.compare("TopCorner") == 0)
							{
								currentUnitStats->m_tiSpriteTop = stof(readValue);
							}
							else if (readLabel.compare("LeftCorner") == 0)
							{
								currentUnitStats->m_tiSpriteLeft = stof(readValue);
							}
							std::getline(currentConfig, currentLine);
						}
					}
					std::getline(currentConfig, currentLine);
				}
				//Finished parsing this unit
				std::cout << "\nFinished parsing a unit.\n";
				m_UnitStats.emplace(currentType, currentUnitStats);
			}
		}
	}

	currentConfig.close();
	currentConfig.open(_inFactionConfigPath);
	if (currentConfig.is_open())
	{
		while (std::getline(currentConfig, currentLine))
		{
			if (currentLine.compare("<Faction>") == 0)
			{
				//Parsing data for unit
				std::cout << "\nReading config for a factions.\n";
				CUnitEnums::FACTION currentFaction = CUnitEnums::FACTION::NONE;
				CUnitEnums::StatBonus_Add* currentFactionBonus = new CUnitEnums::StatBonus_Add;
				std::string readValue;
				std::string readLabel;

				while (currentLine.compare("</Faction>") != 0)
				{
					//std::getline(currentConfig, currentLine);
					if (currentLine.compare("<Name>") == 0)
					{
						while (currentLine.compare("</Name>") != 0)
						{
							ParseLineGetLabel(currentLine, readValue);
							if (readValue.compare("Graysong Talons") == 0)
							{
								currentFaction = CUnitEnums::FACTION::TALONS;
							}
							else if (readValue.compare("Lowvale Lynxes") == 0)
							{
								currentFaction = CUnitEnums::FACTION::LYNXES;
							}
							else if (readValue.compare("Coldfield Ursine") == 0)
							{
								currentFaction = CUnitEnums::FACTION::URSINE;
							}
							else
							{
								currentFaction = CUnitEnums::FACTION::NONE;
							}
							std::getline(currentConfig, currentLine);
						}
						//std::getline(currentConfig, currentLine);
					}
					else if (currentLine.compare("<BonusStats>") == 0)
					{
						while (currentLine.compare("</BonusStats>") != 0)
						{
							readLabel = ParseLineGetLabel(currentLine, readValue);
							if (readLabel.compare("HP") == 0)
							{
								currentFactionBonus->m_tfHPBonus = stof(readValue);
							}
							else if (readLabel.compare("ATK") == 0)
							{
								currentFactionBonus->m_tfAtkBonus = stof(readValue);
							}
							else if (readLabel.compare("MOV") == 0)
							{
								currentFactionBonus->m_tfMoveBonus = stof(readValue);
							}
							else if (readLabel.compare("RGE") == 0)
							{
								currentFactionBonus->m_tiRangeBonus = stoi(readValue);
							}
							std::getline(currentConfig, currentLine);
						}
						//std::getline(currentConfig, currentLine);
					}
					std::getline(currentConfig, currentLine);
				}
				//Finished parsing this faction
				std::cout << "\nFinished parsing a unit.\n";
				m_FactionsBonuses.emplace(currentFaction, currentFactionBonus);
			}
		}
	}
}


void CUnitManager::CreateUnit(CUnitEnums::TYPE _inType)
{
}

