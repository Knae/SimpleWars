#pragma once
#ifndef __CSCENEENUMS_H__
#define __CSCENEENUMS_H__

/// <summary>
/// Enums used specifically by SceneManager
/// </summary>
class CSceneEnums
{
public:
	enum class SCENETYPE 
	{
		NONE,
		MOUNTAINGRASS,
	};

	enum class TILETYPE
	{
		NONE,
		GRASS,
		TALLGRASS,
		ROAD,
		COAST,
		WATER,
		HILL,
		MOUNTAIN,
		URBAN,
		FORT,
		BASETILE
	};
};

#endif // !__CSCENEENUMS_H__