#include "CParseConfigCommon.h"

/// <summary>
/// Processes the line provided. Returns the label at the beginning
/// of the line and writes the values into arg2
///
/// </summary>
/// <param name="_inputLine">the line to be parsed</param>
/// <param name="_lineSettingValues">the values that are on that line</param>
/// <returns>the label at the beginning of the line</returns>
std::string CParseConfigCommon::ParseLineGetLabel(const std::string& _inputLine, std::string& _lineSettingValues)
{
	std::size_t positionOfSymbol = _inputLine.find('=');
	if (positionOfSymbol != std::string::npos)
	{
		std::string parsedLabel = _inputLine.substr(0, positionOfSymbol);
		std::string parsedValue = _inputLine.substr(positionOfSymbol + 1, std::string::npos);
		_lineSettingValues = parsedValue;
		return parsedLabel;
	}
	else
	{
		return std::string();
	}
}

/// <summary>
/// Parse the given string to get the corresponding tiletype
/// </summary>
/// <param name="_inputString"></param>
/// <param name="_outType"></param>
/// <returns></returns>
bool CParseConfigCommon::ConvertToTileType(std::string& _inputString, CSceneEnums::TILETYPE& _outType)
{
	if (_inputString.compare("GRASS") == 0)
	{
		_outType = CSceneEnums::TILETYPE::GRASS;
	}
	else if (_inputString.compare("TALLGRASS") == 0)
	{
		_outType = CSceneEnums::TILETYPE::TALLGRASS;
	}
	else if (_inputString.compare("ROAD") == 0)
	{
		_outType = CSceneEnums::TILETYPE::ROAD;
	}
	else if (_inputString.compare("WATER") == 0)
	{
		_outType = CSceneEnums::TILETYPE::COAST;
	}
	else if (_inputString.compare("WATER") == 0)
	{
		_outType = CSceneEnums::TILETYPE::WATER;
	}
	else if (_inputString.compare("HILL") == 0)
	{
		_outType = CSceneEnums::TILETYPE::HILL;
	}
	else if (_inputString.compare("MOUNTAIN") == 0)
	{
		_outType = CSceneEnums::TILETYPE::MOUNTAIN;
	}
	else if (_inputString.compare("URBAN") == 0)
	{
		_outType = CSceneEnums::TILETYPE::URBAN;
	}
	else if (_inputString.compare("FORT") == 0)
	{
		_outType = CSceneEnums::TILETYPE::FORT;
	}
	else if (_inputString.compare("BASETILE") == 0)
	{
		_outType = CSceneEnums::TILETYPE::BASETILE;
	}
	else
	{
		//Unrecognised tile type name
		std::cout << "\nWARN::Unreocignized/Unrecorded tile type" << std::endl;
		return false;
	}
	return true;
}

/// <summary>
/// Converts a given string to the corresponding unit type
/// </summary>
/// <param name="_inputString">String that to be converted to unit type</param>
/// <param name="_outType">where the converted type will be stored. CUnitEnums::TYPE</param>
/// <returns></returns>
bool CParseConfigCommon::ConvertToUnitType(std::string& _inputString, CUnitEnums::TYPE& _outType)
{
	if (_inputString.compare("INFANTRY") == 0)
	{
		_outType = CUnitEnums::TYPE::INFANTRY;
	}
	else if (_inputString.compare("TANK") == 0)
	{
		_outType = CUnitEnums::TYPE::TANK;
	}
	else if (_inputString.compare("ARTILLERY") == 0)
	{
		_outType = CUnitEnums::TYPE::ARTILLERY;
	}
	else
	{
		//Unrecognised line
		std::cout << "\nWARN::Unreocignized/Unrecorded unit type" << std::endl;
		_outType = CUnitEnums::TYPE::NONE;
		return false;
	}
	return true;
}

UIEnums::TURN CParseConfigCommon::Convert(CUnitEnums::SIDE _input)
{
	switch (_input)
	{
		case CUnitEnums::SIDE::BLUE:
		{
			return UIEnums::TURN::BLUE;
			break;
		}
		case CUnitEnums::SIDE::RED:
		{
			return UIEnums::TURN::RED;
			break;
		}
		default:
		{
			return UIEnums::TURN::NONE;
			break;
		}
	}
}

CUnitEnums::SIDE CParseConfigCommon::Convert(UIEnums::TURN _input)
{
	switch (_input)
	{
		case UIEnums::TURN::BLUE:
		{
			return CUnitEnums::SIDE::BLUE;
			break;
		}
		case UIEnums::TURN::RED:
		{
			return CUnitEnums::SIDE::RED;
			break;
		}
		default:
		{
			return CUnitEnums::SIDE::NONE;
			break;
		}
	}
}