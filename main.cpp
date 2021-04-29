#include <iostream>
#include <memory>
#include <stack>
#include <cassert>

#include "ui/ui.h"
#include "core/core.h"
#include "dbg.h"


class Glue
{
	std::unique_ptr<Core> core;
	std::unique_ptr<UI> ui;
	Core::XY xy_delta; // screen to the left -> delta--
	UI::ScrollableScene* first;
	UI::Scene* second;

public:
	Core::XY convert_xy(Core::XY coords)
	{
		int x = xy_delta.x + coords.x;
		int y = xy_delta.y + coords.y;
		return { .x = x, .y = y };
	}

	Glue()
	{
		UI::Options ui_opts
		{
			.win_title = "kkk",
			.win_w = 640,
			.win_h = 480,
			.tileset_path = "/home/gyro/kkk/curses_640x300.bmp",
			.tile_w = 8,
			.tile_h = 12
		};

		xy_delta = {};
		ui = std::make_unique<UI>(&ui_opts);
		core = std::make_unique<Core>(100, 100);
		core->level->init();

		int wc = ui->canvas.getWidthInTiles()/2;
		int hc = ui->canvas.getHeightInTiles()/2;
		first = static_cast<UI::ScrollableScene*>(ui->canvas.addScene(std::make_unique<UI::ScrollableScene>(wc, hc), 0, 0));
		second = static_cast<UI::Scene*>(ui->canvas.addScene(std::make_unique<UI::Scene>(wc, hc), wc, 0));

		ui->regKeyEventCallback(SDL_SCANCODE_LEFT, [&](bool down)
		{
			//xy_delta.x--;
			if(!down)
				first->x_vel = 0;
			else
			{
				first->x_vel++;
				first->scroll(-1, 0);
			}

		});
		ui->regKeyEventCallback(SDL_SCANCODE_RIGHT, [&](bool down)
		{
			//xy_delta.x++;
			if(!down)
				first->x_vel = 0;
			else
			{
				first->x_vel++;
				first->scroll(1, 0);
			}
		});
		ui->regKeyEventCallback(SDL_SCANCODE_UP, [&](bool down)
		{
			//xy_delta.y--;
			if(!down)
				first->y_vel = 0;
			else
			{
				first->y_vel++;
				first->scroll(0, -1);
			}
		});
		ui->regKeyEventCallback(SDL_SCANCODE_DOWN, [&](bool down)
		{
			//xy_delta.y++;
			if(!down)
				first->y_vel = 0;
			else
			{
				first->y_vel++;
				first->scroll(0, 1);
			}
		});
	}

	EngineSDL::Texture& getTxNum(const Core::Cell* cell)
	{
		if(cell == nullptr)
			return ui->canvas.tileset[0];

		switch(cell->type)
		{
		case 0:
			return ui->canvas.tileset[35];
		default:
			return ui->canvas.tileset[46];
		}
	}

	void drawCells()
	{
		//DEBUG("%d, %d, %d, %d", ui->canvas.x, ui->canvas.y, ui->canvas.w, ui->canvas.h);
//		Core::XY screen_ul_xy = scene2core(ui->scene.ul);


//		auto isOriginOutOfBoundsX = [&]()
//		{
//			return screen_ul_xy.x < 0 || abs(screen_ul_xy.x) >= core->level->getWidth();
//		};
//		auto isOriginOutOfBoundsY = [&]()
//		{
//			return screen_ul_xy.y < 0 || abs(screen_ul_xy.y) >= core->level->getHeight();
//		};

		auto isDrawableX = [&](int x)
		{
			//return x < screen_lr_xy.x && x < core->level->getWidth();
			return x < core->level->w && x >= 0;
		};
		auto isDrawableY = [&](int y)
		{
			//return y < screen_lr_xy.y && y < core->level->getHeight();
			return y < core->level->h && y >= 0;
		};

		//if(ui->scene.w + xy_delta.x <= 0)

//		for(int y = 0, y_core = y + xy_delta.y; (y < ui->canvas.h); y++, y_core++)
//		{
//			if(!isDrawableY(y_core)) continue;
//			for(int x = 0, x_core = x + xy_delta.x; (x < ui->canvas.w); x++, x_core++)
//			{
//				if(!isDrawableX(x_core)) continue;
//				const std::unique_ptr<Core::Cell>& cell = core->level->cells[y_core][x_core];
//				if(cell == nullptr) continue;
//				ui->canvas.renderTx(&getTxNum(cell->type), x, y);
//			}
//		}

//		auto first = static_cast<UI::Scene&>(ui->canvas.addScene(std::make_unique<UI::Scene>(ui->canvas.w/2, ui->canvas.h/2), 0, 0));
//		auto second = static_cast<UI::Scene&>(ui->canvas.addScene(std::make_unique<UI::Scene>(ui->canvas.w/2, ui->canvas.h/2), ui->canvas.w/2, ui->canvas.h/2));


//		ui->canvas.addScene(std::make_unique<UI::Scene>(ui->canvas.w/2, ui->canvas.h/2), 0, 0);
//		ui->canvas.addScene(std::make_unique<UI::Scene>(ui->canvas.w/2, ui->canvas.h/2), ui->canvas.w/2, ui->canvas.h/2);

		for(int y = 0, y_core = y + first->y_delta; (y < first->h); y++, y_core++)
		{
			if(!isDrawableY(y_core)) continue;
			for(int x = 0, x_core = x + first->x_delta; (x < first->w); x++, x_core++)
			{
				if(!isDrawableX(x_core)) continue;
				first->drawTile(getTxNum(core->level->cells[y_core][x_core].get()), x, y);
			}
		}
		for(int y = 0, y_core = y + xy_delta.y; (y < second->h); y++, y_core++)
		{
			if(!isDrawableY(y_core)) continue;
			for(int x = 0, x_core = x + xy_delta.x; (x < second->w); x++, x_core++)
			{
				//DEBUG("%d, %d, %d, %d", second->x, second->y, second->w, second->h);
				if(!isDrawableX(x_core)) continue;
				second->drawTile(getTxNum(core->level->cells[y_core][x_core].get()), x, y);
			}
		}

//		if(!(isOriginOutOfBoundsX() || isOriginOutOfBoundsY()))
//		{
//			for(int y = screen_ul_xy.y; isDrawableY(y); y++)
//			{
//				for(int x = screen_ul_xy.x; isDrawableX(x); x++)
//				{
//					const std::unique_ptr<Core::Cell>& cell = core->level->cells[y][x];
//					if(cell == nullptr) continue;
//					ui->renderTexture(ui->scene, &getTxNum(cell->type), core2scene({x, y}));
//				}
//			}
//		}
	}

	int main_loop()
	{
		for(;;)
		{
			core->update();
			if(ui->eventLoop())	break;
			ui->clear();
			drawCells();
			ui->update();
		}
		return 0;
	}
};


int main(int argc, char **argv)
{
	Glue glue;
	return glue.main_loop();
}

