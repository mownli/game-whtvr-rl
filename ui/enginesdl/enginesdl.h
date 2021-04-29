#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
		void render(int x, int y) const noexcept;
	};

	class Surface
	{
		SDL_Surface* ptr = nullptr;
	public:
		Surface(SDL_Surface* surface) noexcept : ptr(surface) {};
		~Surface() noexcept;

		SDL_Surface* get() const noexcept { return ptr; }
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
	Tileset loadTileset(const std::string& path, int tile_w, int tile_h);
	Texture makeTxFromText(const std::string& text, SDL_Color color);
	Texture combineTx(const Texture& tx1, const Texture& tx2);
	Texture makeSquareTx(int w, int h, SDL_Color& color);
	void clear() noexcept { renderer.clear(); }
	void update() noexcept { renderer.setPresent(); }
	void renderTx(const Texture* texture, int x, int y) noexcept;
	void renderTx(const Texture& texture, int x, int y) noexcept;
	void setKeyboardIgnored() noexcept;

private:
	class Engine
	{
	public:
		Engine();
		~Engine() noexcept;
	};// engine; // Needs to be at the top of the class stack

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

	Engine engine;
	Window win;
	Renderer renderer;
	std::unique_ptr<Font> font;

	std::unique_ptr<Surface> makeSurfaceFromText(
		Font* font,
		const std::string& text,
		SDL_Color& color);
	std::unique_ptr<Surface> makeSquareSurface(int w, int h, SDL_Color& color);
	static void throwSDLError() { throw std::runtime_error(SDL_GetError()); }
};

#endif // SDL_H
