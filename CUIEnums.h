#pragma once
#ifndef __UIEnums_H_
#define __UIEnums_H_

class CUIEnums
{
public:
	enum class TURN
	{
		NONE,
		BLUE,
		RED,
	};

	enum class GAMESTATE
	{
		NONE,
		MAPSELECTION,
		UNITPLACEMENT,
		GAMELOOP,
	};

	enum class MOUSESTATE
	{
		NONE,
		SELECT,
		MOVE,
		ATTACK
	};
private:
};

#endif // !__UIEnums_H_

