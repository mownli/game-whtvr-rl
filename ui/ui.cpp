#include "ui.h"


void UI::Scene::scroll(int x, int y) noexcept
{
	this->x_delta_tiles += x * abs(x_vel);
	this->y_delta_tiles += y * abs(y_vel);
}

void UI::Scene::updateRecursive() noexcept
{
	if(active)
		scroll(x_vel, y_vel);
	for(Scene& scene : subscenes)
	{
		scene.updateRecursive();
	}
}


UI::UI(UI::Options* opts) :
	sdl(EngineSDL(opts->win_title, opts->win_w, opts->win_h)),
	params(*opts),
	canvas(&sdl, { .w = opts->win_w, .h = opts->win_h }, 1, 1)
{
}

void UI::loadTileset(const std::string& tileset_path, int tile_w, int tile_h, const SDL_Color& color_key)
{
	auto ptr = std::make_unique<EngineSDL::Tileset>(sdl.loadTileset(tileset_path, tile_w, tile_h, color_key));
	if(!ptr)
		throw std::runtime_error("Couldn't load tileset");
	tilesets.push_back(std::move(ptr));
}

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
		switch(event.type)
		{
		case SDL_KEYDOWN:
			if(callbacks[event.key.keysym.scancode])
				callbacks[event.key.keysym.scancode](true);
			break;
		case SDL_KEYUP:
			if(callbacks[event.key.keysym.scancode])
				callbacks[event.key.keysym.scancode](false);
			break;
		case SDL_QUIT:
			return 1;
			break;
		default:
			break;
		}
		updateScenes();
	}
	return 0;
}

int UI::drawTile(const EngineSDL::Texture* tx, int x_tiles, int y_tiles) noexcept
{
	return target->drawTile(tx, x_tiles, y_tiles);
}

void UI::drawTile(const SDL_Color& c, int x_tiles, int y_tiles) noexcept
{
	sdl.renderRect(
		x_tiles * target->tile_w,
		y_tiles * target->tile_h,
		target->tile_w,
		target->tile_h,
		c);
}

void UI::updateScenes() noexcept
{
	canvas.updateRecursive();
}

void UI::setTarget(UI::Scene* scene) noexcept
{
	if(!scene)
		target = &canvas;
	else
		target = scene;

	target->lockOn();
}

UI::Scene::Scene(EngineSDL* sdl, const SDL_Rect& rect, int tile_w, int tile_h) noexcept :
	viewport(sdl, rect),
	tile_w(tile_w),
	tile_h(tile_h)
{
	assert(tile_w > 0 && tile_h > 0);
}

UI::Scene::Scene(Scene* parent, const SDL_Rect& rect, int tile_w, int tile_h) noexcept :
	parent(parent),
	viewport(&parent->viewport.getEngine(), rect),
	tile_w(tile_w),
	tile_h(tile_h)
{
	assert(tile_w > 0 && tile_h > 0);
}

UI::Scene* UI::Scene::addScene(const SDL_Rect& p, int tile_w, int tile_h) noexcept
{
	subscenes.emplace_back(this, p, tile_w, tile_h);
	return &subscenes.back();
}

int UI::Scene::drawTile(const EngineSDL::Texture& tx, int x_tiles, int y_tiles) noexcept
{
	auto old_v = viewport.getEngine().getViewport();
	if(&viewport != old_v){
		viewport.lockOn();
	}

	int ret = tx.render(x_tiles * tile_w, y_tiles * tile_h);

	old_v->lockOn();
	return ret;
}

int UI::Scene::drawTile(const EngineSDL::Texture* tx, int x_tiles, int y_tiles) noexcept
{
	return drawTile(*tx, x_tiles, y_tiles);
}
