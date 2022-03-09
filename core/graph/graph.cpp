#include "graph.h"

#include <cstdio>
#include <limits>
#include <algorithm>
#include <cassert>


void Graph::print() const noexcept
{
	for(size_t i = 0; i < matrix.size(); ++i)
	{
		printf("%zu:\n", i);
		for(const auto& adj : matrix[i])
		{
			printf("(%d, %d), ", adj.first, adj.second);
		}
		printf("\n");
	}
}

Graph::vertex_index_t Graph::queue_get_next(
	const std::vector<bool>& visited,
	Graph::queue_unvisited_t& q) noexcept
{
	if(q.empty())
		return std::numeric_limits<vertex_index_t>::max();

	vertex_index_t i = q.top().first;
	if(visited[i] == true)
	{
		q.pop();
		return queue_get_next(visited, q);
	}
	else
	{
		return i;
	}
}

std::vector<Graph::edge_weight_t> Graph::dijkstra(
	const vertex_index_t src,
	const vertex_index_t dest,
	const heuristic_t heuristic,
	const weight_pr_t weight_pr) const noexcept
{
	assert(src >= 0);
	assert(dest >= 0);
	assert(src < static_cast<vertex_index_t>(matrix.size()));
	assert(dest < static_cast<vertex_index_t>(matrix.size()));

	std::vector<edge_weight_t> distances(matrix.size(), std::numeric_limits<edge_weight_t>::max());
	std::vector<bool> visited(matrix.size(), false);

	// TODO: change to set of pairs
	std::vector<vertex_index_t> parents(matrix.size(), std::numeric_limits<vertex_index_t>::max());

	queue_unvisited_t queue_vertex(
		[&](const vertex_id_dist_pair_t& p1, const vertex_id_dist_pair_t& p2)
		{
			const auto h1 = heuristic(dest, p1.first);
			const auto h2 = heuristic(dest, p2.first);
			return p1.second + h1 > p2.second + h2;
		}
	);

	distances[src] = 0;
	queue_vertex.emplace(src, 0);
	for(
		vertex_index_t current = src;
		current != dest && !queue_vertex.empty();
		current = queue_get_next(visited, queue_vertex))
	{
		queue_vertex.pop();
		visited[current] = true;

		for(const auto& i : matrix[current])
		{
			const auto additional_weight = weight_pr(current, i.first);
			if(additional_weight == -1)
				continue;

			if(visited[i.first] == false)
			{
				const edge_weight_t dist =
					distances[current] +
					i.second +
					additional_weight;

				if(dist < distances[i.first])
				{
					distances[i.first] = dist;
					parents[i.first] = current;
					queue_vertex.emplace(i.first, dist);
				}
			}
		}
	}

	std::vector<vertex_index_t> path;
	for(
		vertex_index_t i = dest;
		parents[i] != std::numeric_limits<vertex_index_t>::max();
		i = parents[i])
	{
		path.push_back(i);
	}
	if(!path.empty())
	{
		//path.push_back(src);
		std::reverse(path.begin(), path.end());
	}

	return path;
}

void Graph::add_edge(
	const vertex_index_t src,
	const vertex_index_t dest,
	const edge_weight_t weight) noexcept
{
	assert(src >= 0 && src < static_cast<vertex_index_t>(matrix.size()));
	assert(dest >= 0 && dest < static_cast<vertex_index_t>(matrix.size()));

	{
		vertex_id_dist_pair_t* p = nullptr;
		for(auto& i : matrix[src])
		{
			if(i.first == dest)
			{
				p = &i;
				break;
			}
		}
		if(p)
			p->second = weight;
		else
			matrix[src].emplace_back(dest, weight);
	}

	// Mirror
	{
		vertex_id_dist_pair_t* p = nullptr;
		for(auto& i : matrix[dest])
		{
			if(i.first == src)
			{
				p = &i;
				break;
			}
		}
		if(p)
			p->second = weight;
		else
			matrix[dest].emplace_back(src, weight);
	}

//	auto predicate_src = [&](const std::pair<vertex_index_t, int>& i)
//	{
//		return i.first == dest;
//	};

//	auto iter = std::find_if(matrix[src].begin(), matrix[src].end(), predicate_src);
//	if(iter != std::end(matrix[src]))
//		iter->second = weight;
//	else
//		matrix[src].emplace_back(dest, weight);

//	// Mirror
//	auto predicate_dest = [&](const std::pair<vertex_index_t, int>& i)
//	{
//		return i.first == src;
//	};

//	iter = std::find_if(matrix[dest].begin(), matrix[dest].end(), predicate_dest);
//	if(iter != std::end(matrix[dest]))
//		iter->second = weight;
//	else
	//		matrix[dest].emplace_back(src, weight);
}

