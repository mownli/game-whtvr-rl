#ifndef UNIT_H
#define UNIT_H

#include "object_controller.h"
#include "trait_mobile.h"
#include "trait_dynamic.h"
#include "trait_player.h"
#include "action_abstract.h"
#include "utilities.h"

#include <memory>
#include <bitset>


class Unit
{
public:
	enum class StepRetCode
	{
		OK,
		INT,
		DELETE
	};

	enum class Property
	{
		CELL_EXCL,
		PROPERTIES_N
	};
	//std::bitset<static_cast<std::underlying_type_t<Property>>(Property::PROPERTIES_N)> properties;
	std::bitset<Utilities::enum_to_val(Property::PROPERTIES_N)> properties;

	ObjectController& ctrl;
	int id = -1;
	int speed = 5;
	int energy = 0;
	int hp = 100;
	std::unique_ptr<Action> action;

	std::shared_ptr<TraitMobile> mobility;
	std::shared_ptr<TraitSight> sight;
	std::unique_ptr<TraitDynamic> dynamics;
	std::shared_ptr<Trait> intel;

	Unit(ObjectController& ctrl) noexcept : ctrl(ctrl) {}

	StepRetCode step() noexcept;

	void on_move() noexcept;
};

#endif // UNIT_H
