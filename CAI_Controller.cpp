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
		targetPos = unit->GetCurrentTile();
		currentCalculatedDistance = FindDistanceBetweenTiles(currentPos, targetPos);
		if (currentCalculatedDistance < currentNearestDistance)
		{
			currentNearestDistance = currentCalculatedDistance;
			currentNearestTarget = unit;
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

void CAI_Controller::StartAITurn()
{
	CUnit* targetUnit = nullptr;
	for (auto& unit : *m_vecUnits_AI)
	{
		int dist = FindClosestEnemy(unit, targetUnit);
		if (targetUnit != nullptr)
		{
			bool hasTriedToAttack = false;
			while (!hasTriedToAttack)
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
				}
				else if(unit->GetMovePoints()>0)
				{
					//sf::Vector2i
					hasTriedToAttack = true;
				}
				else
				{
					hasTriedToAttack = true;
				}
			}
		}
	}

	targetUnit = nullptr;
}

