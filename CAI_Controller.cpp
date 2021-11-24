#include "CAI_Controller.h"
#include "CUnitManager.h"
#include "CSceneManager.h"

std::vector<CUnit*>* CAI_Controller::m_vecUnits_AI;
std::vector<CUnit*>* CAI_Controller::m_vecUnits_Player;

CAI_Controller::CAI_Controller()
{
	m_vecUnits_AI = nullptr;
	m_vecUnits_Player = nullptr;
}

int CAI_Controller::FindClosestEnemy(CUnit* _inCurrentUnit, CUnit*& _outTarget)
{
	sf::Vector2u currentPos = _inCurrentUnit->GetCurrentTile();
	sf::Vector2u targetPos(0, 0);
	CUnit* currentNearestTarget = nullptr;
	int currentNearestDistance = 99999;
	int currentCalculatedDistance = 0;
	for (auto& unit : *m_vecUnits_Player)
	{
		if (unit->GetState() != CUnitEnums::STATE::DESTROYED)
		{
			targetPos = unit->GetCurrentTile();
			currentCalculatedDistance = FindDistanceBetweenTiles(currentPos, targetPos);
			if (currentCalculatedDistance < currentNearestDistance)
			{
				currentNearestDistance = currentCalculatedDistance;
				currentNearestTarget = unit;
			}
		}
	}
	_outTarget = currentNearestTarget;
	//currentNearestTarget = nullptr;
	return currentNearestDistance;
}

int CAI_Controller::FindDistanceBetweenTiles(sf::Vector2u& _inCurrentTile, sf::Vector2u& _targetTile)
{
	//Check if can get negative values
	sf::Vector2i diff = GetDirectionToTarget(_inCurrentTile,_targetTile);
	int dist = abs(diff.x) + abs(diff.y);
	return dist;
}

sf::Vector2i CAI_Controller::GetDirectionToTarget(sf::Vector2u& _inCurrentTile, sf::Vector2u& _targetTile)
{
	//Check if can get negative values
	sf::Vector2i direc = sf::Vector2i(_targetTile.x - _inCurrentTile.x, _targetTile.y - _inCurrentTile.y);
	return direc;
}

CAI_Controller::~CAI_Controller()
{
	m_vecUnits_AI = nullptr;
	m_vecUnits_Player = nullptr;
}

void CAI_Controller::IntializeAi(std::vector<CUnit*>* _inPVecAI, std::vector<CUnit*>* _inPVecPlayer)
{
	m_vecUnits_AI = _inPVecAI;
	m_vecUnits_Player = _inPVecPlayer;
}

bool CAI_Controller::StartAITurn()
{
	CUnit* targetUnit = nullptr;
	for (auto& unit : *m_vecUnits_AI)
	{
		int dist = FindClosestEnemy(unit, targetUnit);
		if (targetUnit != nullptr)
		{
			bool hasTriedToAttack = unit->GetHasAtacked();
			bool cannotMove = false;
			while (!hasTriedToAttack && !cannotMove)
			{
				int range = CUnitManager::GetUnitRange(unit);
				if (dist <= range && !unit->GetHasAtacked())
				{
					CUnitEnums::TYPE typeEnum = unit->GetType();
					std::string typeString;
					CParseConfigCommon::ConvertUnitTypeToString(typeEnum, typeString);
					std::cout << "\nAI's " << typeString << " is attacking!" << std::endl;
					if (CUnitManager::Attack(unit, targetUnit))
					{
						CSceneManager::GetTileInScene(targetUnit->GetCurrentTile())->UnitLeavesTile();
					}
					hasTriedToAttack = true;
					return false;
				}
				else if(!cannotMove && unit->GetMovePoints()>0)
				{
					sf::Vector2u unitPosition = unit->GetCurrentTile();
					sf::Vector2u targetPosition = targetUnit->GetCurrentTile();
					sf::Vector2i dir = GetDirectionToTarget(unitPosition, targetPosition);
					if (dir.x != 0)
					{
						dir.x = dir.x / abs(dir.x);
					}

					if (dir.y != 0)
					{
						dir.y = dir.y / abs(dir.y);
					}
					
					//I hate This
					sf::Vector2i posTry1(unitPosition.x + dir.x, unitPosition.y);
					//sf::Vector2i posTry2(unitPosition.x , unitPosition.y + dir.y);
					CSceneEnums::TILETYPE posTryTileType1 = CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetTileType();
					//CSceneEnums::TILETYPE posTryTileType2 = CSceneManager::GetTileInScene(sf::Vector2u(posTry2))->GetTileType();
					if (CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetUnitOnTile() !=nullptr || 
						!CUnitManager::MoveUnit(unit, sf::Vector2u(posTry1), posTryTileType1))
					{
						posTry1 = sf::Vector2i(unitPosition.x, unitPosition.y + dir.y);
						posTryTileType1 = CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetTileType();
						if (CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetUnitOnTile() != nullptr ||
							!CUnitManager::MoveUnit(unit, sf::Vector2u(posTry1), posTryTileType1))
						{
							posTry1 = sf::Vector2i(unitPosition.x - dir.x, unitPosition.y);
							posTryTileType1 = CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetTileType();
							if (CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetUnitOnTile() != nullptr ||
								!CUnitManager::MoveUnit(unit, sf::Vector2u(posTry1), posTryTileType1))
							{
								posTry1 = sf::Vector2i(unitPosition.x, unitPosition.y - dir.y);
								posTryTileType1 = CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetTileType();
								if (CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetUnitOnTile() != nullptr ||
									!CUnitManager::MoveUnit(unit, sf::Vector2u(posTry1), posTryTileType1))
								{
									cannotMove = true;
								}
							}
						}
					}
					else
					{

					}

					if(!cannotMove)
					{
						CSceneManager::GetTileInScene(unitPosition)->UnitLeavesTile();
						CSceneManager::GetTileInScene(unit->GetCurrentTile())->UnitEntersTile(unit);
						return false;
					}
				}
				else
				{
					hasTriedToAttack = true;
				}
			}
		}
	}

	targetUnit = nullptr;
	return true;
}

