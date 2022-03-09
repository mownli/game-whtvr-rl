#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <memory>
#include <vector>
#include <stdexcept>


class EngineSDL
{
public:
	EngineSDL(const std::string& title, int w, int h) : win(title, w, h), renderer(win.getPtr()) {}

	class Font
	{
		TTF_Font* ptr = nullptr;
	public:
		Font(const std::string& path, int size);
		~Font() noexcept;

		TTF_Font* get() const noexcept { return ptr; }
	};

	class Texture
	{
		EngineSDL* parent = nullptr;
		SDL_Texture* ptr = nullptr;
	public:
		Texture() noexcept {};
		Texture(Texture&& tx) noexcept : parent(&tx.getEngine()), ptr(tx.get()) { tx.reset(); }
		Texture& operator=(Texture&& tx) noexcept;
		Texture& operator=(const Texture&) = delete;
		Texture(const Texture& obj) = delete;
		Texture(EngineSDL* parent, SDL_Texture* tx_ll) noexcept : parent(parent), ptr(tx_ll) {};
		Texture(EngineSDL* parent, const std::string& text, SDL_Color color);
		Texture(const Texture& tx1, const Texture& tx2);
		~Texture() noexcept;

		SDL_Texture* get() const noexcept { return ptr; }
		EngineSDL& getEngine() const noexcept { return *parent; }
		void reset() noexcept { ptr = nullptr; }
		int render(int x, int y) const noexcept;
		void setColor(Uint8 r, Uint8 g, Uint8 b) noexcept;
	};

	class Viewport
	{
		EngineSDL* parent;
		SDL_Rect rect;
	public:
		Viewport(EngineSDL* parent, const SDL_Rect& rect) noexcept;
		void lockOn() noexcept;
		EngineSDL& getEngine() const noexcept { return *parent; }
		int getX() const noexcept { return rect.x; }
		int getY() const noexcept { return rect.y; }
		int getW() const noexcept { return rect.w; }
		int getH() const noexcept { return rect.h; }
		const SDL_Rect& getRect() const noexcept { return rect; }
	};

	class Tileset
	{
	public:
		std::vector<EngineSDL::Texture> tiles;
		int tile_w;
		int tile_h;

		Tileset(std::vector<EngineSDL::Texture> tileset, int tile_w, int tile_h) noexcept;
		Tileset(Tileset&& ts) noexcept;
		EngineSDL::Texture& operator[](int index) noexcept { return tiles[index]; }
	};

	int setupFont(const std::string& path, int size);
	Tileset loadTileset(const std::string& path, int tile_w, int tile_h, const SDL_Color& color_key);
	Texture makeTxFromText(const std::string& text, SDL_Color color);
	Texture combineTx(const Texture& tx1, const Texture& tx2);
	Texture makeSquareTx(int w, int h, SDL_Color& color);
	void clear() noexcept { renderer.clear(); }
	void update() noexcept { renderer.setPresent(); }
	int renderTx(const Texture* texture, int x, int y) noexcept;
	int renderTx(const Texture& texture, int x, int y) noexcept;
	void setKeyboardIgnored() noexcept;
	void renderRect(int x, int y, int w, int h, const SDL_Color& c) noexcept;
	void setViewport(Viewport* vp) noexcept;
	Viewport* getViewport() const noexcept { return current_viewport; }

private:
	class Surface
	{
		SDL_Surface* ptr = nullptr;
	public:
		Surface(SDL_Surface* surface) noexcept : ptr(surface) {};
		~Surface() noexcept;

		SDL_Surface* get() const noexcept { return ptr; }
	};

	class Engine
	{
	public:
		Engine();
		~Engine() noexcept;
	};

	class Window
	{
		SDL_Window *ptr = nullptr;
	public:
		Window(const std::string& title, int w, int h);
		~Window() noexcept;

		SDL_Window* getPtr() const noexcept { return ptr; }
	};

	class Renderer
	{
		SDL_Renderer *ptr = nullptr;
	public:
		Renderer(SDL_Window* window);
		~Renderer() noexcept;

		void clear() noexcept { SDL_RenderClear(ptr); }
		void setPresent() noexcept { SDL_RenderPresent(ptr); }
		SDL_Renderer* getPtr() const noexcept { return ptr; }
	};

	class Imglib
	{
	  public:
		int load(int flags);
		~Imglib() { IMG_Quit(); }
	};

	Engine engine;
	Window win;
	Renderer renderer;
	Imglib imglib;
	std::unique_ptr<Font> font;

	Viewport* current_viewport = nullptr;

	std::unique_ptr<Surface> makeSurfaceFromText(
		Font* font,
		const std::string& text,
		SDL_Color& color);
	std::unique_ptr<Surface> makeSquareSurface(int w, int h, SDL_Color& color);
	Surface loadPNG(const std::string& path);
	static void throwSDLError() { throw std::runtime_error(SDL_GetError()); }
};

#endif // SDL_H
