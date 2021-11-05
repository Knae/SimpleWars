#pragma once
#ifndef __CPARSECONFIGCOMMON_H__
#define __CPARSECONFIGCOMMON_H__

#include <string>
#include <iostream>
#include "CSceneEnums.h"
#include "CUnitEnums.h"

class CParseConfigCommon
{
public:
	
protected:
	static std::string ParseLineGetLabel(const std::string& _inputLine, std::string& _lineSettingValues);
	static bool ConvertToTileType(std::string& _inputString, CSceneEnums::TILETYPE& _outType);
	static bool ConvertToUnitType(std::string& _inputString, CUnitEnums::TYPE& _outType);
};

#endif // !__CPARSECONFIGCOMMON_H__