#pragma once
#ifndef __CSCENEENUMS_H__
#define __CSCENEENUMS_H__


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
		WATER,
		HILL,
		MOUNTAIN,
		URBAN,
		FORT
	};
};

#endif // !__CSCENEENUMS_H__