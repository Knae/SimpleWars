#include "CAI_Controller.h"
#include "CUnitManager.h"
#include "CSceneManager.h"
#include "CVFXManager.h"

std::vector<CUnit*>* CAI_Controller::m_pVecUnits_AI;
std::vector<CUnit*>* CAI_Controller::m_pVecUnits_Player;

//Note:This module would be better served as calculating actions for a single unit
//instead of the whole list. Then Gamemanager can process the VFX stuff as well

CAI_Controller::CAI_Controller()
{
	m_pVecUnits_AI = nullptr;
	m_pVecUnits_Player = nullptr;
}

/// <summary>
/// Go through each enemy unit and compares its distance
/// from the unit given in the first arg.writes the address of the
/// closest enemy instance into the 2nd arg
/// </summary>
/// <param name="_inCurrentUnit"></param>
/// <param name="_outTarget"></param>
/// <returns></returns>
int CAI_Controller::FindClosestEnemy(CUnit* _inCurrentUnit, CUnit*& _outTarget)
{
	sf::Vector2u currentPos = _inCurrentUnit->GetCurrentTile();
	sf::Vector2u targetPos(0, 0);
	CUnit* currentNearestTarget = nullptr;
	int currentNearestDistance = 99999;
	int currentCalculatedDistance = 0;
	for (auto& unit : *m_pVecUnits_Player)
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

/// <summary>
/// Calculates and returns the distance
/// between the given tile locations
/// </summary>
/// <param name="_inCurrentTile"></param>
/// <param name="_targetTile"></param>
/// <returns></returns>
int CAI_Controller::FindDistanceBetweenTiles(sf::Vector2u& _inCurrentTile, sf::Vector2u& _targetTile)
{
	//Check if can get negative values
	sf::Vector2i diff = GetDirectionToTarget(_inCurrentTile,_targetTile);
	int dist = abs(diff.x) + abs(diff.y);
	return dist;
}

/// <summary>
/// Calculates and returns the direction that the tile
/// from arg2 is from the tile in arg1
/// </summary>
/// <param name="_inCurrentTile"></param>
/// <param name="_targetTile"></param>
/// <returns>sf::Vector2i of the direction</returns>
sf::Vector2i CAI_Controller::GetDirectionToTarget(sf::Vector2u& _inCurrentTile, sf::Vector2u& _targetTile)
{
	//Check if can get negative values
	sf::Vector2i direc = sf::Vector2i(_targetTile.x - _inCurrentTile.x, _targetTile.y - _inCurrentTile.y);
	return direc;
}

CAI_Controller::~CAI_Controller()
{
	m_pVecUnits_AI = nullptr;
	m_pVecUnits_Player = nullptr;
}

/// <summary>
/// Get and store the pointers to the vector of units in the game
/// </summary>
/// <param name="_inPVecAI"></param>
/// <param name="_inPVecPlayer"></param>
void CAI_Controller::IntializeAi(std::vector<CUnit*>* _inPVecAI, std::vector<CUnit*>* _inPVecPlayer)
{
	m_pVecUnits_AI = _inPVecAI;
	m_pVecUnits_Player = _inPVecPlayer;
}

/// <summary>
/// Runs AI decision making code. For a unit that has not attack nor moved
/// it will try to find a target to attack. Failing that it'll move then try again.
/// if a unit moves or attacks, it'll return false as it's not finish and wait till its
/// called again to repeat the process. If a unit can has no possible tile to move to or 
/// has no MOV points or has already attacked, it'll move on the next unit. Once all units
/// have been processed, then the function returns true.
/// </summary>
/// <returns></returns>
bool CAI_Controller::StartAITurn()
{
	//TODO: Switch to doing one unit at a time each time instead of a for loop
	CUnit* targetUnit = nullptr;
	for (auto& unit : *m_pVecUnits_AI)
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
					//AI trying to attack. Process the VFX stuff as well
					CUnitEnums::TYPE unitType = unit->GetType();
					std::string typeString;
					CParseConfigCommon::ConvertUnitTypeToString(unitType, typeString);
					std::cout << "\nAI's " << typeString << " is attacking!" << std::endl;
					sf::Vector2u targetLocation = targetUnit->GetCurrentTile();
					CVFXManager::AddAttackVFX(targetLocation, unitType);
					if (CUnitManager::Attack(unit, targetUnit))
					{
						//If the target is dead, process it's removal
						//from game system
						CUnitEnums::SIDE targetSide = targetUnit->GetSide();
						CVFXManager::AddDeathVFX(targetLocation);
						CSceneManager::GetTileInScene(targetLocation)->UnitLeavesTile();
						CUnitManager::DeleteUnit(targetUnit,targetSide);
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
					//TODO: change to check possible tiles and distance to target instead
					//of actually moving them.
					sf::Vector2i posTry1(unitPosition.x + dir.x, unitPosition.y);
					CSceneEnums::TILETYPE posTryTileType1 = CSceneManager::GetTileInScene(sf::Vector2u(posTry1))->GetTileType();
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

					//If cannotMove is false, then the AI has moved the unit and we need 
					//to update the tile that unit was in and is now in
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

	//Should only reach here if there are no units to prrocess
	targetUnit = nullptr;
	return true;
}

