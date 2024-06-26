#pragma once
#ifndef __CPARSECONFIGCOMMON_H__
#define __CPARSECONFIGCOMMON_H__

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "CSceneEnums.h"
#include "CUnitEnums.h"
#include "CUIEnums.h"

/// <summary>
/// Needs to be renamed to common conversion and parse functions
/// </summary>

class CParseConfigCommon
{
public:
	static std::string ParseLineGetLabel(const std::string& _inputLine, std::string& _lineSettingValues);
	static bool ConvertToTileType(std::string& _inputString, CSceneEnums::TILETYPE& _outType);
	static bool ConvertToUnitType(std::string& _inputString, CUnitEnums::TYPE& _outType);
	static bool ConvertUnitTypeToString(CUnitEnums::TYPE& _inType, std::string& _outString);
	static CUIEnums::TURN Convert(CUnitEnums::SIDE _input);	
	static CUnitEnums::SIDE Convert(CUIEnums::TURN _input);
};

#endif // !__CPARSECONFIGCOMMON_H__