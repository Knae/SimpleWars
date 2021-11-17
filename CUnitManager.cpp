#include "CUnitManager.h"

std::vector<CUnit*> CUnitManager::m_vecCurrentUnits_Blue;
std::vector<CUnit*> CUnitManager::m_vecCurrentUnits_Red;
std::string CUnitManager::m_strUnitTexturePath_Blue;
std::string CUnitManager::m_strUnitTexturePath_Red;
sf::Texture* CUnitManager::m_pUnitTexture_Blue;
sf::Texture* CUnitManager::m_pUnitTexture_Red;
std::vector<std::string> CUnitManager::m_vecUnitTerrainModPaths;
std::map<CUnitEnums::TYPE, CUnitEnums::UnitRecord*> CUnitManager::m_mapUnitStats;
std::map<CUnitEnums::TYPE, std::map<CSceneEnums::TILETYPE, CTerrainEffects*>> CUnitManager::m_mapTileTerrainEffects;
std::map<CUnitEnums::FACTION, CUnitEnums::StatBonus_Add*> CUnitManager::m_mapFactionsBonuses;

CUnitManager::CUnitManager()
{
	m_pUnitTexture_Blue = nullptr;
	m_pUnitTexture_Red = nullptr;
}

CUnitManager::~CUnitManager()
{
	delete m_pUnitTexture_Red;
	delete m_pUnitTexture_Blue;

	m_pUnitTexture_Blue = nullptr;
	m_pUnitTexture_Red = nullptr;
}

