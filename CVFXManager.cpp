#include "CVFXManager.h"

sf::Texture* CVFXManager::m_texVFX_Bullet;
sf::Texture* CVFXManager::m_texVFX_Shell;
sf::Texture* CVFXManager::m_texVFX_Explode;
sf::Sprite* CVFXManager::m_sprtVFX;

std::string CVFXManager::m_strTexture_BulletEff;
std::string CVFXManager::m_strTexture_ShellEff;
std::string CVFXManager::m_strTexture_ExplosEff;

sf::IntRect CVFXManager::m_VFXIntRect;

CVFXManager::CVFXManager()
{
	m_sprtVFX = nullptr;
	m_texVFX_Bullet = nullptr;
	m_texVFX_Shell = nullptr;
	m_texVFX_Explode = nullptr;
}

CVFXManager::~CVFXManager()
{
	delete m_sprtVFX;
	delete m_texVFX_Bullet;
	delete m_texVFX_Shell;
	delete m_texVFX_Explode;

	m_sprtVFX = nullptr;
	m_texVFX_Bullet = nullptr;
	m_texVFX_Shell = nullptr;
	m_texVFX_Explode = nullptr;
}

void CVFXManager::Initialize(std::string& _inConfigPath)
{
	m_strTexture_BulletEff = "assets/spritemaps/BulletEffects.png";
	m_strTexture_ShellEff = "assets/spritemaps/ShellImpact.png";
	m_strTexture_ExplosEff = "assets/spritemaps/ExplosionEffect.png";

	if (m_texVFX_Bullet != nullptr)
	{
		delete m_texVFX_Bullet;
		m_texVFX_Bullet = nullptr;
	}

	if (!m_texVFX_Bullet->loadFromFile(m_strTexture_BulletEff))
	{
		//ERROR:Unable to load bullet vfx spritemap
	}

	if (m_texVFX_Shell != nullptr)
	{
		delete m_texVFX_Shell;
		m_texVFX_Shell = nullptr;
	}

	if (!m_texVFX_Shell->loadFromFile(m_strTexture_ShellEff))
	{
		//ERROR:Unable to load shell vfx spritemap
	}

	if (m_texVFX_Explode != nullptr)
	{
		delete m_texVFX_Explode;
		m_texVFX_Explode = nullptr;
	}

	if (!m_texVFX_Explode->loadFromFile(m_strTexture_ExplosEff))
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
	if (m_sprtVFX != nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}

}

/// <summary>
/// Update the animation frame of the VFX sprite
/// </summary>
/// <param name="_inElapsedTime"></param>
/// <returns></returns>
bool CVFXManager::UpdateVFX(double& _inElapsedTime)
{
	return false;
}


void CVFXManager::AddAttackParticles_Bullet(sf::Vector2u& _inTilePosition)
{
	m_sprtVFX = new sf::Sprite;
}

void CVFXManager::AddAttackParticles_Shell(sf::Vector2u& _inTilePosition)
{
}

void CVFXManager::AddDeathParticles_Explosive(sf::Vector2u& _inTilePosition)
{
}

