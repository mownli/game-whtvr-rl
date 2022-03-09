#include "ai_bank.h"
#include "unit.h"
#include "actions.h"


decltype(AI_Bank::trees) AI_Bank::trees{};

std::shared_ptr<BehaviourTree::Node> AI_Bank::get_instance(AI_Type type) noexcept
{
	std::shared_ptr<BehaviourTree::Node> tmp = trees[Utilities::enum_to_val(type)].lock();
	if(!tmp)
	{
		switch(type)
		{
		case AI_Type::STD:
			tmp = std::make_shared<AI_STD>();
			trees[Utilities::enum_to_val(type)] = std::static_pointer_cast<AI_STD>(tmp);
			break;
		case AI_Type::PROJECTILE:
			tmp = std::make_shared<Projectile>();
			trees[Utilities::enum_to_val(type)] = std::static_pointer_cast<Projectile>(tmp);
			break;
		default:
			assert(("Not defined", false));
		}
	}

	return tmp;
}

BehaviourTree::Status AI_Bank::Projectile::LeafCanFly::tick(BehaviourTree::UserData* data) noexcept
{
	DEBUG("LeafCanFly");
	assert(data);
	auto data_real = static_cast<Data*>(data);

	assert(data_real->path);
	assert(data_real->iter);

	if(*data_real->iter == data_real->path->end())
	{
		DEBUG("Projectile stopped");
		return BehaviourTree::Status::FAILURE;
	}
	else
	{
		DEBUG("Projectile wants to move");
		return BehaviourTree::Status::SUCCESS;
	}
}

BehaviourTree::Status AI_Bank::Projectile::LeafGo::tick(BehaviourTree::UserData* data) noexcept
{
	DEBUG("LeafGo");
	assert(data);
	auto data_real = static_cast<Data*>(data);

	assert(data_real->path);
	assert(data_real->iter);
	assert(data_real->obj);

	assert(*data_real->iter != data_real->path->end());

	data_real->obj->action = std::make_unique<ActionMove>(**data_real->iter);
	++(*data_real->iter);

	return BehaviourTree::Status::SUCCESS;
}

BehaviourTree::Status AI_Bank::AI_STD::LeafAmIGood::tick(BehaviourTree::UserData* data) noexcept
{
	DEBUG("LeafAmIGood");
	assert(data);
	Unit* obj = static_cast<Data*>(data)->obj;

	assert(obj);

	if(obj->ctrl.get_position(obj->id) == target)
	{
		DEBUG("Reached destination!");
		return BehaviourTree::Status::SUCCESS;
	}
	else
	{
		DEBUG("Not in place yet");
		return BehaviourTree::Status::FAILURE;
	}
}

BehaviourTree::Status AI_Bank::AI_STD::LeafFindPath::tick(BehaviourTree::UserData* data) noexcept
{
	DEBUG("LeafFindPath");
	assert(data);
	Data* data_real = static_cast<Data*>(data);

	assert(data_real->obj);

	assert(data_real->obj->mobility);
	const auto path = data_real->obj->mobility->find_path(*data_real->obj, target);
	if(path.size() == 0)
	{
		DEBUG("Path NOT found");
		return BehaviourTree::Status::FAILURE;
	}
	else
	{
		DEBUG("Found path");
		*data_real->path = path;
		return BehaviourTree::Status::SUCCESS;
	}
}

BehaviourTree::Status AI_Bank::AI_STD::LeafGo::tick(BehaviourTree::UserData* data) noexcept
{
	DEBUG("LeafGo");
	assert(data);
	Data* data_real = static_cast<Data*>(data);

	assert(data_real->path);
	assert(data_real->obj);

	assert(data_real->path->size() > 0);
	data_real->obj->action = std::make_unique<ActionMove>(data_real->path->front());

	return BehaviourTree::Status::SUCCESS;
}
