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
	static bool UpdateVFX(double& _inElapsedTime);
	static bool Display(sf::RenderWindow& _inWindow);
	static bool AddAttackParticles_Bullet(sf::Vector2u& _inTilePosition);
	static bool AddAttackParticles_Shell(sf::Vector2u& _inTilePosition);
	static bool AddDeathParticles_Explosive(sf::Vector2u& _inTilePosition);

private:
	CVFXManager();
	CVFXManager(CVFXManager const&) = delete;
	void operator=(CVFXManager const&) = delete;

	static void SetVFXPosition(sf::Vector2u& _inTilePosition) 
	{
		m_pVFXSprite->setPosition(_inTilePosition.x * m_fTileSize, _inTilePosition.y * m_fTileSize);
	};

	static const double m_iAnimFrameTimeMax;
	static double m_iAnimFrameTime;
	static float m_fTileSize;

	static sf::Texture* m_pVFXTex_Bullet;
	static sf::Texture* m_pVFXTex_Shell;
	static sf::Texture* m_pVFXTex_Explode;
	static sf::Sprite* m_pVFXSprite;

	static std::string m_strTexture_BulletEff;
	static std::string m_strTexture_ShellEff;
	static std::string m_strTexture_ExplosEff;

	static sf::IntRect m_VFXIntRect;
	static sf::IntRect m_VFXIntRectBase;
};

#endif // !__CVFXMANAGER_H__