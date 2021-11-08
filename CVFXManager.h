#pragma once
#ifndef __CVFXMANAGER_H__
#define __CVFXMANAGER_H__
#include <sfml\Graphics.hpp>
#include <vector>
#include "CParseConfigCommon.h"

class CVFXManager
{
public:
	static CVFXManager& GetRef() { static CVFXManager m_vfxMgrThis; return m_vfxMgrThis; }
	~CVFXManager();

	static void Initialize(std::string& _inConfigPath);
	static bool Display(sf::RenderWindow& _inWindow, double& _inElapsedTime);
	static void AddAttackParticles_Bullet(sf::Vector2u& _inTilePosition);
	static void AddAttackParticles_Shell(sf::Vector2u& _inTilePosition);
	static void AddDeathParticles_Explosive(sf::Vector2u& _inTilePosition);

private:
	CVFXManager();
	CVFXManager(CVFXManager const&) = delete;
	void operator=(CVFXManager const&) = delete;

	static bool UpdateVFX(double& _inElapsedTime);

	static sf::Texture* m_texVFX_Bullet;
	static sf::Texture* m_texVFX_Shell;
	static sf::Texture* m_texVFX_Explode;
	static sf::Sprite* m_sprtVFX;

	static std::string m_strTexture_BulletEff;
	static std::string m_strTexture_ShellEff;
	static std::string m_strTexture_ExplosEff;

	static sf::IntRect m_VFXIntRect;
};

#endif // !__CVFXMANAGER_H__