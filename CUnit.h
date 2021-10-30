#pragma once
#ifndef __CUNITS_H__

#include <SFML/Graphics.hpp>
#include "CUnitEnums.h"

class CUnit
{
public:
	CUnit();
	~CUnit();

	void ParseUnitConfig(std::string& _inFilePath);
	void Initialize();
	void Update();
	void Update(double& _inTimeElapsed);
	float GetHP() { return m_fHP; }
	float TakeDamage(float& _inDamage) { m_fHP -= _inDamage; return m_fHP; }
	void Attack();
	void MoveTo(sf::Vector2i _inTileLocation, sf::Vector2f _inWorldLocation);

private:
	void ExplodeInFlamingGlory();

	//faction
	CUnitEnums::TYPE m_UnitType;
	float m_fHP;
	int m_iMovementPoints;
	float m_fDamage;
	int m_iRange;
	const double m_dAnimeFrameTime = 200.0f;
	double m_dAnimProgress = 0.0f;
	
	sf::Texture* m_texTextureSource;
	sf::Sprite m_sprtUnitSprite;
	sf::Vector2i m_v2iCurrentTileLocation;

	//const posiitions for frames in tilemap
	const sf::IntRect m_AnimationIdleFrame = { 0, 0, 16, 16 };
	const sf::IntRect m_AnimationMoveFrame[2] = { { 16, 0, 16, 16}, { 32, 0, 16, 16} };
	const sf::IntRect m_AnimationAttackFrame[3] = { { 48, 0, 16, 16}, { 64, 0, 16, 16}, { 80, 0, 16, 16} };
};

#endif // !__CUNITS_H__