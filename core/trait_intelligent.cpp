#include "trait_intelligent.h"
#include "unit.h"
#include "ai_bank.h"

#include "dbg.h"


int TraitIntelligent::step(Unit& obj) noexcept
{
	if(!obj.action)
	{
		AI_Bank::Data data;
		data.obj = &obj;
		data.path = &cache;
		data.iter = &iter;

		//ai->tree_root.tick(&data);

		assert(ai);
		ai->tick(&data);
	}

	return 0;


	//		if(cache.size() == 0)
	//		{
	//			DEBUG("Empty cache");
	//			cache = obj.mobility->find_path(obj, {8, 4});
	//			DEBUG("Path:");
	//			for(auto& i : cache)
	//			{
	//				DEBUG("%d, %d", i.x, i.y);
	//			}
	//			DEBUG("End path");
	//			iter = cache.cbegin();
	//		}

	//		DEBUG("Energy: %d", obj.energy);

	//		if(iter != cache.cend())
	//		{
	//			switch(obj.mobility->move(obj, *iter))
	//			{
	//			case TraitMobile::MoveRetCode::FAILURE:
	//				DEBUG("Move FAILURE");
	//			case TraitMobile::MoveRetCode::IMPOSSIBLE:
	//				DEBUG("Move IMPOSSIBLE");
	//				return 1;
	//			case TraitMobile::MoveRetCode::NOT_YET:
	//				DEBUG("Move NOTYET");
	//				++obj.energy;
	//				return 0;
	//			case TraitMobile::MoveRetCode::OK:
	//				DEBUG("Move OK");
	//				++iter;
	//				return 0;
	//			}
	//		}

	//		return 0;
}
