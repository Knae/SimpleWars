#pragma once
#ifndef __CUIMANAGER_H__
#define __CUIMANAGER_H__

#include<SFML/Graphics.hpp>

class CUIManager
{
public:
	~CUIManager();
	static CUIManager& GetRef() { static CUIManager m_UIMgr; return m_UIMgr; }

	//void IntializeUI();
private:
	CUIManager();
	CUIManager(CUIManager const&) = delete;
	void operator=(CUIManager const&) = delete;
};

#endif // !__CUIMANAGER_H__