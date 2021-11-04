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
