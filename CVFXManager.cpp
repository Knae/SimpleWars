#include "CVFXManager.h"

sf::Texture* CVFXManager::m_pVFXTex_Bullet;
sf::Texture* CVFXManager::m_pVFXTex_Shell;
sf::Texture* CVFXManager::m_pVFXTex_Explode;
sf::Sprite* CVFXManager::m_pVFXSprite;

std::string CVFXManager::m_strTexture_BulletEff;
std::string CVFXManager::m_strTexture_ShellEff;
std::string CVFXManager::m_strTexture_ExplosEff;

sf::IntRect CVFXManager::m_VFXIntRect;

CVFXManager::CVFXManager()
{
	m_pVFXSprite = nullptr;
	m_pVFXTex_Bullet = nullptr;
	m_pVFXTex_Shell = nullptr;
	m_pVFXTex_Explode = nullptr;
}

CVFXManager::~CVFXManager()
{
	delete m_pVFXSprite;
	delete m_pVFXTex_Bullet;
	delete m_pVFXTex_Shell;
	delete m_pVFXTex_Explode;

	m_pVFXSprite = nullptr;
	m_pVFXTex_Bullet = nullptr;
	m_pVFXTex_Shell = nullptr;
	m_pVFXTex_Explode = nullptr;
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

	if (!m_pVFXTex_Bullet->loadFromFile(m_strTexture_BulletEff))
	{
		//ERROR:Unable to load bullet vfx spritemap
	}

	if (m_pVFXTex_Shell != nullptr)
	{
		delete m_pVFXTex_Shell;
		m_pVFXTex_Shell = nullptr;
	}

	if (!m_pVFXTex_Shell->loadFromFile(m_strTexture_ShellEff))
	{
		//ERROR:Unable to load shell vfx spritemap
	}

	if (m_pVFXTex_Explode != nullptr)
	{
		delete m_pVFXTex_Explode;
		m_pVFXTex_Explode = nullptr;
	}

	if (!m_pVFXTex_Explode->loadFromFile(m_strTexture_ExplosEff))
	{
		//ERROR:Unable to load bullet vfx spritemap
	}

	m_VFXIntRect = sf::IntRect(0, 0, 32, 32);

}

/// <summary>
/// Display any instanced vfx sprites.
/// If none, then all VFX particles have finished animating and
/// have been deleted. Return true if so.
/// </summary>
/// <param name="_inWindow">Window to diplay the sprite. sf::Window&</param>
/// <param name="_inElapsedTime">Elapsed time since last call, in microseconds. double&</param>
/// <returns></returns>
bool CVFXManager::Display(sf::RenderWindow& _inWindow, double& _inElapsedTime)
{
	if (m_pVFXSprite != nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}

}

/// <summary>
/// UpdateInfoDisplay the animation frame of the VFX sprite
/// </summary>
/// <param name="_inElapsedTime"></param>
/// <returns></returns>
bool CVFXManager::UpdateVFX(double& _inElapsedTime)
{
	return false;
}


void CVFXManager::AddAttackParticles_Bullet(sf::Vector2u& _inTilePosition)
{
	m_pVFXSprite = new sf::Sprite;
}

void CVFXManager::AddAttackParticles_Shell(sf::Vector2u& _inTilePosition)
{
	m_pVFXSprite = new sf::Sprite;
}

void CVFXManager::AddDeathParticles_Explosive(sf::Vector2u& _inTilePosition)
{
	m_pVFXSprite = new sf::Sprite;
}

