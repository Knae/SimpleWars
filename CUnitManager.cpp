#include "CUnitManager.h"

std::vector<CUnit*> CUnitManager::m_CurrentUnits_Blue;
std::vector<CUnit*> CUnitManager::m_CurrentUnits_Red;
std::string CUnitManager::m_UnitTexturePath_Blue;
std::string CUnitManager::m_UnitTexturePath_Red;
sf::Texture* CUnitManager::m_UnitTexture_Blue;
sf::Texture* CUnitManager::m_UnitTexture_Red;
std::map<CUnitEnums::TYPE, CUnitEnums::UnitRecord*> CUnitManager::m_UnitStats;
std::map<CUnitEnums::FACTION, CUnitEnums::StatBonus_Add*> CUnitManager::m_FactionsBonuses;

CUnitManager::CUnitManager()
{
	m_UnitTexture_Blue = nullptr;
	m_UnitTexture_Red = nullptr;
}

CUnitManager::~CUnitManager()
{
	delete m_UnitTexture_Red;
	delete m_UnitTexture_Blue;

	m_UnitTexture_Blue = nullptr;
	m_UnitTexture_Red = nullptr;
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

	if (m_UnitTexture_Blue != nullptr)
	{
		delete m_UnitTexture_Blue;
		m_UnitTexture_Blue = nullptr;
	}

	if (m_UnitTexture_Red != nullptr)
	{
		delete m_UnitTexture_Red;
		m_UnitTexture_Red = nullptr;
	}

	m_UnitTexture_Blue = new sf::Texture();
	if (!m_UnitTexture_Blue->loadFromFile(m_UnitTexturePath_Blue))
	{
		std::cout << "\nERROR:Unable to load textures for Blue units!" << std::endl;
	}

	m_UnitTexture_Red = new sf::Texture();
	if (!m_UnitTexture_Red->loadFromFile(m_UnitTexturePath_Red))
	{
		std::cout << "\nERROR:Unable to load textures for Red units!" << std::endl;
	}
}

void CUnitManager::DisplayUnits(sf::RenderWindow& _inWindow)
{
	for (auto& element : m_CurrentUnits_Blue)
	{
		_inWindow.draw(*element->GetSprite());
	}

	for (auto& element : m_CurrentUnits_Red)
	{
		_inWindow.draw(*element->GetSprite());
	}
}

CUnit* CUnitManager::CreateUnit(CUnitEnums::TYPE _inType,CUnitEnums::FACTION _inFaction, CUnitEnums::SIDE _inSide)
{
	std::map<CUnitEnums::TYPE, CUnitEnums::UnitRecord*>::iterator mapIter;
	mapIter = m_UnitStats.find(_inType);
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
		unitSprite->setTexture(*m_UnitTexture_Blue);
		unitSprite->setTextureRect(unitTextureRect);
		newUnit->Initialize(unitSprite);
		m_CurrentUnits_Blue.push_back(newUnit);
	}
	else
	{
		unitSprite->setTexture(*m_UnitTexture_Red);
		unitSprite->setTextureRect(unitTextureRect);
		newUnit->Initialize(unitSprite);
		m_CurrentUnits_Red.push_back(newUnit);
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
	distanceToCurrentTile.x = abs(int(_inPosition.x) - (int)(unitPosition.x));
	distanceToCurrentTile.y = abs(int(_inPosition.y) - (int)(unitPosition.y));

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

void CUnitManager::Update(double& _inElapsedTime)
{
	for (auto& element : m_CurrentUnits_Blue)
	{
		element->Update(_inElapsedTime);
	}

	for (auto& element : m_CurrentUnits_Red)
	{
		element->Update(_inElapsedTime);
	}
}

bool CUnitManager::CheckIfAnyUnitsLeft(CUnitEnums::SIDE _inSide)
{
	return false;
}
