#include "object_controller.h"

class Unit;
class Level;

#include <vector>
#include <memory>
#include <queue>


class Core : public ObjectController
{
	void apply_unit(std::unique_ptr<Unit> unit, const Coordinates& xy) noexcept;

	void delete_unit(int id) noexcept;

public:
	std::unique_ptr<Level> level;
	bool waiting_for_input = false;

	// available ids
	std::priority_queue<int, std::vector<int>, std::greater<int>> free_ids;
	// id to unit
	std::vector<std::unique_ptr<Unit>> unit_table;
	// unit_id to xy
	std::vector<Coordinates> unit_position_table;
	// xy to unit_id
	std::vector<std::vector<std::vector<int>>> cell_unit_id_table;

	decltype(unit_table)::iterator iter;

	Core(size_t level_w, size_t level_h) noexcept;

	int update() noexcept;

	// ObjectController
	Coordinates get_position(int id) const noexcept override;

	int move_object(int id, const Coordinates& xy) noexcept override;

	Cell::CellType get_cell_type(const Coordinates& xy) const noexcept override;

	std::vector<int> get_cell_object_ids(const Coordinates& xy) const noexcept override;

	Unit* get_object_by_id(int id) const noexcept override;

	std::vector<Coordinates> find_path(
		const Coordinates& from,
		const Coordinates& to,
		//move_heuristic_t predicate,
		weight_pr_t weight_pr) const noexcept override;

	void register_unit(std::unique_ptr<Unit> unit, const Coordinates xy) noexcept override;
};





