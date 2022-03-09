#include "enginesdl/enginesdl.h"

#include <cassert>
#include <functional>
#include <unordered_map>
#include <list>


class UI
{
	EngineSDL sdl;
	std::unordered_map<SDL_Scancode, std::function<void(bool down)>> callbacks;

public:
	class Scene
	{
		Scene* parent = nullptr;
		EngineSDL::Viewport viewport;

	public:
		bool active = true;
		int tile_w = 1;
		int tile_h = 1;
		int x_vel = 0;
		int y_vel = 0;
		int x_delta_tiles = 0;
		int y_delta_tiles = 0;
		std::list<Scene> subscenes;

		Scene(EngineSDL* sdl, const SDL_Rect& rect, int tile_w, int tile_h) noexcept;
		Scene(Scene* parent, const SDL_Rect& rect, int tile_w, int tile_h) noexcept;
		Scene* addScene(const SDL_Rect& p, int tile_w, int tile_h) noexcept;
		int drawTile(const EngineSDL::Texture& tx, int x_tiles, int y_tiles) noexcept;
		int drawTile(const EngineSDL::Texture* tx, int x_tiles, int y_tiles) noexcept;
		friend void drawTile(const SDL_Color& c, int x_tiles, int y_tiles) noexcept;
		int getWidthInTiles() const noexcept { return viewport.getW() / tile_w; };
		int getHeightInTiles() const noexcept { return viewport.getH() / tile_h; };
		void scroll(int amount_x, int amount_y) noexcept;
		void resetPosition() noexcept { x_delta_tiles = 0; y_delta_tiles = 0; }
		void updateRecursive() noexcept;
		void lockOn() noexcept { viewport.lockOn(); }
	};

	struct Options
	{
		std::string win_title;
		int win_w = 0;
		int win_h = 0;
		std::string tileset_path;
		int tile_w = 0;
		int tile_h = 0;
	};

	Options params;
	Scene canvas;
	Scene* target = &canvas;
	std::vector<std::unique_ptr<EngineSDL::Tileset>> tilesets;

public:
	UI(Options* opts);
	void loadTileset(const std::string& tileset_path, int tile_w, int tile_h, const SDL_Color& color_key);
	void regKeyEventCallback(SDL_Scancode scancode, std::function<void(bool key_down)>);
	int eventLoop();
	void clear() noexcept { sdl.clear(); }
	void update() noexcept { sdl.update(); }
	int drawTile(const EngineSDL::Texture* tx, int x_tiles, int y_tiles) noexcept;
	void drawTile(const SDL_Color& c, int x_tiles, int y_tiles) noexcept;
	void updateScenes() noexcept;
	void setTarget(Scene* scene) noexcept;
};
