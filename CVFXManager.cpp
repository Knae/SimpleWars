#include "CVFXManager.h"

const double CVFXManager::m_iAnimFrameTimeMax = 200.0f;
double CVFXManager::m_iAnimFrameTime;
float CVFXManager::m_fTileSize;

sf::Texture* CVFXManager::m_pVFXTex_Bullet;
sf::Texture* CVFXManager::m_pVFXTex_Shell;
sf::Texture* CVFXManager::m_pVFXTex_Explode;
sf::Sprite* CVFXManager::m_pVFXSprite;
sf::Sprite* CVFXManager::m_pVFXUnitDeath;

std::string CVFXManager::m_strTexture_BulletEff;
std::string CVFXManager::m_strTexture_ShellEff;
std::string CVFXManager::m_strTexture_ExplosEff;

sf::IntRect CVFXManager::m_VFXIntRect;
sf::IntRect CVFXManager::m_VFXIntRectBase;

CVFXManager::CVFXManager()
{
	m_pVFXSprite = nullptr;
	m_pVFXUnitDeath = nullptr;
	m_pVFXTex_Bullet = nullptr;
	m_pVFXTex_Shell = nullptr;
	m_pVFXTex_Explode = nullptr;
	m_iAnimFrameTime = 0;
	m_fTileSize = 0;
}

CVFXManager::~CVFXManager()
{

	delete m_pVFXTex_Bullet;
	delete m_pVFXTex_Shell;
	delete m_pVFXTex_Explode;

	m_pVFXTex_Bullet = nullptr;
	m_pVFXTex_Shell = nullptr;
	m_pVFXTex_Explode = nullptr;

	if (m_pVFXSprite != nullptr)
	{
		delete m_pVFXSprite;
		m_pVFXSprite = nullptr;
	}

	if (m_pVFXUnitDeath != nullptr)
	{
		delete m_pVFXUnitDeath;
		m_pVFXUnitDeath = nullptr;
	}
}

/// <summary>
/// Initialize the currently unused VFX class
/// </summary>
/// <param name="_inConfigPath"></param>
void CVFXManager::Initialize(std::string& _inConfigPath)
{
	m_strTexture_BulletEff = "assets/spritemaps/BulletEffects.png";
	m_strTexture_ShellEff = "assets/spritemaps/ShellImpact.png";
	m_strTexture_ExplosEff = "assets/spritemaps/ExplosionEffect.png";

	if (m_pVFXTex_Bullet != nullptr)
	{
		delete m_pVFXTex_Bullet;
		m_pVFXTex_Bullet = nullptr;
	}

	m_pVFXTex_Bullet = new sf::Texture;
	if (!m_pVFXTex_Bullet->loadFromFile(m_strTexture_BulletEff))
	{
		//ERROR:Unable to load bullet vfx spritemap
	}

	if (m_pVFXTex_Shell != nullptr)
	{
		delete m_pVFXTex_Shell;
		m_pVFXTex_Shell = nullptr;
	}

	m_pVFXTex_Shell = new sf::Texture;
	if (!m_pVFXTex_Shell->loadFromFile(m_strTexture_ShellEff))
	{
		//ERROR:Unable to load shell vfx spritemap
	}

	if (m_pVFXTex_Explode != nullptr)
	{
		delete m_pVFXTex_Explode;
		m_pVFXTex_Explode = nullptr;
	}

	m_pVFXTex_Explode = new sf::Texture();
	if (!m_pVFXTex_Explode->loadFromFile(m_strTexture_ExplosEff))
	{
		//ERROR:Unable to load bullet vfx spritemap
	}

	m_VFXIntRect = sf::IntRect(0, 0, 32, 32);
	m_VFXIntRectBase = m_VFXIntRect;
	//Set this through game manager?
	m_fTileSize = 32.0f;
}

/// <summary>
/// Display any instanced vfx sprites.
/// If none, then all VFX particles have finished animating and
/// have been deleted. Return true if so.
/// </summary>
/// <param name="_inWindow">Window to diplay the sprite. sf::Window&</param>
/// <param name="_inElapsedTime">Elapsed time since last call, in microseconds. double&</param>
/// <returns></returns>
bool CVFXManager::Display(sf::RenderWindow& _inWindow)
{
	if (m_pVFXSprite != nullptr)
	{
		//std::cout << "\nDisplaying attack VFX!" << std::endl;
		_inWindow.draw(*m_pVFXSprite);
		return true;
	}
	else if (m_pVFXUnitDeath != nullptr)
	{
		//std::cout << "\nDisplaying death VFX!" << std::endl;
		_inWindow.draw(*m_pVFXUnitDeath);
		return true;
	}
	else
	{
		//ERROR: No VFX to generate
		return false;
	}

}

