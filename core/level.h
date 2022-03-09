#ifndef LEVEL_H
#define LEVEL_H

#include "graph/graph.h"
#include "cell.h"

class Coordinates;

#include <vector>


class Level
{
	Graph graph;

	void fill_graph(size_t w, size_t h) noexcept;

public:
	std::vector<std::vector<Cell>> cells;

	Level(size_t w, size_t h);

	int getWidth() const noexcept { return cells.back().size(); }

	int getHeight() const noexcept { return cells.size(); }

	Coordinates vertex_to_xy(size_t vertex) const noexcept;

	size_t xy_to_vertex(const Coordinates& xy) const noexcept;

	std::vector<Coordinates> find_path(
		const Coordinates& from,
		const Coordinates& to,
		//Graph::heuristic_t heur,
		Graph::weight_pr_t weight_pr) const noexcept;
};

#endif // LEVEL_H
