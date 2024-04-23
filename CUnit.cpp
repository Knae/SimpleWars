#include "CUnit.h"
#include "CAudioManager.h"

CUnit::CUnit(	CUnitEnums::TYPE _inUnits, CUnitEnums::SIDE _inSIDE,
				int _tileSize, float _inHP, float _inMovement, float _inDamage, int _inRange,
				CUnitEnums::DIRECTION _inDirection)
{
	m_eUnitType = _inUnits;
	m_fHP = _inHP;
	m_fMovePoints_Base = _inMovement;
	m_fMovePoints = m_fMovePoints_Base;
	m_fDamage = _inDamage;
	m_iRange = _inRange;
	m_fTileSize = (float)(_tileSize);
	m_eCurrentState = CUnitEnums::STATE::NONE;
	m_eCurrentDirection = _inDirection;
	m_eControllingPlayer = _inSIDE;
	m_UnitSprite = nullptr;
	m_eFaction = CUnitEnums::FACTION::TALONS;
	m_eCurrentTileType = CSceneEnums::TILETYPE::NONE;
	m_bHasAttacked = false;
}

CUnit::~CUnit()
{
	if (m_UnitSprite != nullptr)
	{
		delete m_UnitSprite;
		m_UnitSprite = nullptr;
	}
}

/// <summary>
/// Initializes the unit's sprite source and starting MOV points
/// </summary>
/// <param name="_inSpriteSource"></param>
void CUnit::Initialize(sf::Sprite* _inSpriteSource)
{
	m_UnitSprite = _inSpriteSource;
	m_fMovePoints = m_fMovePoints_Base;
}

/// <summary>
/// Converts the position to it's int equivalent before setting
/// its location
/// </summary>
/// <param name="_inPosition"></param>
void CUnit::SetLocation(sf::Vector2f _inPosition)
{
	sf::Vector2u tilePosition(	(unsigned int)(_inPosition.x / m_fTileSize),
								(unsigned int)(_inPosition.y / m_fTileSize));
	SetLocation(tilePosition);
}

/// <summary>
/// Sets the units current tile location and also 
/// it's display position 
/// </summary>
/// <param name="_inPosition"></param>
void CUnit::SetLocation(sf::Vector2u _inPosition)
{
	m_CurrentTileLocation = _inPosition;
	m_UnitSprite->setPosition(_inPosition.x * m_fTileSize, _inPosition.y * m_fTileSize);
}

/// <summary>
/// UpdateInfoDisplay Function
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
			sf::Vector2f recordedTileLocation = sf::Vector2f(m_CurrentTileLocation.x * m_fTileSize, m_CurrentTileLocation.y*m_fTileSize);
			if (m_UnitSprite->getPosition() != recordedTileLocation)
			{
				sf::Vector2f spriteLocation = m_UnitSprite->getPosition();
				spriteLocation = LerpMovement(spriteLocation,recordedTileLocation,0.5f);
				m_UnitSprite->setPosition(spriteLocation);
			}
			else
			{
				m_eCurrentState = CUnitEnums::STATE::IDLE;
				m_bIsActing = false;
			}
			break;
		}
		case CUnitEnums::STATE::ATTACK:
		{
			m_eCurrentState = CUnitEnums::STATE::IDLE;
			m_bIsActing = false;
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

/// <summary>
/// Expanded update function to enable animations
/// TODO: change texture based on animProgress
/// </summary>
/// <param name="_inTimeElapsed"></param>
void CUnit::Update(double& _inTimeElapsed)
{
	m_dAnimProgress += _inTimeElapsed;
	if (m_dAnimProgress >= m_dAnimeFrameTime)
	{
		Update();
	}
}

/// <summary>
/// Set current state to attac. Meant for animations
/// </summary>
void CUnit::Attack()
{
	m_eCurrentState = CUnitEnums::STATE::ATTACK;
	m_bIsActing = true;
}

/// <summary>
/// Set this unit to enter the given tile index
/// </summary>
/// <param name="_inTileLocation">Tile location in terms on index, given in Vector2u form</param>
void CUnit::MoveTo(sf::Vector2u _inTileLocation)
{
	m_CurrentTileLocation = _inTileLocation;
	m_eCurrentState = CUnitEnums::STATE::MOVE;
	IncrementMovePoints(-1.0f);
	m_bIsActing = true;
}

/// <summary>
/// Convert the given float position to tile index form before passing it
/// to MoveTo(Vector2u) to "Set this unit to enter the given tile index"
/// </summary>
/// <param name="_inPosition">mouse/pixel location=, given in Vector2f form</param>
void CUnit::MoveTo(sf::Vector2f _inPosition)
{
	sf::Vector2u tilePosition(	(unsigned int)(_inPosition.x / m_fTileSize),
								(unsigned int)(_inPosition.y / m_fTileSize));
	MoveTo(tilePosition);
}

/// <summary>
/// Reset its MOV points and its state back to idle
/// </summary>
void CUnit::Replenish()
{
	if (m_eCurrentState != CUnitEnums::STATE::DESTROYED)
	{
		m_fMovePoints = m_fMovePoints_Base;
		m_bHasAttacked = false;
		m_eCurrentState = CUnitEnums::STATE::IDLE;
	}
}

void CUnit::ExplodeInFlamingGlory()
{
	//Boom Boom Animation goes here.
	m_eCurrentState = CUnitEnums::STATE::DESTROYED;
	CAudioManager::getRef().AddSoundEvent(m_strSoundFile_Death);
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