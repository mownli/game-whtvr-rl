#include "level.h"
#include "coordinates.h"

#include <fstream>
#include <cmath>


void Level::fill_graph(size_t w, size_t h) noexcept
{
	for(size_t row = 0; row < h; ++row)
	{
		for(size_t col = 0; col < w; ++col)
		{
			if(col < w-1)
				graph.add_edge(col + w*row, col + w*row + 1, 1);
			if(row < h-1)
				graph.add_edge(col + w*row, col + w*(row+1) , 1);

			if(row < h-1)
			{
				if(col > 0)
					graph.add_edge(col + w*row, col-1 + w*(row+1) , 1);
				if(col < w-1)
					graph.add_edge(col + w*row, col+1 + w*(row+1) , 1);
			}
		}
	}
}

Coordinates Level::vertex_to_xy(size_t vertex) const noexcept
{
	const auto w = getWidth();
	const int y = vertex / w;
	const int x = vertex % w;

	return {x, y};
}

size_t Level::xy_to_vertex(const Coordinates& xy) const noexcept
{
	return xy.y * getWidth() + xy.x;
}

Level::Level(size_t w, size_t h) :
	graph(w * h),
	cells(h, std::vector<Cell>(w, Cell(Cell::CellType::INVALID)))
{
	// TODO: level size check

	fill_graph(w, h);

//	cells.reserve(h);
//	for(int j = 0; j < h; j++)
//	{
//		//				for(int i = 0; i < w; i++)
//		//				{

//		//cells.push_back(std::vector<std::unique_ptr<Cell>>(w));
//		cells.push_back(std::vector<Cell>(w));

//		//				}
//	}

	std::ifstream f("/home/gyro/docs/prog/projects/kkk/map");

	if(std::string tmp; f.is_open())
	{
		for(size_t y = 0; getline(f, tmp); ++y)
		{
			for(size_t x = 0; x < tmp.size(); ++x)
			{
				switch(tmp[x])
				{
				case '.':
					cells[y][x].type = Cell::CellType::SIMPLE;
					break;
				case '#':
					cells[y][x].type = Cell::CellType::WALL;
					break;
				default:
					cells[y][x].type = Cell::CellType::INVALID;
				}
			}
		}
	  }

//	for(int y = 1; y < h - 1; y++)
//	{
//		for(int x = 1; x < w - 1; x++)
//		{
//			int type;
//			if((y > 1 && y < h - 2) && (x > 1 && x < w - 2))
//				type = 1;
//			else
//				type = 0;

//			cells[y][x] = std::make_unique<Cell>(type);
//		}
//	}
}

std::vector<Coordinates> Level::find_path(const Coordinates& from,
	const Coordinates& to,
	//Graph::heuristic_t heur_pr,
	Graph::weight_pr_t weight_pr) const noexcept
{
	Graph::heuristic_t heur = [&](int dest, int current) -> int
	{
		const auto dest_xy = vertex_to_xy(dest);
		const auto current_xy = vertex_to_xy(current);

		//return std::max(abs(dest_xy.y - current_xy.y), abs(dest_xy.x - current_xy.x));
		return sqrt(pow(dest_xy.y - current_xy.y, 2) + pow(dest_xy.x - current_xy.x, 2));
	};

	const auto path = graph.find_path(
		xy_to_vertex(from),
		xy_to_vertex(to),
		heur,
		weight_pr);

	std::vector<Coordinates> ret;
	for(const auto& i : path)
		ret.push_back(vertex_to_xy(i));
	return ret;
}
