#pragma once
#ifndef __CPARSECONFIGCOMMON_H__
#define __CPARSECONFIGCOMMON_H__

#include <string>

class CParseConfigCommon
{
public:
	
protected:
	static std::string ParseLineGetLabel(const std::string& _inputLine, std::string& _lineSettingValues);
};

#endif // !__CPARSECONFIGCOMMON_H__