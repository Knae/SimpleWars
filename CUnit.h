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
	CUnit(	CUnitEnums::TYPE _inUnits = CUnitEnums::TYPE::NONE, CUnitEnums::SIDE _inSIDE = CUnitEnums::SIDE::NONE,
			int _tileSize = 32,float _inHP = 0.0f, float _inMovement = 0.0f, float _inDamage = 0.0f, int _inRange = 0.0f,
			CUnitEnums::DIRECTION _inDirection = CUnitEnums::DIRECTION::NORTH );
	~CUnit();

	//void Initialize(sf::Texture* _inTextureSource);
	void Initialize(sf::Sprite* _inSpriteSource);
	void SetHP(float& _inHP)						{ m_fHP = _inHP; }
	void SetMovement(float& _inMovement)			{ m_fMovementPointsBase = _inMovement; }
	void SetDamageDealt(float& _inDamage)			{ m_fDamage = _inDamage; }
	void SetRange(int& _inRange)					{ m_iRange = _inRange; }
	void SetFaction(CUnitEnums::FACTION _inFaction) { m_eFaction = _inFaction; }
	void SetSide(CUnitEnums::SIDE _inSide)			{ m_eControllingPlayer = _inSide; }
	void SetLocation(sf::Vector2f _inPosition);

	void GetMovementStat(float& _outMoveBase, float& _outMovePoints) {
		_outMoveBase = m_fMovementPointsBase;
		_outMovePoints = m_fMovementPoints;
	}
	sf::Vector2u GetCurrentTile()		{ return m_v2uCurrentTileLocation; }
	CUnitEnums::STATE GetState()		{ return m_eCurrentState; }
	CUnitEnums::FACTION GetFaction()	{ return m_eFaction; }
	CUnitEnums::SIDE GetSide()			{ return m_eControllingPlayer; }
	CUnitEnums::TYPE GetType()			{ return m_eUnitType; }
	int GetRange()						{ return m_iRange; }

	void Update();
	void Update(double& _inTimeElapsed);
	float GetHP()						{ return m_fHP; }
	void Attack();
	void MoveTo(sf::Vector2u _inTileLocation);
	void MoveTo(sf::Vector2f _inPosition);
	sf::Sprite* GetSprite()				{ return m_sprtUnitSprite; }
	float TakeDamage(float& _inDamage)				{ m_fHP -= _inDamage; return m_fHP; }
	void IncrementMovementPoints(float _inPoints)	{ m_fMovementPoints += _inPoints; }
	void IncrementDamageDealt(float _inAmount)		{ m_fDamage += _inAmount; }
	void IncrementRange(int _inAmount)				{ m_iRange += _inAmount; }
	//void ChangeFaction();
	void Replenish();

private:
	void ExplodeInFlamingGlory();
	sf::Vector2f LerpMovement(sf::Vector2f& _currentPos, sf::Vector2f& _destinationPos, float _fAlpha);

	CUnitEnums::TYPE m_eUnitType;
	CUnitEnums::SIDE m_eControllingPlayer;
	CUnitEnums::STATE m_eCurrentState;
	CUnitEnums::FACTION m_eFaction;
	CUnitEnums::DIRECTION m_eCurrentDirection;
	float m_fHP;
	float m_fMovementPointsBase;
	float m_fMovementPoints;
	float m_fDamage;
	float m_fTileSize;
	int m_iRange;
	const double m_dAnimeFrameTime = 200.0f;
	double m_dAnimProgress = 0.0f;
	
	//sf::Texture* m_texTextureSource;
	sf::Sprite* m_sprtUnitSprite;
	sf::Vector2u m_v2uCurrentTileLocation;

	//const posiitions for frames in tilemap
	const sf::IntRect m_AnimationFrameL = { 0, 0, 32, 32 };
	const sf::IntRect m_AnimationFrameR = { 32, 0, 32, 32};
	const sf::IntRect m_ButtonFrame = { 0, 0, 32, 64 };
};

#endif // !__CUNITS_H__