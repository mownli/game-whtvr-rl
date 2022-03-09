#include "behaviour_tree.h"


BehaviourTree::Status BehaviourTree::Sequence::tick(UserData* userdata)
{
	for(auto& i : children)
	{
		switch(i->tick(userdata))
		{
		  case Status::RUNNING:
			return Status::RUNNING;
			break;
		  case Status::FAILURE:
			return Status::FAILURE;
			break;
		  case Status::SUCCESS:
			break;
		}
	}
	return Status::SUCCESS;
}

BehaviourTree::Status BehaviourTree::SequenceWithMemory::tick(UserData* userdata)
{
	for(; iter != children.cend(); ++iter)
	{
		switch(iter->get()->tick(userdata))
		{
		  case Status::RUNNING:
			return Status::RUNNING;
			break;
		  case Status::FAILURE:
			reset_iterator();
			return Status::FAILURE;
			break;
		  case Status::SUCCESS:
			break;
		}
	}
	reset_iterator();
	return Status::SUCCESS;
}

BehaviourTree::Status BehaviourTree::Fallback::tick(UserData* userdata)
{
	for(auto& i : children)
	{
		switch(i->tick(userdata))
		{
		  case Status::RUNNING:
			return Status::RUNNING;
			break;
		  case Status::SUCCESS:
			return Status::SUCCESS;
			break;
		  case Status::FAILURE:
			break;
		}
	}
	return Status::FAILURE;
}

BehaviourTree::Status BehaviourTree::FallbackWithMemory::tick(UserData* userdata)
{
	for(; iter != children.cend(); ++iter)
	{
		switch(iter->get()->tick(userdata))
		{
		  case Status::RUNNING:
			return Status::RUNNING;
			break;
		  case Status::SUCCESS:
			reset_iterator();
			return Status::SUCCESS;
			break;
		  case Status::FAILURE:
			break;
		}
	}
	reset_iterator();
	return Status::FAILURE;
}

BehaviourTree::Status BehaviourTree::Parallel::tick(UserData* userdata)
{
	size_t succesful = 0;
	size_t failed = 0;
	for(auto& i : children)
	{
		switch(i->tick(userdata))
		{
		  case Status::FAILURE:
			++failed;
			break;
		  case Status::SUCCESS:
			++succesful;
			break;
		  case Status::RUNNING:
			break;
		}
	}

	if(succesful >= threshold)
	{
		return Status::SUCCESS;
	}
	else if(failed > children.size() - threshold)
	{
		return Status::FAILURE;
	}

	return Status::RUNNING;
}

BehaviourTree::Status BehaviourTree::Inverter::tick(UserData* userdata)
{
	switch(child.get()->tick(userdata))
	{
	  case Status::FAILURE:
		return Status::SUCCESS;
		break;
	  case Status::SUCCESS:
		return Status::FAILURE;
		break;
	  case Status::RUNNING:
		return Status::RUNNING;
		break;
	}
}
