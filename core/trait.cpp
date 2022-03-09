#include "trait.h"
#include "cell.h"
//#include "action_abstract.h"
#include "object.h"

#include <cassert>


//int TraitMobile::step() noexcept
//{
//	if(move_c == 0)
//	{
//		auto ret = move();
//		if(!ret)
//		{
//			// TODO: move return
//			return ret;
//		}
//		else if(singleshot)
//		{
//			// TODO: do something with speed after stop
//			speed_x = 0;
//			speed_y = 0;
//		}
//		else
//		{
//			prepare_next_move();
//		}
//	}
//	else
//	{
//		--move_c;
//		assert(move_c >= 0);
//	}

//	return 0;
//}

//int TraitMobile::move() noexcept
//{
//	Coordinates xy_rel_to = {};
//	if(speed_y > 0)
//		xy_rel_to.y = 1;
//	else if(speed_y < 0)
//		xy_rel_to.y = -1;

//	if(speed_x > 0)
//		xy_rel_to.x = 1;
//	else if(speed_x < 0)
//		xy_rel_to.x = -1;

//	return parent.parent_cell->move_object(xy_rel_to, parent);
//}




int TraitPlayer::step(Object& obj) noexcept
{
	// TODO: observers
	return 1;
}
