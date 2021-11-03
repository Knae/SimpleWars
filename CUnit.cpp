#include "CUnit.h"

CUnit::CUnit(	CUnitEnums::TYPE _inUnits, CUnitEnums::DIRECTION _inDirection,
				float _tileSize, float _inHP, float _inMovement, float _inDamage, int _inRange)
{
	m_UnitType = _inUnits;
	m_fHP = _inHP;
	m_fMovementPointsBase = _inMovement;
	m_fMovementPoints = m_fMovementPointsBase;
	m_fDamage = _inDamage;
	m_iRange = _inRange;
	m_fTileSize = _tileSize;
	m_CurrentState = CUnitEnums::STATE::NONE;
	m_CurrenDirection = _inDirection;
	m_texTextureSource = nullptr;
	m_sprtUnitSprite = nullptr;
}

CUnit::~CUnit()
{
	if (m_sprtUnitSprite != nullptr)
	{
		delete m_sprtUnitSprite;
		m_sprtUnitSprite = nullptr;
	}
}

void CUnit::Initialize(sf::Texture* _inTextureSource)
{
	m_texTextureSource = _inTextureSource;
	m_sprtUnitSprite = new sf::Sprite();
	m_sprtUnitSprite->setTexture(*m_texTextureSource);
	m_fMovementPoints = m_fMovementPointsBase;
}

void CUnit::SetHP(float& _inHP)
{
	m_fHP = _inHP;
}

void CUnit::SetMovement(float& _inMovement)
{
	m_fMovementPointsBase = _inMovement;
}

void CUnit::SetDamageDealt(float& _inDamage)
{
	m_fDamage = _inDamage;
}

void CUnit::SetRange(int& _inRange)
{
	m_iRange = _inRange;
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

void CUnit::MoveTo(sf::Vector2i _inTileLocation)
{
	m_v2iCurrentTileLocation = _inTileLocation;
	m_sprtUnitSprite->setPosition(_inTileLocation.x* m_fTileSize, _inTileLocation.y* m_fTileSize);
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
