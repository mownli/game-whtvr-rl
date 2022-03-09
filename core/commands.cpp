#include "commands.h"

#include <cassert>


static int move(Object& obj, const Coordinates& xy) noexcept
{
	assert(obj.mobility);
	assert(obj.dynamics);

	return
}


static void register_actions(Unit& unit, Cell& destination)
{
	if(unit.speed < 10)
	{
		unit.action_queue.emplace_back(std::make_unique<ActionNOP>());
	}
	unit.action_queue.emplace_back(std::make_unique<ActionMove>(destination));
}

int CommandMoveNW::process() noexcept
{
	const auto& pos = unit.parent_cell->pos;
	if(pos.x == 0 || pos.y == 0)
		return 1;

	Cell& destination = unit.parent_cell->parent_array[pos.y - 1][pos.x - 1];
	if(!unit.can_move_to(destination))
		return 1;

	register_actions(unit, destination);
	return 0;
}

int CommandMoveN::process() noexcept
{
	const auto& pos = unit.parent_cell->pos;
	if(pos.y == 0)
		return 1;

	Cell& destination = unit.parent_cell->parent_array[pos.y - 1][pos.x];
	if(!unit.can_move_to(destination))
		return 1;

	register_actions(unit, destination);
	return 0;
}

int CommandMoveNE::process() noexcept
{
	const auto& pos = unit.parent_cell->pos;
	if(pos.x + 1 >= unit.parent_cell->parent_array[pos.y].size() || pos.y == 0)
		return 1;

	Cell& destination = unit.parent_cell->parent_array[pos.y - 1][pos.x + 1];
	if(!unit.can_move_to(destination))
		return 1;

	register_actions(unit, destination);
	return 0;
}

int CommandMoveSW::process() noexcept
{
	const auto& pos = unit.parent_cell->pos;
	if(pos.x == 0 || pos.y + 1 >= unit.parent_cell->parent_array.size())
		return 1;

	Cell& destination = unit.parent_cell->parent_array[pos.y + 1][pos.x - 1];
	if(!unit.can_move_to(destination))
		return 1;

	register_actions(unit, destination);
	return 0;
}

int CommandMoveS::process() noexcept
{
	const auto& pos = unit.parent_cell->pos;
	if(pos.y + 1 >= unit.parent_cell->parent_array.size())
		return 1;

	Cell& destination = unit.parent_cell->parent_array[pos.y + 1][pos.x];
	if(!unit.can_move_to(destination))
		return 1;

	register_actions(unit, destination);
	return 0;
}

int CommandMoveSE::process() noexcept
{
	const auto& pos = unit.parent_cell->pos;
	if(pos.x + 1 >= unit.parent_cell->parent_array[pos.y].size()
		|| pos.y + 1 >= unit.parent_cell->parent_array.size())
		return 1;

	Cell& destination = unit.parent_cell->parent_array[pos.y + 1][pos.x + 1];
	if(!unit.can_move_to(destination))
		return 1;

	register_actions(unit, destination);
	return 0;
}

int CommandMoveW::process() noexcept
{
	const auto& pos = unit.parent_cell->pos;
	if(pos.x == 0)
		return 1;

	Cell& destination = unit.parent_cell->parent_array[pos.y][pos.x - 1];
	if(!unit.can_move_to(destination))
		return 1;

	register_actions(unit, destination);
	return 0;
}

int CommandMoveE::process() noexcept
{
	const auto& pos = unit.parent_cell->pos;
	if(pos.x + 1 >= unit.parent_cell->parent_array[pos.y].size())
		return 1;

	Cell& destination = unit.parent_cell->parent_array[pos.y][pos.x + 1];
	if(!unit.can_move_to(destination))
		return 1;

	register_actions(unit, destination);
	return 0;
}
