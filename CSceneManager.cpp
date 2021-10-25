#include "CSceneManager.h"
CSceneManager CSceneManager::m_SceneMgr;
CScene* CSceneManager::m_ManagedScene;

CSceneManager::CSceneManager()
{
	m_ManagedScene = nullptr;
}

CSceneManager::~CSceneManager()
{
	delete m_ManagedScene;
	m_ManagedScene = nullptr;
}

bool CSceneManager::CreateScene(CSceneEnums::SCENETYPE& _inputType,const std::string& _inputConfigPath)
{
	if (_inputType == CSceneEnums::SCENETYPE::MOUNTAINGRASS)
	{
		if (m_ManagedScene != nullptr)
		{
			delete m_ManagedScene;
			m_ManagedScene = nullptr;
		}
		m_ManagedScene = new CScene();
		m_ManagedScene->LoadMapConfig(_inputConfigPath);
		return true;
	}
	else
	{
		return false;
	}
}