/// <summary>
/// Parse all the config files related to units
/// </summary>
/// <param name="_inUnitConfigPath"></param>
/// <param name="_inFactionConfigPath"></param>
void CUnitManager::ParseConfig(const std::string& _inUnitConfigPath, const std::string& _inFactionConfigPath)
{
	std::ifstream currentConfig;
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
						m_strUnitTexturePath_Red = readPath;
					}
					else if (readLabel.compare("Blue") == 0)
					{
						m_strUnitTexturePath_Blue = readPath;
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
							ConvertToUnitType(readValue, currentType);
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
								currentUnitStats->m_tiSpriteWidth = stoi(readValue);
							}
							else if (readLabel.compare("Height") == 0)
							{
								currentUnitStats->m_tiSpriteHeight = stoi(readValue);
							}
							else if (readLabel.compare("TopCorner") == 0)
							{
								currentUnitStats->m_tiSpriteTop = stoi(readValue);
							}
							else if (readLabel.compare("LeftCorner") == 0)
							{
								currentUnitStats->m_tiSpriteLeft = stoi(readValue);
							}
							std::getline(currentConfig, currentLine);
						}
					}
					else if(currentLine.compare("<TerrainMod>") == 0)
					{
						while (!currentLine.compare("</TerrainMod>") == 0)
						{
							readLabel = ParseLineGetLabel(currentLine, readValue);
							if (readLabel.compare("Path") == 0)
							{
								m_vecUnitTerrainModPaths.push_back(readValue);
							}
							std::getline(currentConfig, currentLine);
						}

					}
					std::getline(currentConfig, currentLine);
				}
				//Finished parsing this unit
				std::cout << "\nFinished parsing a unit.\n";
				m_mapUnitStats.emplace(currentType, currentUnitStats);
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
							//This could probably be converted to a function in
							//ParseConfigCommon
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
				m_mapFactionsBonuses.emplace(currentFaction, currentFactionBonus);
			}
		}
	}

	currentConfig.close();
	for (auto& element : m_vecUnitTerrainModPaths)
	{
		//For each path, open and parse the terrain modifiers
		currentConfig.open(element);
		if (currentConfig.is_open())
		{
			/*while (std::getline(currentConfig, currentLine))
			{*/
				CUnitEnums::TYPE currentType = CUnitEnums::TYPE::NONE;
				CSceneEnums::TILETYPE currentTile = CSceneEnums::TILETYPE::NONE;
				std::string readValue;
				std::string readLabel;
				CTerrainEffects* newTerrainRecord = nullptr;

				while (std::getline(currentConfig, currentLine))
				{
					if (currentLine.compare("<TYPE>")==0)
					{
						while (currentLine.compare("</TYPE>")!=0)
						{
							ConvertToUnitType(currentLine, currentType);
							if (currentType == CUnitEnums::TYPE::NONE)
							{
								std::cout << "\nUnknown type in this config file: " << element << std::endl;
							}
							std::getline(currentConfig, currentLine);
						}
						if (currentType != CUnitEnums::TYPE::NONE)
						{
							m_mapTileTerrainEffects.insert(std::make_pair(currentType,std::map<CSceneEnums::TILETYPE,CTerrainEffects*>()));
						}
					}
					else if(currentLine.compare("<TERRAIN>")==0)
					{
						while (currentLine.compare("</TERRAIN>")!=0)
						{
							if (currentLine.compare("<START>")==0)
							{
								newTerrainRecord = new CTerrainEffects();
								CSceneEnums::TILETYPE terrainType = CSceneEnums::TILETYPE::NONE;
								while (currentLine.compare("</STOP>") != 0)
								{
									readLabel = ParseLineGetLabel(currentLine,readValue);	
									
									if (readLabel.compare("TYPE") == 0)
									{
										if (!ConvertToTileType(readValue, terrainType))
										{
											std::cout << "\nUnrecognised tile type while parsing tile effects: " << readValue << std::endl;
										}
									}
									else if (readLabel.compare("MOV") == 0)
									{
										newTerrainRecord->SetModifierMovement(std::stof(readValue));
									}
									else if (readLabel.compare("RGE") == 0)
									{
										newTerrainRecord->setRangeOffset(std::stoi(readValue));
									}
									//Damage Taken
									else if (readLabel.compare("DMG_T") == 0)
									{
										newTerrainRecord->SetModifierDamageTaken(std::stof(readValue));
									}
									//Damage dealt
									else if (readLabel.compare("DMG_D") == 0)
									{
										newTerrainRecord->SetModifierDamageDealt(std::stof(readValue));
									}
									std::getline(currentConfig, currentLine);
								}

								if (m_mapTileTerrainEffects.find(currentType) != m_mapTileTerrainEffects.end())
								{
									(*m_mapTileTerrainEffects.find(currentType)).second.emplace(terrainType, newTerrainRecord);
								}
							}

							std::getline(currentConfig, currentLine);
						}
					}
					//std::getline(currentConfig, currentLine);
				}
				newTerrainRecord = nullptr;
			//}
		}
		currentConfig.close();
	}


	if (m_pUnitTexture_Blue != nullptr)
	{
		delete m_pUnitTexture_Blue;
		m_pUnitTexture_Blue = nullptr;
	}

	if (m_pUnitTexture_Red != nullptr)
	{
		delete m_pUnitTexture_Red;
		m_pUnitTexture_Red = nullptr;
	}

	m_pUnitTexture_Blue = new sf::Texture();
	if (!m_pUnitTexture_Blue->loadFromFile(m_strUnitTexturePath_Blue))
	{
		std::cout << "\nERROR:Unable to load textures for Blue units!" << std::endl;
	}

	m_pUnitTexture_Red = new sf::Texture();
	if (!m_pUnitTexture_Red->loadFromFile(m_strUnitTexturePath_Red))
	{
		std::cout << "\nERROR:Unable to load textures for Red units!" << std::endl;
	}
}

void CUnitManager::DisplayUnits(sf::RenderWindow& _inWindow)
{
	for (auto& element : m_vecCurrentUnits_Blue)
	{
		if (element->GetState() != CUnitEnums::STATE::DESTROYED)
		{
			_inWindow.draw(*element->GetSprite());
		}
	}

	for (auto& element : m_vecCurrentUnits_Red)
	{
		if (element->GetState() != CUnitEnums::STATE::DESTROYED)
		{
			_inWindow.draw(*element->GetSprite());
		}
	}
}

