#include "enginesdl/enginesdl.h"
#include "dbg.h"

#include <cassert>
#include <functional>
#include <map>
#include <unordered_map>
#include <list>


class UI
{
	EngineSDL sdl;
	//std::map<SDL_Scancode, std::function<void()>> callbacks;
	std::unordered_map<SDL_Scancode, std::function<void(bool down)>> callbacks;

public:
	class SceneAbstract
	{
	protected:
		SceneAbstract* parent = nullptr;
	public:
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
		std::list<std::unique_ptr<SceneAbstract>> subscenes;

		SceneAbstract* addScene(std::unique_ptr<SceneAbstract> scene, int x, int y) noexcept;
		virtual void drawTile(const EngineSDL::Texture& tx, int x, int y) = 0;
		virtual ~SceneAbstract() {}
	};

	class Scene : public SceneAbstract
	{
	public:
		Scene(int w, int h) noexcept;
		virtual void drawTile(const EngineSDL::Texture& tx, int x, int y) noexcept override;
	};

	class Canvas : public SceneAbstract
	{
		//EngineSDL& sdl;
	public:
		EngineSDL::Tileset tileset;

		//Canvas(EngineSDL& sdl, int w, int h, EngineSDL::Tileset ts) noexcept;
		Canvas(int w, int h, EngineSDL::Tileset ts) noexcept;
		void drawTile(const EngineSDL::Texture& tx, int x, int y) noexcept override ;
		int getWidthInTiles() const noexcept { return w / tileset.tile_w; }
		int getHeightInTiles() const noexcept { return h / tileset.tile_h; }
	};

	class ScrollableScene : public Scene
	{
	public:
		enum class Direction {LEFT, RIGHT, UP, DOWN};

		int x_vel = 0;
		int y_vel = 0;
		int x_delta = 0;
		int y_delta = 0;

		ScrollableScene(int w, int h) noexcept;
		void scroll(int amount_x, int amount_y) noexcept;
//		void setVelocityX(int vel) noexcept { x_vel = vel; }
//		void setVelocityY(int vel) noexcept { x_vel = vel; }
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
	Canvas canvas;

public:
	UI(Options* opts);
	//void loadTileset(const std::string& tileset_path, int tile_w, int tile_h);
	void regKeyEventCallback(SDL_Scancode scancode, std::function<void(bool pressed)>);
	int eventLoop();
	void clear() noexcept { sdl.clear(); }
	void update() noexcept { sdl.update(); }
	void drawTile(SceneAbstract& screen, const EngineSDL::Texture& tx, int x, int y) noexcept;
};
