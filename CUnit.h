#pragma once
#ifndef __CUNITS_H__

#include <SFML/Graphics.hpp>
#include "CUnitEnums.h"

/// <summary>
/// Unit class that will be used to create a unit
/// </summary>
class CUnit
{
public:
	CUnit(	CUnitEnums::TYPE _inUnits = CUnitEnums::TYPE::NONE,
			int _tileSize = 32,float _inHP = 0.0f, float _inMovement = 0.0f, float _inDamage = 0.0f, int _inRange = 0.0f,
			CUnitEnums::DIRECTION _inDirection = CUnitEnums::DIRECTION::NORTH );
	~CUnit();

	//void Initialize(sf::Texture* _inTextureSource);
	void Initialize(sf::Sprite* _inSpriteSource);
	void SetHP(float& _inHP) { m_fHP = _inHP; }
	void SetMovement(float& _inMovement) { m_fMovementPointsBase = _inMovement; }
	void SetDamageDealt(float& _inDamage) { m_fDamage = _inDamage; }
	void SetRange(int& _inRange) { m_iRange = _inRange; }
	void SetFaction(CUnitEnums::FACTION _inFaction) { m_eFaction = _inFaction; }
	void Update();
	void Update(double& _inTimeElapsed);
	float GetHP() { return m_fHP; }
	float TakeDamage(float& _inDamage) { m_fHP -= _inDamage; return m_fHP; }
	void Attack();
	void MoveTo(sf::Vector2u _inTileLocation);
	void MoveTo(sf::Vector2f _inPosition);
	sf::Sprite* GetSprite() { return m_sprtUnitSprite; }
	void Replenish();

private:
	void ExplodeInFlamingGlory();

	//faction
	CUnitEnums::TYPE m_UnitType;
	CUnitEnums::STATE m_CurrentState;
	CUnitEnums::DIRECTION m_CurrenDirection;
	float m_fHP;
	float m_fMovementPointsBase;
	float m_fMovementPoints;
	float m_fDamage;
	float m_fTileSize;
	int m_iRange;
	CUnitEnums::FACTION m_eFaction;
	const double m_dAnimeFrameTime = 200.0f;
	double m_dAnimProgress = 0.0f;
	
	//sf::Texture* m_texTextureSource;
	sf::Sprite* m_sprtUnitSprite;
	sf::Vector2u m_v2uCurrentTileLocation;

	//const posiitions for frames in tilemap
	const sf::IntRect m_AnimationFrameL = { 0, 0, 32, 32 };
	const sf::IntRect m_AnimationFrameR = { 32, 0, 32, 32};
};

#endif // !__CUNITS_H__