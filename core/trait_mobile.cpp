#include "trait_mobile.h"
#include "unit.h"

#include "dbg.h"


std::vector<Coordinates> TraitMobile::find_path(const Unit& obj, const Coordinates& xy) const noexcept
{
	return obj.ctrl.find_path(
		obj.ctrl.get_position(obj.id),
		xy,
		[&obj](const Coordinates& current, const Coordinates& next) noexcept -> int
		{
			switch(obj.ctrl.get_cell_type(next))
			{
			case Cell::CellType::INVALID:
			case Cell::CellType::WALL:
				return -1;
				break;
			default:
				// TODO: different terrain weights
				return 0;
			}
		});
}

TraitMobile::MoveRetCode TraitMobile::move(Unit& obj, const Coordinates& xy) const noexcept
{
	const int cost = get_move_cost(obj, xy);
	DEBUG("Move cost to (%d, %d): %d", xy.x, xy.y, cost);
	if(cost < 0)
	{
		obj.energy = 0;
		DEBUG("Move IMPOSSIBLE");
		return IMPOSSIBLE;
	}
	else if(obj.energy < cost)
	{
		DEBUG("Move NOT_YET");
		return NOT_YET;
	}
	else
	{
		obj.energy = 0;

		if(obj.properties[Utilities::enum_to_val(Unit::Property::CELL_EXCL)])
		{
			auto object_ids = obj.ctrl.get_cell_object_ids(xy);
			for(const auto id : object_ids)
			{
				auto object_i = obj.ctrl.get_object_by_id(id);
				if(object_i->properties[Utilities::enum_to_val(Unit::Property::CELL_EXCL)])
				{
					obj.dynamics->on_collision(obj, *object_i);

					// Check if the unit still exists
					//if(object_i)
					object_i->dynamics->on_collision(*object_i, obj);
				}
			}
		}

		if(obj.ctrl.move_object(obj.id, xy))
		{
			DEBUG("Move FAILURE");
			return FAILURE;
		}
		else
		{
			obj.on_move();
			DEBUG("Move OK");
			return OK;
		}
	}
}

int TraitMobile::get_move_cost(const Unit& obj, const Coordinates& xy) const noexcept
{
	auto temp = obj.ctrl.get_position(obj.id) - xy;
	if(!temp.is_unit())
	{
		DEBUG("The direction vector is not a unit vector");
		return -1;
	}
	else
	{
		DEBUG("Next cell is of type: %d", Utilities::enum_to_val(obj.ctrl.get_cell_type(xy)));
		switch(obj.ctrl.get_cell_type(xy))
		{
		case Cell::CellType::INVALID:
		case Cell::CellType::WALL:
			return -1;
			break;
		default:
			return obj.speed;
		}
	}
}
