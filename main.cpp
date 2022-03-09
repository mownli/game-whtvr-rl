#include "ui/ui.h"
#include "core/core.h"
#include "core/coordinates.h"
#include "core/cell.h"
#include "core/level.h"
#include "core/unit.h"
//#include "core/trait_player.h"

#include <iostream>
#include <memory>
#include <cassert>

#include "core/dbg.h"


class Glue
{
	std::unique_ptr<Core> core;
	std::unique_ptr<UI> ui;
	Coordinates xy_delta; // screen to the left -> delta--
	UI::Scene* first;
	UI::Scene* second;
	bool quit_flag = false;

public:
	Glue()
	{
		const int win_w = 640;
		const int win_h = 480;
		const int tile_w = 8;
		const int tile_h = 12;

		UI::Options ui_opts
		{
			.win_title = "kkk",
			.win_w = win_w,
			.win_h = win_h,
		};

		xy_delta = {};
		ui = std::make_unique<UI>(&ui_opts);
		ui->loadTileset(
			"/home/gyro/docs/prog/projects/kkk/curses_640x300.bmp",
			tile_w,
			tile_h,
			{ 255, 0, 255, 0 });

		core = std::make_unique<Core>(100, 100);

		SDL_Rect rect1 = {
			.x = 0,
			.y = 0,
			.w = win_w / 2,
			.h = win_h / 2,
		};
		SDL_Rect rect2 = {
			.x = win_w / 2,
			.y = 0,
			.w = win_w / 2,
			.h = win_h / 2,
		};

		first = static_cast<UI::Scene*>(ui->canvas.addScene(rect1, tile_w, tile_h));
		second = static_cast<UI::Scene*>(ui->canvas.addScene(rect2, tile_w, tile_h));

		setup_keys();
	}

	Coordinates convert_xy(Coordinates coords)
	{
		int x = xy_delta.x + coords.x;
		int y = xy_delta.y + coords.y;
		return { .x = x, .y = y };
	}

	EngineSDL::Texture& getTxNum(const Coordinates& cell_xy)
	{
		auto type = core->get_cell_type(cell_xy);
		if(type == Cell::CellType::INVALID)
			return ui->tilesets.front()->operator[](0);

		auto& cell_objects = core->cell_unit_id_table[cell_xy.y][cell_xy.x];
		if(!cell_objects.empty())
		{
			auto unit_id = (cell_objects.front());
			if(unit_id == 0)
				return ui->tilesets.front()->operator[](64);
			else
				return ui->tilesets.front()->operator[](1);
//			switch(unit->type)
//			{
//			case 5:
//				return ui->tilesets.front()->operator[](1);
//			default:
//				return ui->tilesets.front()->operator[](50);
//			}
		}

		switch(type)
		{
		case Cell::CellType::SIMPLE:
			return ui->tilesets.front()->operator[](46);
		case Cell::CellType::WALL:
			return ui->tilesets.front()->operator[](35);
		default:
			return ui->tilesets.front()->operator[](15);
		}
	}

	void drawCells()
	{
		auto isDrawableX = [&](int x)
		{
			//return x < screen_lr_xy.x && x < core->level->getWidth();
			return x < core->level->getWidth() && x >= 0;
		};
		auto isDrawableY = [&](int y)
		{
			//return y < screen_lr_xy.y && y < core->level->getHeight();
			return y < core->level->getHeight() && y >= 0;
		};

		ui->setTarget(first);
		ui->drawTile({.g = 255}, 0, 15);
		first->drawTile(ui->tilesets.front()->operator[](1), 0, 15);

		for(int y = 0, y_core = y + first->y_delta_tiles; y < first->getHeightInTiles(); ++y, ++y_core)
		{
			if(!isDrawableY(y_core)) continue;
			for(int x = 0, x_core = x + first->x_delta_tiles; x < first->getWidthInTiles(); ++x, ++x_core)
			{
				if(!isDrawableX(x_core)) continue;
				first->drawTile(getTxNum({x_core, y_core}), x, y);
			}
		}
		ui->setTarget(second);
		for(int y = 0, y_core = y + second->y_delta_tiles; y < second->getHeightInTiles(); ++y, ++y_core)
		{
			if(!isDrawableY(y_core)) continue;
			for(int x = 0, x_core = x + second->x_delta_tiles; x < second->getWidthInTiles(); ++x, ++x_core)
			{
				//DEBUG("%d, %d, %d, %d", second->x, second->y, second->w, second->h);
				if(!isDrawableX(x_core)) continue;
				second->drawTile(getTxNum({x_core, y_core}), x, y);
			}
		}
	}

	int exec()
	{
		while(!quit_flag)
		{
			if(core->update())
			{
				// TODO: request processing
				DEBUG("WAITING FOR INPUT");
			}
			if(ui->eventLoop())	break;
			ui->clear();
			drawCells();
			ui->update();
			SDL_Delay(0);
		}
		return 0;
	}

