#include "core.h"
#include "level.h"
#include "trait_intelligent.h"
#include "unit.h"
#include "ai_bank.h"
#include "utilities.h"

#include <algorithm>
#include "dbg.h"


void Core::register_unit(std::unique_ptr<Unit> unit, const Coordinates xy) noexcept
{
	apply_unit(std::move(unit), xy);
}

void Core::apply_unit(std::unique_ptr<Unit> unit, const Coordinates& xy) noexcept
{
	auto iter_backup_i = iter - unit_table.begin();

	int id;
	if(free_ids.size() > 0)
	{
		id = free_ids.top();
		free_ids.pop();
	}
	else
	{
		id = unit_table.size();
	}

	unit->id = id;
	unit_table.emplace_back(std::move(unit));
	cell_unit_id_table[xy.y][xy.x].push_back(id);
	unit_position_table.push_back(xy);

	iter = unit_table.begin() + iter_backup_i;
}

void Core::delete_unit(int id) noexcept
{
	assert(static_cast<decltype(unit_table.size())>(id) < unit_table.size());

	free_ids.push(id);
	unit_table[id].reset();

	// Skip unit_position_table
	auto xy = unit_position_table[id];

	auto& t = cell_unit_id_table[xy.y][xy.x];
	auto it = std::find(t.begin(), t.end(), id);
	if(it == t.end())
		assert(("No unit in cell_unit_id_table", false));
	else
		t.erase(it);
}

Core::Core(size_t level_w, size_t level_h) noexcept :
	level(std::make_unique<Level>(level_w, level_h)),
	cell_unit_id_table(level_h, std::vector<std::vector<int>>(level_w, std::vector<int>()))
{
	free_ids.push(0);

	std::unique_ptr<Unit> unit;

	unit = std::make_unique<Unit>(*this);
	unit->mobility = std::make_unique<TraitMobile>();
	unit->intel = std::make_unique<TraitPlayer>();
	apply_unit(std::move(unit), {5, 5});

	unit = std::make_unique<Unit>(*this);
	unit->mobility = std::make_unique<TraitMobile>();
	unit->intel = std::make_unique<TraitIntelligent>(AI_Bank::get_instance(AI_Bank::AI_Type::STD));
	apply_unit(std::move(unit), {21, 3});

	unit = std::make_unique<Unit>(*this);
	unit->mobility = std::make_unique<TraitMobile>();
	unit->intel = std::make_unique<TraitIntelligent>(AI_Bank::get_instance(AI_Bank::AI_Type::STD));
	apply_unit(std::move(unit), {1, 7});

	iter = unit_table.begin();
}

//int Core::command(Command& command) noexcept
//{
//	if(!waiting_for_input)
//		return 1;

//	if(command.process())
//	{
//		DEBUG("COMMAND REJECTED");
//		return 1;
//	}
//	else
//	{
//		DEBUG("UNLOCKED");
//		waiting_for_input = false;
//		return 0;
//	}
//}

int Core::update() noexcept
{
	DEBUG("UPDATE");
//	static int c = 0;
//	if(c == 1000)
//	{
//		unit_table.clear();
//		unit_position_table.clear();
//		for(auto& j :  cell_unit_id_table)
//			for(auto& i : j)
//				i.clear();

//		return 0;
//	}
//	++c;

//	if(waiting_for_input)
//		return 1;

	for(; iter != unit_table.end(); ++iter)
	{
		auto unit = iter->get();
		// TODO: change the container
		if(unit)
		{
			DEBUG("Unit step, id=%d", unit->id);
			switch(unit->step())
			{
			case Unit::StepRetCode::OK:
				break;
			case Unit::StepRetCode::INT:
				DEBUG("LOCKED");
				waiting_for_input = true;
				return 1;
			case Unit::StepRetCode::DELETE:
				delete_unit(unit->id);
				break;
			}
		}
	}
	iter = unit_table.begin();
	return 0;
}

Coordinates Core::get_position(int id) const noexcept
{
	return unit_position_table[id];
}

int Core::move_object(int id, const Coordinates& xy) noexcept
{
	Coordinates current_pos = get_position(id);
	if(current_pos == xy)
		return 0;

	auto& current_cell = cell_unit_id_table[current_pos.y][current_pos.x];
	auto& destination_cell = cell_unit_id_table[xy.y][xy.x];

	auto iter = std::find(current_cell.begin(), current_cell.end(), id);
	assert(iter != current_cell.end());

	current_cell.erase(iter);
	destination_cell.push_back(id);
	unit_position_table[id] = xy;

	return 0;
}

Cell::CellType Core::get_cell_type(const Coordinates& xy) const noexcept
{
	return level->cells[xy.y][xy.x].type;
}

std::vector<int> Core::get_cell_object_ids(const Coordinates& xy) const noexcept
{
//	const auto& ids = level->cells[xy.y][xy.x].object_ids;
//	std::vector<Object*> ret(ids.size());
//	for(auto i : ids)
//		ret.push_back(level->units[i].get());

	return cell_unit_id_table[xy.y][xy.x];
}

Unit* Core::get_object_by_id(int id) const noexcept
{
	return unit_table[id].get();
}

std::vector<Coordinates> Core::find_path(
	const Coordinates& from,
	const Coordinates& to,
	//move_heuristic_t heur,
	weight_pr_t weight_pr) const noexcept
{
	//return level->find_path(from, to, heur, weight_pr);
	return level->find_path(
		from,
		to,
		[weight_pr, this](size_t current, size_t next) noexcept -> int
		{
			return weight_pr(
				level->vertex_to_xy(current),
				level->vertex_to_xy(next));
		}
	);
}
