#include "CUnit.h"

CUnit::CUnit(	CUnitEnums::TYPE _inUnits,
				int _tileSize, float _inHP, float _inMovement, float _inDamage, int _inRange,
				CUnitEnums::DIRECTION _inDirection)
{
	m_UnitType = _inUnits;
	m_fHP = _inHP;
	m_fMovementPointsBase = _inMovement;
	m_fMovementPoints = m_fMovementPointsBase;
	m_fDamage = _inDamage;
	m_iRange = _inRange;
	m_fTileSize = (float)(_tileSize);
	m_CurrentState = CUnitEnums::STATE::NONE;
	m_CurrenDirection = _inDirection;
	//m_texTextureSource = nullptr;
	m_sprtUnitSprite = nullptr;
	m_eFaction = CUnitEnums::FACTION::TALONS;
}

CUnit::~CUnit()
{
	if (m_sprtUnitSprite != nullptr)
	{
		delete m_sprtUnitSprite;
		m_sprtUnitSprite = nullptr;
	}
}

//void CUnit::Initialize(sf::Texture* _inTextureSource)
void CUnit::Initialize(sf::Sprite* _inSpriteSource)
{
	//m_texTextureSource = _inTextureSource;
	m_sprtUnitSprite = _inSpriteSource;
	//m_sprtUnitSprite->setTexture(*m_texTextureSource);
	m_fMovementPoints = m_fMovementPointsBase;
}

void CUnit::Update()
{
	switch (m_CurrentState)
	{
		case CUnitEnums::STATE::IDLE:
		{
			break;
		}
		case CUnitEnums::STATE::MOVE:
		{
			break;
		}
		case CUnitEnums::STATE::ATTACK:
		{
			break;
		}
		case CUnitEnums::STATE::DESTROYED:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void CUnit::Update(double& _inTimeElapsed)
{
	m_dAnimProgress += _inTimeElapsed;
	if (m_dAnimProgress >= m_dAnimeFrameTime)
	{
		Update();
	}
}

void CUnit::Attack()
{
	m_CurrentState = CUnitEnums::STATE::ATTACK;
}

void CUnit::MoveTo(sf::Vector2u _inTileLocation)
{
	m_v2uCurrentTileLocation = _inTileLocation;
	m_sprtUnitSprite->setPosition(_inTileLocation.x* m_fTileSize, _inTileLocation.y* m_fTileSize);
}

void CUnit::MoveTo(sf::Vector2f _inPosition)
{
	sf::Vector2u tilePosition(	(unsigned int)(_inPosition.x / m_fTileSize),
								(unsigned int)(_inPosition.y / m_fTileSize));
	MoveTo(tilePosition);
}

void CUnit::Replenish()
{
	m_fMovementPoints = m_fMovementPointsBase;
}

void CUnit::ExplodeInFlamingGlory()
{
	//Boom Boom Animation.
	m_CurrentState = CUnitEnums::STATE::DESTROYED;
}