/// <summary>
/// Add an attack VFX to specified tile based on the unitType
/// </summary>
/// <param name="_inTilePosition"></param>
/// <param name="_inUnitType"></param>
/// <returns></returns>
bool CVFXManager::AddAttackVFX(sf::Vector2u& _inTilePosition, CUnitEnums::TYPE _inUnitType)
{
	//Only proceed if there's no existing VFXSprite
	if (m_pVFXSprite == nullptr)
	{
		switch (_inUnitType)
		{
			case CUnitEnums::TYPE::INFANTRY:
			{
				AddAttackParticles_Bullet(_inTilePosition);
				break;
			}
			case CUnitEnums::TYPE::TANK:
			case CUnitEnums::TYPE::ARTILLERY:
			{
				AddAttackParticles_Shell(_inTilePosition);
				break;
			}
			case CUnitEnums::TYPE::NONE:
			default:
				break;
		}
		std::cout << "\nAdding attack VFX to tile (" << _inTilePosition.x << ", " << _inTilePosition.y + ")!" << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// Add a death explosion at the specified tile location
/// </summary>
/// <param name="_inTilePosition"></param>
/// <returns></returns>
bool CVFXManager::AddDeathVFX(sf::Vector2u& _inTilePosition)
{
	if (m_pVFXUnitDeath == nullptr)
	{
		AddDeathParticles_Explosive(_inTilePosition);
		return true;
	}
	else
	{
		return false;
	}

}

/// <summary>
/// Update the animation frame of the VFX sprite
/// </summary>
/// <param name="_inElapsedTime"></param>
/// <returns>true if there still some VFX sprite to be displayed</returns>
bool CVFXManager::UpdateVFX(double& _inElapsedTime)
{
	//If there's a VFXsprite to display, increase
	//the animation timer
	if (m_pVFXSprite != nullptr || m_pVFXUnitDeath != nullptr)
	{
		m_iAnimFrameTime += _inElapsedTime;
	}

	//If the timer has reached the animation frame time,
	//move to the next frame and subtract the frame time
	//from the timer
	if (m_iAnimFrameTime >= m_iAnimFrameTimeMax)
	{
		m_iAnimFrameTime -= m_iAnimFrameTimeMax;
		int rectLeft = m_VFXIntRect.left + 32;
		//If we've gone past the last frame,
		//delete the sprite and reset the timer to 0
		//Only return false if both VFX and deathVFX are
		//nullptr
		if (rectLeft >= 96)
		{
			m_VFXIntRect = m_VFXIntRectBase;
			m_iAnimFrameTime = 0;
			if (m_pVFXSprite != nullptr)
			{
				delete m_pVFXSprite;
				m_pVFXSprite = nullptr;
			}
			else if (m_pVFXUnitDeath != nullptr)
			{
				delete m_pVFXUnitDeath;
				m_pVFXUnitDeath = nullptr;
			}

			if (m_pVFXSprite == nullptr && m_pVFXUnitDeath == nullptr)
			{
				return false;
			}
		}
		else
		{
			//We're updating the frame positions, so the 
			//sprite we're updating better not be a nullptr
			m_VFXIntRect.left = rectLeft;
			if (m_pVFXSprite != nullptr)
			{
				m_pVFXSprite->setTextureRect(m_VFXIntRect);
			}
			else if (m_pVFXUnitDeath != nullptr)
			{
				m_pVFXUnitDeath->setTextureRect(m_VFXIntRect);
			}

			return true;
		}
	}

	return false;
}

//Create a new VFX sprite, set it to the bullet effect animation
bool CVFXManager::AddAttackParticles_Bullet(sf::Vector2u& _inTilePosition)
{
	if (m_pVFXSprite == nullptr)
	{
		m_pVFXSprite = new sf::Sprite;
		m_pVFXSprite->setTexture(*m_pVFXTex_Bullet);
		m_pVFXSprite->setTextureRect(m_VFXIntRect);
		SetVFXPosition(_inTilePosition);
		return true;
	}
	else
	{
		//ERROR: Existing VFX in queue to play
		return false;
	}
}

//Create a new VFX sprite, set it to the smoke effect animation
bool CVFXManager::AddAttackParticles_Shell(sf::Vector2u& _inTilePosition)
{
	if (m_pVFXSprite == nullptr)
	{
		m_pVFXSprite = new sf::Sprite;
		m_pVFXSprite->setTexture(*m_pVFXTex_Shell);
		m_pVFXSprite->setTextureRect(m_VFXIntRect);
		SetVFXPosition(_inTilePosition);
		return true;
	}
	else
	{
		//ERROR: Existing VFX in queue to play
		return false;
	}
}

//Create a new VFX sprite, set it to the explosive effect animation
bool CVFXManager::AddDeathParticles_Explosive(sf::Vector2u& _inTilePosition)
{
	if (m_pVFXUnitDeath == nullptr)
	{
		m_pVFXUnitDeath = new sf::Sprite;
		m_pVFXUnitDeath->setTexture(*m_pVFXTex_Explode);
		m_pVFXUnitDeath->setTextureRect(m_VFXIntRect);
		SetDeathVFXPosition(_inTilePosition);
		return true;
	}
	else
	{
		//ERROR: Existing VFX in queue to play
		return false;
	}
}

