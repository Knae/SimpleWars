#pragma once
#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__

#include "CScene.h"

class CSceneManager
{
public:
	static CSceneManager& GetRef() { return m_SceneMgr; }
	//{
	//	static CSceneManager m_SceneMgr;
	//	return m_SceneMgr;
	//}
	virtual ~CSceneManager();
	static bool CreateScene(CSceneEnums::SCENETYPE& _inputType,const std::string& _inputConfigPath);


private:
	CSceneManager();
	CSceneManager(CSceneManager const&) = delete;
	void operator=(CSceneManager const&) = delete;

	static CSceneManager m_SceneMgr;
	static CScene* m_ManagedScene;
};

#endif // !__CSCENEMANAGER_H__