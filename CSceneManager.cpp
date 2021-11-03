#include "CSceneManager.h"
CSceneManager CSceneManager::m_SceneMgr;
CScene* CSceneManager::m_pCurrentManagedScene;

CSceneManager::CSceneManager()
{
	m_pCurrentManagedScene = nullptr;
}

CSceneManager::~CSceneManager()
{
	delete m_pCurrentManagedScene;
	m_pCurrentManagedScene = nullptr;
}

/// <summary>
/// Loads a new map by deleting the old one creating a new one.
/// Pass the correponding ini map config file and tell it to build it
/// </summary>
/// <param name="_inputType - What scene to build"></param>
/// <param name="_inputConfigPath - path to the config file"></param>
/// <returns></returns>
bool CSceneManager::CreateScene(CSceneEnums::SCENETYPE& _inputType,const std::string& _inputConfigPath)
{
	if (_inputType == CSceneEnums::SCENETYPE::MOUNTAINGRASS)
	{
		if (m_pCurrentManagedScene != nullptr)
		{
			delete m_pCurrentManagedScene;
			m_pCurrentManagedScene = nullptr;
		}
		m_pCurrentManagedScene = new CScene();
		m_pCurrentManagedScene->LoadMapConfig(_inputConfigPath);
		m_pCurrentManagedScene->InitializeMap();
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// Display the currently managed scene
/// </summary>
/// <param name="_targetWindow" - the SFML window it is to be displayed></param>
void CSceneManager::DisplayScene(sf::RenderWindow& _targetWindow)
{
	int* sceneColourValues = m_pCurrentManagedScene->GetBaseColourArrayPointer();
	sf::Color sceneBackground(*sceneColourValues, *(sceneColourValues + 1), *(sceneColourValues + 2));
	_targetWindow.clear(sceneBackground);
	_targetWindow.draw(*m_pCurrentManagedScene);
}