CUnit* CUnitManager::CreateUnit(CUnitEnums::TYPE _inType,CUnitEnums::FACTION _inFaction, CUnitEnums::SIDE _inSide)
{
	std::map<CUnitEnums::TYPE, CUnitEnums::UnitRecord*>::iterator mapIter;
	mapIter = m_mapUnitStats.find(_inType);
	CUnitEnums::UnitRecord* newUnitStats = (*mapIter).second;
	CUnit* newUnit = new CUnit(	
		_inType,
		_inSide,
		newUnitStats->m_tiSpriteHeight,
		newUnitStats->m_tfHP,
		newUnitStats->m_tfMove,
		newUnitStats->m_tfAtk,
		newUnitStats->m_tiRange);
	newUnit->SetFaction(_inFaction);

	sf::IntRect unitTextureRect(
		newUnitStats->m_tiSpriteLeft,
		newUnitStats->m_tiSpriteTop,
		newUnitStats->m_tiSpriteWidth,
		newUnitStats->m_tiSpriteHeight
	);
	sf::Sprite* unitSprite = new sf::Sprite();
	if (_inSide == CUnitEnums::SIDE::BLUE)
	{
		unitSprite->setTexture(*m_pUnitTexture_Blue);
		unitSprite->setTextureRect(unitTextureRect);
		newUnit->Initialize(unitSprite);
		m_vecCurrentUnits_Blue.push_back(newUnit);
	}
	else
	{
		unitSprite->setTexture(*m_pUnitTexture_Red);
		unitSprite->setTextureRect(unitTextureRect);
		newUnit->Initialize(unitSprite);
		m_vecCurrentUnits_Red.push_back(newUnit);
	}
	unitSprite = nullptr;
	return newUnit;
}

/// <summary>
/// Move units. Checks if the new location is 
/// one tile away. Otherwise, unit location is not changed.
/// Actual movement is done thorugh update calls to the UnitManager
/// </summary>
/// <param name="_inUnit">Pointer to unit being moved. CUnit*</param>
/// <param name="_inPosition">New position to move to. sf::Vector2u</param>
/// <returns>whether the unit will be moved or not</returns>
bool CUnitManager::MoveUnit(CUnit* _inUnit, sf::Vector2u _inPosition)
{
	sf::Vector2i distanceToCurrentTile(0, 0);
	sf::Vector2u unitPosition = _inUnit->GetCurrentTile();
	distanceToCurrentTile.x = abs((int)(_inPosition.x - unitPosition.x));
	distanceToCurrentTile.y = abs((int)(_inPosition.y - unitPosition.y));

	if ((distanceToCurrentTile.x + distanceToCurrentTile.y) == 1)
	{
		_inUnit->MoveTo(_inPosition);
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// Move units. uses Vector2f instead
/// </summary>
/// <param name="_inUnit">Pointer to unit being moved. CUnit*</param>
/// <param name="_inPosition">New position to move to. sf::Vector2u</param>
/// <returns>whether the unit will be moved or not</returns>
bool CUnitManager::MoveUnit(CUnit* _inUnit, sf::Vector2f _inPosition)
{
	sf::IntRect unitSpriteRect = _inUnit->GetSprite()->getTextureRect();
	int m_fTileSize = unitSpriteRect.width;
	sf::Vector2u tilePosition(	(unsigned int)(_inPosition.x / m_fTileSize),
								(unsigned int)(_inPosition.y / m_fTileSize));
	
	return MoveUnit(_inUnit, tilePosition);
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
	for (auto& element : m_vecCurrentUnits_Blue)
	{
		delete element;
		element = nullptr;
	}

	for (auto& element : m_vecCurrentUnits_Red)
	{
		delete element;
		element = nullptr;
	}

	m_vecCurrentUnits_Blue.clear();
	m_vecCurrentUnits_Red.clear();
}

void CUnitManager::Update(double& _inElapsedTime)
{
	for (auto& element : m_vecCurrentUnits_Blue)
	{
		element->Update(_inElapsedTime);
	}

	for (auto& element : m_vecCurrentUnits_Red)
	{
		element->Update(_inElapsedTime);
	}
}

bool CUnitManager::CheckIfAnyUnitsLeft(CUnitEnums::SIDE _inSide)
{
	if (_inSide == CUnitEnums::SIDE::BLUE)
	{
		for (auto& element : m_vecCurrentUnits_Red)
		{
			if (element->GetState() != CUnitEnums::STATE::DESTROYED)
			{
				return true;
			}
		}
	}
	else if (_inSide == CUnitEnums::SIDE::RED)
	{
		for (auto& element : m_vecCurrentUnits_Blue)
		{
			if (element->GetState() != CUnitEnums::STATE::DESTROYED)
			{
				return true;
			}
		}
	}

	return false;
}

void CUnitManager::SwitchTurns()
{
	for (auto& element : m_vecCurrentUnits_Blue)
	{
		element->Replenish();
	}

	for (auto& element : m_vecCurrentUnits_Red)
	{
		element->Replenish();
	}
}

