#pragma once
#ifndef __UIEnums_H_
#define __UIEnums_H_

class UIEnums
{
public:
	enum class TURN
	{
		NONE,
		BLUE,
		RED,
	};

	enum class MOUSESTATE
	{
		NONE,
		SELECTING
	};

	enum class GAMESTATE
	{
		NONE,
		MAPSELECTION,
		UNITPLACEMENT,
		GAMELOOP
	};

private:
};

#endif // !__UIEnums_H_

