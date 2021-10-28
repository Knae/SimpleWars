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