#include "enginesdl.h"
//#include "dbg.h"

#include <string>
#include <cassert>


EngineSDL::Engine::Engine()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0 || TTF_Init() != 0)
		throwSDLError();
}

EngineSDL::Engine::~Engine() noexcept
{
	TTF_Quit();
	SDL_Quit();
}

EngineSDL::Window::Window(const std::string& title, int w, int h)
{
	ptr = SDL_CreateWindow(
				title.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				w,
				h,
				SDL_WINDOW_SHOWN);
	if(ptr == nullptr)
		throwSDLError();
}

EngineSDL::Window::~Window() noexcept
{
	if(ptr != nullptr) SDL_DestroyWindow(ptr);
}

EngineSDL::Renderer::Renderer(SDL_Window* window)
{
	ptr = SDL_CreateRenderer(
				window,
				-1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ptr == nullptr)
		throwSDLError();
	SDL_SetRenderDrawColor(this->getPtr(), 255, 0, 0, 255);
}

EngineSDL::Renderer::~Renderer() noexcept
{
	if(ptr != nullptr) SDL_DestroyRenderer(ptr);
}

EngineSDL::Font::Font(const std::string& path, int size)
{
	//DEBUG("Creating Font object");
	ptr = TTF_OpenFont(path.c_str(), size);
	if(ptr == nullptr)
		throwSDLError();
	//DEBUG("Font address: %p", ptr);
}

EngineSDL::Font::~Font() noexcept
{
	//DEBUG("Destroying Font object %p", ptr);
	if(ptr != nullptr) TTF_CloseFont(ptr);
}

int EngineSDL::setupFont(const std::string& path, int size)
{
	//DEBUG("Setting up font");
	font = std::make_unique<Font>(path, size);
	return font == nullptr;
}

EngineSDL::Tileset EngineSDL::loadTileset(const std::string& path, int tile_w, int tile_h)
{
	if((tile_h <= 0) || (tile_w <= 0))
		throw std::runtime_error("Bad tileset dimensions");

	Surface image(SDL_LoadBMP(path.c_str()));
	if(image.get() == nullptr) throwSDLError();

	if((image.get()->clip_rect.h % tile_h) || (image.get()->clip_rect.w % tile_w))
		throw std::runtime_error("Bad tileset dimensions");

	Uint8 r, g, b;
	SDL_GetRGB(*(Uint32*)image.get()->pixels, image.get()->format, &r, &g, &b);
	SDL_SetColorKey(image.get(), SDL_TRUE, SDL_MapRGB(image.get()->format, r, g, b));

	int size = image.get()->clip_rect.w / tile_w * image.get()->clip_rect.h / tile_h;
	std::vector<Texture> tiles(size);

	auto iter = tiles.begin();
	for(int y = 0; y < image.get()->clip_rect.h; y += tile_h)
	{
		for(int x = 0; x < image.get()->clip_rect.w; x += tile_w)
		{
			Surface target(SDL_CreateRGBSurface(
				image.get()->flags,
				tile_w,
				tile_h,
				image.get()->format->BitsPerPixel,
				image.get()->format->Rmask,
				image.get()->format->Gmask,
				image.get()->format->Bmask,
				image.get()->format->Amask));
			if(target.get() == nullptr) throwSDLError();

			SDL_Rect rect = {x, y, tile_w, tile_h};
			if(SDL_BlitSurface(image.get(), &rect, target.get(), 0))
				throwSDLError();

			SDL_Texture* tx = SDL_CreateTextureFromSurface(renderer.getPtr(), target.get());
			if(tx == nullptr) throwSDLError();

			*iter = Texture(this, tx);
			iter++;
		}
	}
	return Tileset(std::move(tiles), tile_w, tile_h);
}

EngineSDL::Texture EngineSDL::makeTxFromText(const std::string& text, SDL_Color color)
{
	//DEBUG("Creating Texture object");
	if(font == nullptr) throw std::runtime_error("Font is not initialized");

	std::unique_ptr<EngineSDL::Surface> surf = makeSurfaceFromText(font.get(), text, color);

	SDL_Texture *texture_ll = SDL_CreateTextureFromSurface(renderer.getPtr(), surf->get());
	if(texture_ll == nullptr)
		throwSDLError();

	//DEBUG("Texture address: %p", texture_ll);
	return Texture(this, texture_ll);
}

