#include "unit.h"

#include <cassert>


Unit::StepRetCode Unit::step() noexcept
{
	if(hp <= 0)
		return StepRetCode::DELETE;
	else
	{
		StepRetCode ret = StepRetCode::OK;

		if(dynamics)
			dynamics->step(*this);

		if(intel)
		{
			if(intel->step(*this))
				ret = StepRetCode::INT;
		}

		if(action)
		{
			switch(action->exec(*this))
			{
			case 0:
				action.reset();
				break;
			case 1:
				++energy;
				break;
			default:
				assert(("Not implemented yet", false));
			}
		}

		return ret;
	}
}

void Unit::on_move() noexcept
{
	if(dynamics)
	{
		dynamics->on_move(*this);
	}
}

