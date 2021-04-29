#include "ui.h"

#include "dbg.h"


//void UI::ScrollableScene::scroll(UI::ScrollableScene::Direction dir, int amount, int vel_inc)
void UI::ScrollableScene::scroll(int x, int y) noexcept
{
	//this->x += x;
	this->x_delta += x * abs(x_vel);
	//this->y += y;
	this->y_delta += y * abs(y_vel);
//	switch(dir)
//	{
//	case Direction::UP:
//		y -= amount;
//		y_delta -= amount;
//		break;
//	case Direction::DOWN:
//		y += amount;
//		y_delta += amount;
//		break;
//	case Direction::LEFT:
//		x -= amount;
//		x_delta -= amount;
//		break;
//	case Direction::RIGHT:
//		x += amount;
//		x_delta += amount;
//		break;
//	}
}


UI::UI(UI::Options* opts) :
	sdl(EngineSDL(opts->win_title, opts->win_w, opts->win_h)),
	params(*opts),
	canvas(opts->win_w, opts->win_h, sdl.loadTileset(params.tileset_path, params.tile_w, params.tile_h))
	//canvas(sdl, opts->win_w, opts->win_h, sdl.loadTileset(params.tileset_path, params.tile_w, params.tile_h))
{

}

//void UI::loadTileset(const std::string& tileset_path, int tile_w, int tile_h)
//{
//	//sdl.loadTileset(tileset_path, tile_w, tile_h)
//	tileset = std::make_unique<Tileset>(sdl.loadTileset(tileset_path, tile_w, tile_h), tile_w, tile_h);
//}

//UI::Scene UI::createScene(int upper_left_x_, int upper_left_y_, int w_, int h_)
//{
//	return Scene(sdl, upper_left_x_, upper_left_y_, w_, h_);
//}

//UI::ScrollableScene UI::createScrollableScene(int upper_left_x_, int upper_left_y_, int w_, int h_)
//{
//	return ScrollableScene(sdl, upper_left_x_, upper_left_y_, w_, h_);
//}


void UI::regKeyEventCallback(SDL_Scancode scancode, std::function<void(bool)> call)
{
	//	callbacks.erase(callbacks.find(scancode));
	//	callbacks.insert(std::make_pair(scancode, call));
	//callbacks.insert_or_assign(scancode, call);
	callbacks[scancode] = call;
}

int UI::eventLoop()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if(!callbacks.empty())
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
				callbacks[event.key.keysym.scancode](true);
				break;
			case SDL_KEYUP:
				callbacks[event.key.keysym.scancode](false);
				break;
			default:
				break;
			}
		}

		if(event.type == SDL_QUIT)
		{
			return 1;
		}
	}
	return 0;
}

void UI::drawTile(SceneAbstract& scene, const EngineSDL::Texture& tx, int x, int y) noexcept
{
	scene.drawTile(tx, x, y);
}

UI::SceneAbstract* UI::SceneAbstract::addScene(std::unique_ptr<SceneAbstract> scene, int x, int y) noexcept
{
	subscenes.push_back(std::move(scene));
	UI::SceneAbstract& s = *subscenes.back();
	s.x = x;
	s.y = y;
	s.parent = this;
	return &s;
}


UI::Scene::Scene(int w_, int h_) noexcept
{
	w = w_;
	h = h_;
}

void UI::Scene::drawTile(const EngineSDL::Texture& tx, int x, int y) noexcept
{
	assert(parent != nullptr);
	if(x >= 0 && x < w && y >= 0 && y < h)
		parent->drawTile(tx, this->x + x, this->y + y);
}


UI::ScrollableScene::ScrollableScene(int w, int h) noexcept :
	Scene(w, h)
{

}


//UI::Canvas::Canvas(EngineSDL& sdl, int w, int h, EngineSDL::Tileset ts) noexcept :
UI::Canvas::Canvas(int w, int h, EngineSDL::Tileset ts) noexcept :
	//sdl(sdl),
	tileset(std::move(ts))
{
	this->w = w;
	this->h = h;
}

void UI::Canvas::drawTile(const EngineSDL::Texture& tx, int x, int y) noexcept
{
	tx.render(tileset.tile_w * x, tileset.tile_h * y);
}
