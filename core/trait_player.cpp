#include "trait_player.h"
#include "unit.h"
#include "actions.h"

#include "dbg.h"


StateMachine_OO::AbstractState* TraitPlayer::StateMove::next(StateMachine_OO::AbstractContext* ctx)
{
	MoveContext* ctx_real = static_cast<MoveContext*>(ctx);
	assert(ctx_real->obj);

	switch(ctx_real->obj->mobility->move(*ctx_real->obj, ctx_real->xy))
	{
	case TraitMobile::MoveRetCode::FAILURE:
	case TraitMobile::MoveRetCode::IMPOSSIBLE:
	case TraitMobile::MoveRetCode::OK:
		return createStateObj<StateIdle>();
	case TraitMobile::MoveRetCode::NOT_YET:
		++ctx_real->obj->energy;
		return this;
	}
}

int TraitPlayer::step(Unit& obj) noexcept
{
//	fsm.transition(&move_ctx);
//	auto ret = fsm.status();
//	DEBUG("Status: %d", ret);
//	return ret;

	if(obj.action)
		return 0;
	else
		return 1;
}

void TraitPlayer::move_command(Unit& obj, Coordinates xy) noexcept
{
//	if(fsm.status() == 0)
//		return;
//	move_ctx.obj = &obj;
//	move_ctx.xy = xy;
//	fsm.setState<StateMove>();

	if(obj.action)
	{
		return;
	}
	else
	{
//		// Skip a turn
//		if(obj.ctrl.get_position(obj.id) == xy)
//			return;

		obj.action = std::make_unique<ActionMove>(xy);
	}
}

void TraitPlayer::shoot_command(Unit& obj, Coordinates target_xy) noexcept
{
	if(obj.action)
	{
		return;
	}
	else
	{
		obj.action = std::make_unique<ActionShoot>(target_xy);
	}
}
