#include "CUnit.h"

CUnit::CUnit(	CUnitEnums::TYPE _inUnits, CUnitEnums::SIDE _inSIDE,
				int _tileSize, float _inHP, float _inMovement, float _inDamage, int _inRange,
				CUnitEnums::DIRECTION _inDirection)
{
	m_eUnitType = _inUnits;
	m_fHP = _inHP;
	m_fMovementPointsBase = _inMovement;
	m_fMovementPoints = m_fMovementPointsBase;
	m_fDamage = _inDamage;
	m_iRange = _inRange;
	m_fTileSize = (float)(_tileSize);
	m_eCurrentState = CUnitEnums::STATE::NONE;
	m_eCurrentDirection = _inDirection;
	m_eControllingPlayer = _inSIDE;
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

void CUnit::SetLocation(sf::Vector2f _inPosition)
{
	sf::Vector2u tilePosition(	(unsigned int)(_inPosition.x / m_fTileSize),
								(unsigned int)(_inPosition.y / m_fTileSize));
	m_v2uCurrentTileLocation = tilePosition;
	m_sprtUnitSprite->setPosition(tilePosition.x * m_fTileSize, tilePosition.y * m_fTileSize);
}

/// <summary>
/// Update Function
/// Intended for updating animation frames
/// </summary>
void CUnit::Update()
{
	switch (m_eCurrentState)
	{
		case CUnitEnums::STATE::IDLE:
		{
			break;
		}
		case CUnitEnums::STATE::MOVE:
		{
			sf::Vector2f recordedTileLocation = sf::Vector2f(m_v2uCurrentTileLocation.x * m_fTileSize, m_v2uCurrentTileLocation.y*m_fTileSize);
			if (m_sprtUnitSprite->getPosition() != recordedTileLocation)
			{
				sf::Vector2f spriteLocation = m_sprtUnitSprite->getPosition();
				spriteLocation = LerpMovement(spriteLocation,recordedTileLocation,0.5f);
				m_sprtUnitSprite->setPosition(spriteLocation);
			}
			else
			{
				m_eCurrentState = CUnitEnums::STATE::IDLE;
			}
			break;
		}
		case CUnitEnums::STATE::ATTACK:
		{
			m_eCurrentState = CUnitEnums::STATE::IDLE;
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
	m_eCurrentState = CUnitEnums::STATE::ATTACK;
}

void CUnit::MoveTo(sf::Vector2u _inTileLocation)
{
	m_v2uCurrentTileLocation = _inTileLocation;
	m_eCurrentState = CUnitEnums::STATE::MOVE;
	//m_sprtUnitSprite->setPosition(_inTileLocation.x* m_fTileSize, _inTileLocation.y* m_fTileSize);
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
	m_eCurrentState = CUnitEnums::STATE::DESTROYED;
}

/// <summary>
/// LERP movement based on Jeremy's example.
/// </summary>
/// <param name="_currentPos">origin position. sf::Vector2f&</param>
/// <param name="_destinationPos">desination position. sf::Vector2f&</param>
/// <param name="_fAlpha">defines where the mid point between the 2 positions is. Is a percentage. float</param>
/// <returns></returns>
sf::Vector2f CUnit::LerpMovement(sf::Vector2f& _currentPos, sf::Vector2f& _destinationPos, float _fAlpha)
{
	if (_fAlpha >= 1.0f)
	{
		//ERROR: Incorect alpha, return unchanged coordinates
		return _currentPos;
	}
	else if (	(abs(_destinationPos.x - _currentPos.x) <= 0.1f) &&
				(abs(_destinationPos.y - _currentPos.y) <= 0.1f)	)
	{
		//If the difference between the 2 coordinates is small, return the destination coordinates instead.
		return _destinationPos;
	}

	return(_currentPos * _fAlpha) + (_destinationPos * (1 - _fAlpha));
}