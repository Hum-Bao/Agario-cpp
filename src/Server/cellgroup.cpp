#include "cellgroup.h"

CellGroup::CellGroup() : cell_group_(), active_cells_(1) {}

Cell CellGroup::GetCell(int index) const {
    return cell_group_[index];
}