Graph::edge_weight_t Graph::get_edge_weight(
	const vertex_index_t src,
	const vertex_index_t dest) const noexcept
{
	assert(src >= 0 && src < static_cast<vertex_index_t>(matrix.size()));
	assert(dest >= 0 && dest < static_cast<vertex_index_t>(matrix.size()));

	for(const auto& i : matrix[src])
	{
		if(i.first == dest)
		{
			return i.second;
		}
	}

	return std::numeric_limits<int>::max();

//	auto predicate = [&](const std::pair<vertex_index_t, int>& i)
//	{
//		return i.first == dest;
//	};
//	auto iter = std::find_if(matrix[src].cbegin(), matrix[src].cend(), predicate);

//	return iter != std::end(matrix[src]) ? iter->second : std::numeric_limits<int>::max();
}

Graph::DetailedInfo Graph::dijkstra_detailed(
	const vertex_index_t src,
	const vertex_index_t dest,
	const heuristic_t heuristic,
	const weight_pr_t weight_pr) const noexcept
{
	assert(src >= 0);
	assert(dest >= 0);
	assert(src < static_cast<vertex_index_t>(matrix.size()));
	assert(dest < static_cast<vertex_index_t>(matrix.size()));

	std::vector<edge_weight_t> distances(matrix.size(), std::numeric_limits<edge_weight_t>::max());
	std::vector<bool> visited(matrix.size(), false);

	// TODO: change to set of pairs
	std::vector<vertex_index_t> parents(matrix.size(), std::numeric_limits<vertex_index_t>::max());

	queue_unvisited_t queue_vertex(
		[&](const vertex_id_dist_pair_t& p1, const vertex_id_dist_pair_t& p2)
		{
			const auto h1 = heuristic(dest, p1.first);
			const auto h2 = heuristic(dest, p2.first);
			return p1.second + h1 > p2.second + h2;
		}
	);

	distances[src] = 0;
	queue_vertex.emplace(src, 0);
	for(
		vertex_index_t current = src;
		current != dest && !queue_vertex.empty();
		current = queue_get_next(visited, queue_vertex))
	{
		queue_vertex.pop();
		visited[current] = true;

		for(const auto& i : matrix[current])
		{
			const auto additional_weight = weight_pr(current, i.first);
			if(additional_weight == -1)
				continue;

			if(visited[i.first] == false)
			{
				const edge_weight_t dist =
					distances[current] +
					i.second +
					additional_weight;

				if(dist < distances[i.first])
				{
					distances[i.first] = dist;
					parents[i.first] = current;
					queue_vertex.emplace(i.first, dist);
				}
			}
		}
	}

	DetailedInfo ret;
	for(
		vertex_index_t i = dest;
		parents[i] != std::numeric_limits<vertex_index_t>::max();
		i = parents[i])
	{
		ret.path.push_back(i);
	}
	if(!ret.path.empty())
	{
		//path.push_back(src);
		std::reverse(ret.path.begin(), ret.path.end());
	}

	for(decltype(visited.size()) i = 0; i < visited.size(); ++i)
	{
		if(visited[i])
			ret.visited.push_back(i);
	}

	return ret;
}