EngineSDL::Texture EngineSDL::combineTx(const EngineSDL::Texture& tx1, const EngineSDL::Texture& tx2)
{
	//DEBUG("Combining textures %p and %p", tx1->get(), tx2->get());
	int w1, h1;
	Uint32 fmt;
	SDL_QueryTexture(tx1.get(), &fmt, nullptr, &w1, &h1);

	SDL_Texture* target_tx = SDL_CreateTexture(
		renderer.getPtr(),
		fmt,
		SDL_TEXTUREACCESS_TARGET,
		w1, h1
	);
	if(target_tx == nullptr)
		throwSDLError();

	SDL_SetRenderTarget(renderer.getPtr(), target_tx);
	SDL_RenderClear(renderer.getPtr());
	SDL_RenderCopy(renderer.getPtr(), tx1.get(), nullptr, nullptr);

	int w2, h2;
	SDL_QueryTexture(tx2.get(), nullptr, nullptr, &w2, &h2);
	SDL_Rect dest_rect{(w1-w2)/2, (h1-h2)/2, w2, h2};

	SDL_RenderCopy(renderer.getPtr(), tx2.get(), nullptr, &dest_rect);
	// Reset renderer
	SDL_SetRenderTarget(renderer.getPtr(), nullptr);

	//DEBUG("Combination result: %p", target_tx);
	return Texture(this, target_tx);
}

EngineSDL::Texture EngineSDL::makeSquareTx(int w, int h, SDL_Color& color)
{
	//DEBUG("Creating square Texture object");
	std::unique_ptr<EngineSDL::Surface> surf = makeSquareSurface(w, h, color);

	SDL_Texture *texture_ll = SDL_CreateTextureFromSurface(renderer.getPtr(), surf->get());
	if(texture_ll == nullptr)
		throwSDLError();

	//DEBUG("Texture address: %p", texture_ll);
	return Texture(this, texture_ll);
}

void EngineSDL::renderTx(const EngineSDL::Texture& texture, int x, int y) noexcept
{
	renderTx(&texture, x, y);
}

void EngineSDL::renderTx(const EngineSDL::Texture* texture, int x, int y) noexcept
{
	//DEBUG("Rendering texture %p", texture ? texture->getPtr() : nullptr);
	if(texture == nullptr)
		return;

	// Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	// Query the texture to get its width and height to use
	SDL_QueryTexture(texture->get(), NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer.getPtr(), texture->get(), NULL, &dst);
}

void EngineSDL::setKeyboardIgnored() noexcept
{
	SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
	SDL_EventState(SDL_KEYUP, SDL_IGNORE);
}

EngineSDL::Texture& EngineSDL::Texture::operator=(EngineSDL::Texture&& tx) noexcept
{
	if(this != &tx) {}

	if(ptr != nullptr){
		//DEBUG("Destroying Texture object %p", ptr);
		SDL_DestroyTexture(ptr);
	}

	this->ptr = tx.get();
	this->parent = &tx.getEngine();
	tx.reset();

	return *this;
}

EngineSDL::Texture::Texture(EngineSDL* parent, const std::string& text, SDL_Color color) :
	Texture(parent->makeTxFromText(text, color))
{

}

EngineSDL::Texture::Texture(const EngineSDL::Texture& tx1, const EngineSDL::Texture& tx2) :
	Texture(tx1.getEngine().combineTx(tx1, tx2))
{

}

EngineSDL::Texture::~Texture() noexcept
{
	if(ptr != nullptr)
	{
		//DEBUG("Destroying Texture object %p", ptr);
		SDL_DestroyTexture(ptr);
	}
}

void EngineSDL::Texture::render(int x, int y) const noexcept
{
	assert(parent);
	parent->renderTx(this, x, y);
}

EngineSDL::Surface::~Surface() noexcept
{
	if(ptr != nullptr)
	{
		//DEBUG("Destroying Surface object %p", ptr);
		SDL_FreeSurface(ptr);
	}
}

std::unique_ptr<EngineSDL::Surface> EngineSDL::makeSurfaceFromText(
	EngineSDL::Font* font,
	const std::string& text,
	SDL_Color& color)
{
	//DEBUG("Creating Surface object from text");
	SDL_Surface *surf = TTF_RenderText_Blended(font->get(), text.c_str(), color);
	if(surf == nullptr) throw std::runtime_error(TTF_GetError());

	//DEBUG("Surface address: %p", surf);
	return std::make_unique<EngineSDL::Surface>(surf);
}

std::unique_ptr<EngineSDL::Surface> EngineSDL::makeSquareSurface(int w, int h, SDL_Color& color)
{
	//DEBUG("Creating square Surface object");
	SDL_Surface* surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	if(surf == nullptr)
		throwSDLError();
	SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, color.r, color.g, color.b));

	//DEBUG("Surface address: %p", surf);
	return std::make_unique<EngineSDL::Surface>(surf);
}

EngineSDL::Tileset::Tileset(std::vector<EngineSDL::Texture> tileset, int tile_w, int tile_h) noexcept :
	tiles(std::move(tileset)),
	tile_w(tile_w),
	tile_h(tile_h)
{}

EngineSDL::Tileset::Tileset(EngineSDL::Tileset&& ts) noexcept : tiles(std::move(ts.tiles))
{
	tile_w = ts.tile_w;
	tile_h = ts.tile_h;
}
