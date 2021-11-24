#include "CSceneManager.h"
CSceneManager CSceneManager::m_SceneMgr;
CScene* CSceneManager::m_pCurrentManagedScene;
sf::RenderTexture* CSceneManager::m_pSceneBackground;
sf::Sprite* CSceneManager::m_pSpriteBackground;

CSceneManager::CSceneManager()
{
	m_pCurrentManagedScene = nullptr;
	m_pSpriteBackground = nullptr;
	m_pSceneBackground = nullptr;
}

CSceneManager::~CSceneManager()
{
	delete m_pCurrentManagedScene;
	delete m_pSpriteBackground;
	delete m_pSceneBackground;

	m_pCurrentManagedScene = nullptr;
	m_pSpriteBackground = nullptr;
	m_pSceneBackground = nullptr;
}

/// <summary>
/// Loads a new map by deleting the old one creating a new one.
/// Pass the correponding ini map config file and tell it to build it
/// </summary>
/// <param name="_inputConfigPath - path to the config file"></param>
/// <returns></returns>
bool CSceneManager::CreateScene(const std::string& _inputConfigPath)
{
	if (m_pCurrentManagedScene != nullptr)
	{
		delete m_pCurrentManagedScene;
		m_pCurrentManagedScene = nullptr;
	}

	if (m_pSceneBackground != nullptr)
	{
		delete m_pSceneBackground;
		m_pSceneBackground = nullptr;
	}

	if (m_pSpriteBackground != nullptr)
	{
		delete m_pSpriteBackground;
		m_pSpriteBackground = nullptr;
	}

	m_pCurrentManagedScene = new CScene();
	m_pCurrentManagedScene->ParseConfig(_inputConfigPath);
	m_pCurrentManagedScene->InitializeMap();

	//=============================================================================================================
	//Needs to be a pointer because we're reading an array of 3 colour values.
	int* sceneColourValues = m_pCurrentManagedScene->GetBaseColourArrayPointer();
	sf::Color sceneBackgroundColor( *sceneColourValues, *(sceneColourValues + 1), *(sceneColourValues + 2));
	//=============================================================================================================
	m_pSceneBackground = new sf::RenderTexture();
	m_pSceneBackground->create(m_pCurrentManagedScene->GetSceneWidth_Pixels(), m_pCurrentManagedScene->GetSceneHeight_Pixels());
	m_pSceneBackground->clear(sceneBackgroundColor);

	m_pSpriteBackground = new sf::Sprite();
	m_pSpriteBackground->setTexture(m_pSceneBackground->getTexture(), true);
	return true;
}

/// <summary>
/// Display the currently managed scene
/// </summary>
/// <param name="_targetWindow" - the SFML window it is to be displayed></param>
void CSceneManager::DisplayScene(sf::RenderWindow& _targetWindow)
{
	_targetWindow.draw(*m_pSpriteBackground);
	_targetWindow.draw(*m_pCurrentManagedScene);
}
