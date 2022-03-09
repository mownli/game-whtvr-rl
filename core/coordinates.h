#ifndef COORDINATES_H
#define COORDINATES_H


struct Coordinates
{
	int x = 0;
	int y = 0;

	Coordinates operator-(const Coordinates& op) noexcept
	{
		Coordinates ret;
		ret.x = op.x - x;
		ret.y = op.y - y;
		return ret;
	}

	Coordinates operator+(const Coordinates& op) noexcept
	{
		Coordinates ret;
		ret.x = x + op.x;
		ret.y = y + op.y;
		return ret;
	}

	bool is_not_negative() const noexcept
	{
		return (x >= 0 && y >= 0) ? true : false;
	}

	bool operator==(const Coordinates& op) const noexcept
	{
		return (x == op.x && y == op.y) ? true : false;
	}

	bool is_unit() const noexcept
	{
		return ((x >= -1 && x <= 1) && (y >= -1 && y <= 1)) ? true : false;
	}
};

#endif // COORDINATES_H
