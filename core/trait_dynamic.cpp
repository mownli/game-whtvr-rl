#include "trait_dynamic.h"

#include <cassert>


void TraitDynamic::add_listener(std::shared_ptr<Trait> trait, std::vector<std::weak_ptr<Trait> >& target) noexcept
{
#ifndef NDEBUG // Check if duplicate
	for(const auto& i : target)
	{
		if(auto spt = i.lock())
		{
			assert(trait.get() != spt.get());
		}
	}
#endif

	target.push_back(trait);
}

void TraitDynamic::delete_listener(Trait* trait, std::vector<std::weak_ptr<Trait> >& target) noexcept
{
	for(auto i = target.begin(); i != target.end(); ++i)
	{
		if(auto spt = i->lock())
		{
			if(spt.get() == trait)
			{
				target.erase(i);
				return;
			}
		}
	}
	assert(("Trying to delete a non-listener", false));
}

int TraitDynamic::step(Unit& obj) noexcept
{
	for(auto i = on_step_listeners.rbegin(); i != on_step_listeners.rend(); ++i)
	{
		if(auto spt = i->lock())
		{
			spt->step(obj);
		}
		else
		{
			// CAREFUL. Invalidates iterator, possibly
			on_step_listeners.erase(i.base());
			continue;
		}
	}
	return 0;
}

void TraitDynamic::add_listener_move(std::shared_ptr<Trait> trait) noexcept
{
	add_listener(trait, on_move_listeners);
}

void TraitDynamic::add_listener_step(std::shared_ptr<Trait> trait) noexcept
{
	add_listener(trait, on_step_listeners);
}

void TraitDynamic::delete_listener_move(Trait* trait) noexcept
{
	delete_listener(trait, on_move_listeners);
}

void TraitDynamic::delete_listener_step(Trait* trait) noexcept
{
	delete_listener(trait, on_step_listeners);
}

int TraitDynamic::on_move(Unit& obj) noexcept
{
	for(auto i = on_move_listeners.rbegin(); i != on_move_listeners.rend(); ++i)
	{
		if(auto spt = i->lock())
		{
			spt->step(obj);
		}
		else
		{
			// CAREFUL. Invalidates iterator, possibly
			on_move_listeners.erase(i.base());
			continue;
		}
	}
	return 0;
}

int TraitDynamic::on_collision(Unit& cause, Unit& target) noexcept
{
	for(decltype(on_collision_cb.size()) i = 0; i < on_collision_cb.size();)
	{
		if(!on_collision_cb[i])
		{
			on_collision_cb.erase(on_collision_cb.begin() + i);
		}
		else
		{
			if(on_collision_cb[i](cause, target))
			{
				on_collision_cb.erase(on_collision_cb.begin() + i);
			}
			else
				++i;
		}
	}

	return 0;
}
