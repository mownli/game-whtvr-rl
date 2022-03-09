#ifndef ACTIONS_H
#define ACTIONS_H

#include "action_abstract.h"
#include "unit.h"

#include "pooshka.h"


class ActionMove : public Action
{
public:
	const Coordinates target;

	ActionMove(const Coordinates& target) noexcept : target(target) {}

	int exec(Unit& obj) noexcept override
	{
		switch(obj.mobility->move(obj, target))
		{
		case TraitMobile::MoveRetCode::FAILURE:
		case TraitMobile::MoveRetCode::IMPOSSIBLE:
		case TraitMobile::MoveRetCode::OK:
			return 0;
		case TraitMobile::MoveRetCode::NOT_YET:
			return 1;
		}
	}
};


class ActionShoot : public Action
{
public:
	const Coordinates target;

	ActionShoot(const Coordinates& target_xy) noexcept : target(target_xy) {}

	int exec(Unit& obj) noexcept override
	{
		Pooshka p;
		p.shoot(&obj, target);

		return 0;
	}
};



#endif // ACTIONS_H
