#pragma once
#ifndef __CUNITS_H__
#define __CUNITS_H__

#include <SFML/Graphics.hpp>
#include "CUnitEnums.h"
#include "CSceneEnums.h"

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
	inline void SetHP(float& _inHP)									{ m_fHP = _inHP; }
	inline void SetMovement(float& _inMovement)						{ m_fMovePoints_Base = _inMovement; }
	inline void SetDamageDealt(float& _inDamage)					{ m_fDamage = _inDamage; }
	inline void SetRange(int& _inRange)								{ m_iRange = _inRange; }
	inline void SetFaction(CUnitEnums::FACTION _inFaction)			{ m_eFaction = _inFaction; }
	inline void SetSide(CUnitEnums::SIDE _inSide)					{ m_eControllingPlayer = _inSide; }
	inline void SetHasAttacked()									{ m_bHasAttacked = true; }
	inline void SetCurrentTileType(CSceneEnums::TILETYPE _inType)	{ m_eCurrentTileType = _inType; }
	void SetLocation(sf::Vector2f _inPosition);
	void SetLocation(sf::Vector2u _inPosition);

	void GetMovementStat(float& _outMoveBase, float& _outMovePoints) {
		_outMoveBase = m_fMovePoints_Base;
		_outMovePoints = m_fMovePoints;
	}
	inline float GetMovePoints()						{ return m_fMovePoints; }
	inline bool GetHasAtacked()							{ return m_bHasAttacked; }
	inline bool GetIfActing()							{ return m_bIsActing; }
	inline sf::Vector2u GetCurrentTile()				{ return m_CurrentTileLocation; }
	inline CSceneEnums::TILETYPE GetCurrentTileType()	{ return m_eCurrentTileType; }
	inline CUnitEnums::STATE GetState()					{ return m_eCurrentState; }
	inline CUnitEnums::FACTION GetFaction()				{ return m_eFaction; }
	inline CUnitEnums::SIDE GetSide()					{ return m_eControllingPlayer; }
	inline CUnitEnums::TYPE GetType()					{ return m_eUnitType; }
	inline int GetRange()								{ return m_iRange; }
	inline float GetHP()								{ return m_fHP; }
	inline float GetDamageDealt()						{ return m_fDamage; }
	inline sf::Sprite* GetSprite()						{ return m_UnitSprite; }

	void Update();
	void Update(double& _inTimeElapsed);
	void Attack();
	void MoveTo(sf::Vector2u _inTileLocation);
	void MoveTo(sf::Vector2f _inPosition);
	inline float TakeDamage(float& _inDamage)	{ m_fHP -= _inDamage; return m_fHP; }
	void IncrementHealth(float _inAmount)		{ m_fHP += _inAmount; m_fHP = (m_fHP>0 ? m_fHP:1.0f);};
	void IncrementMovePoints(float _inPoints)	{ m_fMovePoints += _inPoints; m_fMovePoints = (m_fMovePoints<0 ? 0.0f:m_fMovePoints);}
	void IncrementDamageDealt(float _inAmount)	{ m_fDamage += _inAmount; m_fDamage = (m_fDamage>0 ? m_fDamage:1.0f);}
	void IncrementRange(int _inAmount)			{ m_iRange += _inAmount; m_iRange = (m_iRange>0 ? m_iRange:1);}
	void Replenish();
	void ExplodeInFlamingGlory();

private:
	sf::Vector2f LerpMovement(sf::Vector2f& _currentPos, sf::Vector2f& _destinationPos, float _fAlpha);

	CUnitEnums::TYPE m_eUnitType;
	CUnitEnums::SIDE m_eControllingPlayer;
	CUnitEnums::STATE m_eCurrentState;
	CUnitEnums::FACTION m_eFaction;
	CUnitEnums::DIRECTION m_eCurrentDirection;
	CSceneEnums::TILETYPE m_eCurrentTileType;
	float m_fHP;
	float m_fMovePoints_Base;
	float m_fMovePoints;
	float m_fDamage;
	float m_fTileSize;
	int m_iRange;
	bool m_bHasAttacked;
	const double m_dAnimeFrameTime = 200.0f;
	double m_dAnimProgress = 0.0f;
	bool m_bIsActing = false;

	sf::Sprite* m_UnitSprite;
	sf::Vector2u m_CurrentTileLocation;

	//const positions for frames in tilemap
	const sf::IntRect m_AnimationFrameL = { 0, 0, 32, 32 };
	const sf::IntRect m_AnimationFrameR = { 32, 0, 32, 32};
	const sf::IntRect m_ButtonFrame = { 0, 0, 32, 64 };
};

#endif // !__CUNITS_H__