	void setup_keys()
	{
		// UNIT MOVEMENT
//		ui->regKeyEventCallback(SDL_SCANCODE_KP_7, [&](bool down)
//		{
//			if(!down)
//				return;
//			CommandMoveNW op(*core->level->units.front().get());
//			core->command(op);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_KP_8, [&](bool down)
//		{
//			if(!down)
//				return;
//			CommandMoveN op(*core->level->units.front().get());
//			core->command(op);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_KP_9, [&](bool down)
//		{
//			if(!down)
//				return;
//			CommandMoveNE op(*core->level->units.front().get());
//			core->command(op);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_KP_1, [&](bool down)
//		{
//			if(!down)
//				return;
//			CommandMoveSW op(*core->level->units.front().get());
//			core->command(op);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_KP_2, [&](bool down)
//		{
//			if(!down)
//				return;
//			CommandMoveS op(*core->level->units.front().get());
//			core->command(op);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_KP_3, [&](bool down)
//		{
//			if(!down)
//				return;
//			CommandMoveSE op(*core->level->units.front().get());
//			core->command(op);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_KP_4, [&](bool down)
//		{
//			if(!down)
//				return;
//			CommandMoveW op(*core->level->units.front().get());
//			core->command(op);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_KP_6, [&](bool down)
//		{
//			if(!down)
//				return;
//			CommandMoveE op(*core->level->units.front().get());
//			core->command(op);
//		});





		////////////////////////////////////////////


		ui->regKeyEventCallback(SDL_SCANCODE_KP_7, [&](bool down)
		{
			if(!down)
				return;
			auto xy = core->get_position(0);
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->move_command(*obj, xy.operator+({-1, -1}));
		});
		ui->regKeyEventCallback(SDL_SCANCODE_KP_8, [&](bool down)
		{
			if(!down)
				return;
			auto xy = core->get_position(0);
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->move_command(*obj, xy.operator+({0, -1}));
		});
		ui->regKeyEventCallback(SDL_SCANCODE_KP_9, [&](bool down)
		{
			if(!down)
				return;
			auto xy = core->get_position(0);
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->move_command(*obj, xy.operator+({1, -1}));
		});
		ui->regKeyEventCallback(SDL_SCANCODE_KP_6, [&](bool down)
		{
			if(!down)
				return;
			auto xy = core->get_position(0);
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->move_command(*obj, xy.operator+({1, 0}));
		});
		ui->regKeyEventCallback(SDL_SCANCODE_KP_3, [&](bool down)
		{
			if(!down)
				return;
			auto xy = core->get_position(0);
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->move_command(*obj, xy.operator+({1, 1}));
		});
		ui->regKeyEventCallback(SDL_SCANCODE_KP_2, [&](bool down)
		{
			if(!down)
				return;
			auto xy = core->get_position(0);
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->move_command(*obj, xy.operator+({0, 1}));
		});
		ui->regKeyEventCallback(SDL_SCANCODE_KP_1, [&](bool down)
		{
			if(!down)
				return;
			auto xy = core->get_position(0);
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->move_command(*obj, xy.operator+({-1, 1}));
		});
		ui->regKeyEventCallback(SDL_SCANCODE_KP_4, [&](bool down)
		{
			if(!down)
				return;
			auto xy = core->get_position(0);
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->move_command(*obj, xy.operator+({-1, 0}));
		});
		ui->regKeyEventCallback(SDL_SCANCODE_KP_5, [&](bool down)
		{
			if(!down)
				return;
			++core->iter;
		});

		///////////////////////////////////////////////////////////////////////////////////////////////

		ui->regKeyEventCallback(SDL_SCANCODE_Q, [&](bool down)
		{
			if(!down)
				return;
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->shoot_command(*obj, {0, 0});
		});
		ui->regKeyEventCallback(SDL_SCANCODE_W, [&](bool down)
		{
			if(!down)
				return;
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->shoot_command(*obj, {5, 0});
		});
		ui->regKeyEventCallback(SDL_SCANCODE_E, [&](bool down)
		{
			if(!down)
				return;
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->shoot_command(*obj, {10, 0});
		});
		ui->regKeyEventCallback(SDL_SCANCODE_D, [&](bool down)
		{
			if(!down)
				return;
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->shoot_command(*obj, {10, 5});
		});
		ui->regKeyEventCallback(SDL_SCANCODE_C, [&](bool down)
		{
			if(!down)
				return;
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->shoot_command(*obj, {10, 10});
		});
		ui->regKeyEventCallback(SDL_SCANCODE_X, [&](bool down)
		{
			if(!down)
				return;
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->shoot_command(*obj, {5, 10});
		});
		ui->regKeyEventCallback(SDL_SCANCODE_Z, [&](bool down)
		{
			if(!down)
				return;
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->shoot_command(*obj, {0, 10});
		});
		ui->regKeyEventCallback(SDL_SCANCODE_A, [&](bool down)
		{
			if(!down)
				return;
			auto obj = core->get_object_by_id(0);
			static_cast<TraitPlayer*>(obj->intel.get())->shoot_command(*obj, {0, 5});
		});










//		ui->regKeyEventCallback(SDL_SCANCODE_LEFT, [&](bool down)
//		{
//			first->x_vel = -down;
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_RIGHT, [&](bool down)
//		{
//			first->x_vel = down;
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_UP, [&](bool down)
//		{
//			first->y_vel = -down;
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_DOWN, [&](bool down)
//		{
//			first->y_vel = down;
//		});

//		ui->regKeyEventCallback(SDL_SCANCODE_R, [&](bool down)
//		{
//			if(!down)
//				first->resetPosition();
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_Q, [&](bool)
//		{
//			quit_flag = true;
//		});

//		ui->regKeyEventCallback(SDL_SCANCODE_Z, [&](bool)
//		{
//			ui->tilesets.front()->operator[](0).setColor(255, 0, 0);

//			int w, h;
//			SDL_QueryTexture(ui->tilesets.front()->operator[](1).get(), nullptr, nullptr, &w, &h);
//			DEBUG("WWWWHHH  %d, %d", w, h);
//			//SDL_SetTextureAlphaMod(ui->tilesets.front()->operator[](1).get(), 255);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_X, [&](bool)
//		{
//			ui->tilesets.front()->operator[](1).setColor(0, 255, 0);
//		});
//		ui->regKeyEventCallback(SDL_SCANCODE_C, [&](bool)
//		{
//			ui->tilesets.front()->operator[](1).setColor(0, 0, 255);

//		});
	}
};


int main(int argc, char **argv)
{
	Glue glue;
	return glue.exec();
}

