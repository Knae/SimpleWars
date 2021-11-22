#pragma once
#ifndef __CAICONTROLLER_H__
#define __CAICONTROLLER_H__

#include <SFML/Graphics.hpp>
#include <vector>

class CUnit;
//class CUnitManager;
class CAI_Controller
{
public:
	~CAI_Controller();

	static void IntializeAi(
		std::vector<CUnit*>* _inPVecAI,
		std::vector<CUnit*>* _inPVecPlayer);

	static void StartAITurn();

private:
	CAI_Controller();
	CAI_Controller(CAI_Controller const&) = delete;
	void operator=(CAI_Controller const&) = delete;

	static int FindClosestEnemy(CUnit* _inCurrentUnit, CUnit*& _outTarget);
	static int FindDistanceBetweenTiles(sf::Vector2u& _inCurrentTile, sf::Vector2u& _targetTile);
	static sf::Vector2i GetDirectionToTarget(sf::Vector2u& _inCurrentTile, sf::Vector2u& _targetTile);

	static std::vector<CUnit*>* m_vecUnits_AI;
	static std::vector<CUnit*>* m_vecUnits_Player;
};

#endif // !__CAICONTROLLER_H__