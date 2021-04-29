#include <vector>
#include <memory>


class Core
{
public:
	struct XY
	{
		int x;
		int y;
	};
	class Item
	{

	};
	class Cell
	{
	public:
		Cell(int type_) : type(type_) {}
		~Cell() {}
		int type;
		//std::stack<std::unique_ptr<Item>> items;
	};
	class Level
	{
	public:
		int w, h;
		std::vector<std::vector<std::unique_ptr<Cell>>> cells;
		//cells(h_, (std::vector<std::unique_ptr<Cell>>(w_))) {}
		Level(int w_, int h_) : w(w_), h(h_)
		{
			cells.reserve(h);
			for(int j = 0; j < h; j++)
			{
//				for(int i = 0; i < w; i++)
//				{
					cells.push_back(std::vector<std::unique_ptr<Cell>>(w));
//				}
			}
		}
		void init()
		{
			for(int y = 1; y < h - 1; y++)
			{
				for(int x = 1; x < w - 1; x++)
				{
					int type;
					if((y > 1 && y < h - 2) && (x > 1 && x < w - 2))
						type = 1;
					else
						type = 0;

					cells[y][x] = std::make_unique<Cell>(type);
				}
			}
		}
		int getWidth() { return cells.back().size(); }
		int getHeight() { return cells.size(); }

	};

	std::unique_ptr<Level> level;

	Core(int level_w, int level_h) noexcept
		: level(std::make_unique<Level>(level_w, level_h)) {}
	void update() {}
};

