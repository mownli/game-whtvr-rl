#ifndef CELL_H
#define CELL_H

#include <vector>


class Cell
{
public:
	enum class CellType
	{
		WALL,
		INVALID,
		SIMPLE
	};
	CellType type;

	Cell(CellType type) noexcept : type(type) {};
};

#endif // CELL_H
